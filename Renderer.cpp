#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>

std::shared_ptr<Renderer> Renderer::_instance = nullptr;

const std::string Renderer::UniformVariables::viewMatrix = "glViewMatrix";
const std::string Renderer::UniformVariables::projectionMatrix = "glProjectionMatrix";
const std::string Renderer::UniformVariables::modelMatrix = "glModelMatrix";
const std::string Renderer::UniformVariables::hasTexture = "glHasTexture";

void Renderer::GLFWwindowDeleter::operator()(GLFWwindow* p_ptr) {
	
	glfwDestroyWindow(p_ptr);
}


std::shared_ptr<Renderer> Renderer::GetInstance() {

	if (_instance == nullptr) {
		_instance = std::shared_ptr<Renderer>(new Renderer());
	}

	return _instance;
}

std::shared_ptr<ShaderProgram> Renderer::ShaderFactory(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath) {

	_shaders.push_back(std::shared_ptr<ShaderProgram>(new ShaderProgram(p_vertexShaderPath, p_fragmentShaderPath)));
	_shaders.back()->SetVariable(UniformVariables::projectionMatrix, _projectionMatrix);
	_shaders.back()->SetVariable(UniformVariables::viewMatrix, _camera.GetViewTransformation());
	return _shaders.back();
}

std::shared_ptr<Texture> Renderer::TextureFactory(const std::string& p_texturePath) {

	_textures.push_back(std::shared_ptr<Texture>(new Texture(p_texturePath)));

	if (_textures.back()->_textureData == nullptr) {
		return nullptr;
	}

	return _textures.back();
}

void Renderer::AddObject(Entity& p_object) {

	//obtaining the GraphicsComponent of the Entity
	auto graphicsComponent = p_object.GetComponentOfType<GraphicsComponent>();
	if (graphicsComponent == nullptr) {
		std::cerr << "Object does not have a Graphics Component";
		return;
	}

	if (graphicsComponent->mesh == nullptr) {
		std::cerr << "Object does not have a Mesh";
		return;
	}

	//generating VAO to store buffer data
	glGenVertexArrays(1, &graphicsComponent->_vao);

	//generating the VBO and EBO
	glGenBuffers(1, &graphicsComponent->_vbo);
	glGenBuffers(1, &graphicsComponent->_ebo);

	//activating VAO
	glBindVertexArray(graphicsComponent->_vao);

	//copying data to VBO
	glBindBuffer(GL_ARRAY_BUFFER, graphicsComponent->_vbo);
	auto& vertices = graphicsComponent->mesh->GetVertices();
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), reinterpret_cast<void*>(const_cast<Vertex*>(vertices.data())), GL_STATIC_DRAW);

	//copying data to EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graphicsComponent->_ebo);
	auto& elements = graphicsComponent->mesh->GetElements();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements.size(), reinterpret_cast<void*>(const_cast<unsigned int*>(elements.data())), GL_STATIC_DRAW);

	//TODO: refactor this to not send worthless data if object has no texture
	//setting Position attribute
	glVertexAttribPointer(ShaderAttributes::Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(ShaderAttributes::Position);
	//setting Color attribute
	glVertexAttribPointer(ShaderAttributes::Color, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(ShaderAttributes::Color);
	//setting Texture attribute
	glVertexAttribPointer(ShaderAttributes::TextureCoordinates, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(ShaderAttributes::TextureCoordinates);

	//deactivating VAO
	glBindVertexArray(0);

	_entities.push_back(graphicsComponent);
}

void Renderer::SetPerspective(float p_fov, float p_nearPlane, float p_farPlane) {

	float aspectRatio = static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight);
	_projectionMatrix = glm::perspective(p_fov / 2.f, aspectRatio, p_nearPlane, p_farPlane);

	for (auto& shader : _shaders) {
		shader->SetVariable(UniformVariables::projectionMatrix, _projectionMatrix);
	}
}

void Renderer::RenderFunction() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto& object : _entities) {
		glBindVertexArray(object->_vao);

		//If there is a shader associated we use it
		//else fallback to the default shader
		//object->_entity.Rotate(0.f, 1.f, 0.f, 1);
		auto& shader = object->shaderProgram != nullptr ? object->shaderProgram : _shaders[0];

		shader->SetVariable(UniformVariables::modelMatrix, object->GetModelTransformation());
		glUseProgram(shader->_id);

		shader->SetVariable(UniformVariables::hasTexture, false);
		if (object->texture != nullptr) {
			shader->SetVariable(UniformVariables::hasTexture, true);
			glBindTexture(GL_TEXTURE_2D, object->texture->_id);
		}

		//TODO: IMPORTANT!! design a mechanism to store drawing logic in the object
		//to permit customizing this
		//Update: partially done via storing DrawMode
		glDrawElements(object->mesh->GetDrawMode(), object->mesh->GetElementCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	glfwSwapBuffers(_window.get());
}

void Renderer::CleanupFunction() {

	for (const auto& entity : _entities) {
		glBindVertexArray(entity->_vao);
		//TODO: make an enum for vertex attributes
		glDisableVertexAttribArray(ShaderAttributes::Position);
		glDisableVertexAttribArray(ShaderAttributes::Color);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &entity->_vbo);
		glDeleteBuffers(1, &entity->_ebo);

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &entity->_vao);
	}

	//TODO, bulk delete?
	for (const auto& texture : _textures) {

		glDeleteTextures(1, &texture->_id);
	}

	glUseProgram(0);
	for (const auto& shader : _shaders) {
		glDeleteProgram(shader->_id);
	}
}
Renderer::Renderer() :
	_projectionMatrix(glm::identity<glm::mat4>()),
	_camera()
{

	//init and setup glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//setup and create window
	_window = std::unique_ptr<GLFWwindow, GLFWwindowDeleter>(
		glfwCreateWindow(_windowWidth, _windowHeight, "TavaGL V0.3a", nullptr, nullptr)
	);

	if (_window == nullptr)
	{
		std::cerr << "Failed to create GLFW window \n";
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(_window.get());

	//init glew
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cerr << "Error: " << glewGetErrorString(res) << "\n";
		exit(1);
	}

	//we register a callback when the window is resized to always keep the viewport accurate
	glfwSetFramebufferSizeCallback(_window.get(),
		[](GLFWwindow* p_window, int p_width, int p_height) {
			glViewport(0, 0, p_width, p_height);
			_instance->RenderFunction();
		}
	);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glfwSwapInterval(0);

	//configuring face culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	//loading the default shader
	ShaderFactory("shader.vert", "shader.frag");
	_shaders.front()->SetVariable(UniformVariables::projectionMatrix, _projectionMatrix);
	_shaders.front()->SetVariable(UniformVariables::viewMatrix, _camera.GetViewTransformation());

}

//TODO:
//multithread
void Renderer::Run() {

	double lastTime = glfwGetTime();
	int frameCount = 0;

	while (!glfwWindowShouldClose(_window.get())) {
		glfwPollEvents();
		RenderFunction();
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		frameCount++;

		if (delta >= 1.0) {
			double fps = frameCount / delta;
			std::cout << "Frame Rate: " << fps << " FPS" << std::endl;
			frameCount = 0;
			lastTime = currentTime;
		}
	}

	CleanupFunction();
	glfwTerminate();
}