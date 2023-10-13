#include "Renderer.h"
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>

std::shared_ptr<Renderer> Renderer::_instance = nullptr;

const std::string Renderer::UniformVariables::projectionMatrix = "glProjectionMatrix";
const std::string Renderer::UniformVariables::modelMatrix = "glModelMatrix";
const std::string Renderer::UniformVariables::hasTexture = "glHasTexture";

std::shared_ptr<Renderer> Renderer::GetInstance(){
	
	if (_instance == nullptr){
		_instance = std::shared_ptr<Renderer>(new Renderer());
	}

	return _instance;
}

std::shared_ptr<ShaderProgram> Renderer::ShaderFactory(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath) {

	ShaderProgram program(p_vertexShaderPath, p_fragmentShaderPath);
	_shaders.push_back(std::make_shared<ShaderProgram>(program));
	return _shaders.back();
}

std::shared_ptr<Texture> Renderer::TextureFactory(const std::string& p_texturePath) {
	
	Texture texture(p_texturePath);
	if (texture._textureData == nullptr) {
		return nullptr;
	}

	_textures.push_back(std::make_shared<Texture>(texture));
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

void Renderer::RenderFunction(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto& object : _entities) {
		glBindVertexArray(object->_vao);

		//If there is a shader associated we use it
		//else fallback to the default shader
		auto shader = object->shaderProgram != nullptr ? object->shaderProgram : _shaders[0];

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

	glutPostRedisplay();
	glutSwapBuffers();
}

void Renderer::CleanupFunction(){
	
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

void Renderer::RenderCallback() {
	
	_instance->RenderFunction();
}

void Renderer::CleanupCallback() {

	_instance->CleanupFunction();
}

Renderer::Renderer() :
	_projectionMatrix(glm::identity<glm::mat4>())
{	
	//TODO: look into command line arguments
	int argc = 0;
	char* argv[1] = {};
	glutInit(&argc, argv);

	//setup and create window
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(_windowWidth, _windowHeight);
	glutCreateWindow("TavaGL V0.3a");

	//init opengl context
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cerr << "Error: " << glewGetErrorString(res) << "\n";
		exit(1);
	}

	//set screen clear color
	glClearColor(0.f, 0.f, 0.f, 0.f);
	
	//configuring face culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	//configuring depth test
	glEnable(GL_DEPTH_TEST);

	//loading the default shader
	ShaderFactory("shader.vert", "shader.frag");
	_shaders.front()->SetVariable(UniformVariables::projectionMatrix, _projectionMatrix);

	//registering render and cleanup callbacks
	glutDisplayFunc(RenderCallback);
	glutCloseFunc(CleanupCallback);

}

//TODO: multithread the main loop
//IMPOSSIBLE, GLUT is not thread safe
//Either thread the physics engine or move to GLFW or something
void Renderer::Run() {

	glutMainLoop();
}