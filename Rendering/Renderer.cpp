module;

//don't think this can be converted to a module

//TODO: investigate C5105
#include <Windows.h>
#include <gl/glew.h>
#include <gl/glfw3.h>

module Rendering;

import <glm/gtc/type_ptr.hpp>;

import <iostream>;
import <fstream>;


std::unique_ptr<Renderer> Renderer::_instance = nullptr;

void Renderer::GLFWwindowDeleter::operator()(GLFWwindow* p_ptr) {
	
	glfwDestroyWindow(p_ptr);
}


Renderer& Renderer::GetInstance() {

	if (_instance == nullptr) {
		_instance = std::unique_ptr<Renderer>(new Renderer());
	}

	return *_instance;
}

std::shared_ptr<ShaderProgram> Renderer::ShaderFactory(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath) {

	_shaders.push_back(std::shared_ptr<ShaderProgram>(new ShaderProgram(p_vertexShaderPath, p_fragmentShaderPath)));
	_shaders.back()->SetVariable(UniformVariables::projectionMatrix, _projectionMatrix);

	glm::vec3 lightColor(1.f, 1.f, 1.f);
	glm::vec3 lightPosition(0.f, 0.f, 0.f);
	float ambianceStrength = 1.f;
	float specularStrength = 0.f;
	float diffuseStrength = 0.f;

	if (!_lightSource.expired()) {
		auto lightSource = _lightSource.lock();
		lightColor = lightSource->getLightColor();
		lightPosition = lightSource->getLightPosition();
		ambianceStrength = lightSource->getAmbienceStrength();
		diffuseStrength = lightSource->getDiffuseStrength();
		specularStrength = lightSource->getSpecularStrength();
	}

	_shaders.back()->SetVariable(UniformVariables::lightColor, lightColor);
	_shaders.back()->SetVariable(UniformVariables::lightPosition, lightPosition);
	_shaders.back()->SetVariable(UniformVariables::lightAmbianceStrength, ambianceStrength);
	_shaders.back()->SetVariable(UniformVariables::lightDiffuseStrength, diffuseStrength);
	_shaders.back()->SetVariable(UniformVariables::lightSpecularStrength, specularStrength);

	return _shaders.back();
}

std::shared_ptr<Texture> Renderer::TextureFactory(const std::string& p_texturePath) {

	_textures.push_back(std::shared_ptr<Texture>(new Texture(p_texturePath)));

	if (_textures.back()->_textureData == nullptr) {
		return nullptr;
	}

	return _textures.back();
}

std::shared_ptr<Mesh> Renderer::MeshFactory(const std::vector<Vertex>& p_vertices, const std::vector<unsigned int>& p_indices, const GLenum p_mode) {
	
	return std::shared_ptr<Mesh>(new Mesh(p_vertices, p_indices, p_mode));
}

void Renderer::SetCameraLock(bool p_lock) {
	_cameraLock = p_lock;
}

void Renderer::AddObject(const Entity& p_object) {

	//obtaining the GraphicsComponent of the Entity

	auto graphicsComponentWeak = p_object.TryGetComponentOfType<RenderComponent>();
	if (graphicsComponentWeak.expired()) {
		std::cerr << "Object does not have a Graphics Component";
		return;
	}

	auto graphicsComponent = graphicsComponentWeak.lock();

	if (graphicsComponent->mesh == nullptr) {
		std::cerr << "Graphics Component does not have a Mesh";
		return;
	}

	_entities.push_back(graphicsComponentWeak);
}

void Renderer::Set2DMode(float p_width, float p_height) {

	_camera.SetCameraPosition(0.f, 0.f, 1.f);
	_camera.SetCameraDirection(0.f, 0.f, -1.f);
	SetCameraLock(true);
	_projectionMatrix = glm::ortho(-p_width / 2, p_width / 2, 
									-p_height / 2, p_height / 2, 
									0.1f, 5.f);
	
	for (auto& shader : _shaders) {

		shader->SetVariable(UniformVariables::projectionMatrix, _projectionMatrix);
	}
}

void Renderer::SetPerspective(float p_fov, float p_nearPlane, float p_farPlane) {

	float aspectRatio = static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight);
	_projectionMatrix = glm::perspective(p_fov / 2.f, aspectRatio, p_nearPlane, p_farPlane);

	for (auto& shader : _shaders) {

		shader->SetVariable(UniformVariables::projectionMatrix, _projectionMatrix);
	}
}

void Renderer::SetLightSource(const Entity& p_object) {

	_lightSource = p_object.TryGetComponentOfType<LightSourceComponent>();
	if (_lightSource.expired()) {
		std::cerr << "Object does not contain a LightSourceComponent\n";
	}

	auto lightSource = _lightSource.lock();

	for (auto& shader : _shaders) {
		shader->SetVariable(UniformVariables::lightColor, lightSource->getLightColor());
		shader->SetVariable(UniformVariables::lightPosition, lightSource->getLightPosition());
		shader->SetVariable(UniformVariables::lightAmbianceStrength, lightSource->getAmbienceStrength());
		shader->SetVariable(UniformVariables::lightDiffuseStrength, lightSource->getDiffuseStrength());
		shader->SetVariable(UniformVariables::lightSpecularStrength, lightSource->getSpecularStrength());
	}
}

void Renderer::RenderFunction() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto& entity : _entities) {
		
		//TODO: expired components removal;
		if (entity.expired()) {
			continue;
		}
		auto component = entity.lock();
		auto& mesh = component->mesh;
		if (mesh == nullptr) {
			continue;
		}
		glBindVertexArray(mesh->_vao);

		//If there is a shader associated we use it
		//else fallback to the default shader
		auto shader = component->shaderProgram != nullptr ? component->shaderProgram : _shaders[0];

		shader->SetVariable(UniformVariables::modelMatrix, component->GetModelTransformation());
		shader->SetVariable(UniformVariables::viewMatrix, _camera.GetViewTransformation());
		shader->SetVariable(UniformVariables::cameraPosition, _camera.GetPosition());
		glUseProgram(shader->_id);

		shader->SetVariable(UniformVariables::hasTexture, false);
		if (component->texture != nullptr) {
			shader->SetVariable(UniformVariables::hasTexture, true);
			glBindTexture(GL_TEXTURE_2D, component->texture->_id);
		}

		//TODO: IMPORTANT!! design a mechanism to store drawing logic in the object
		//to permit customizing this
		//Update: partially done via storing DrawMode
		glDrawElements(mesh->GetDrawMode(), mesh->GetElementCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	glfwSwapBuffers(_window.get());
}

void Renderer::CleanupFunction() {

	glUseProgram(0);
}
void Renderer::ComputeTime() {

	double currentTime = glfwGetTime();
	double delta = currentTime - _lastTime;
	_deltaTime = static_cast<float>(delta);
	_fpsDelta += delta;
	_lastTime = currentTime;
	++_frames;
	if (_fpsDelta >= 1.0) {
		double fps = static_cast<double>(_frames) / _fpsDelta;
		std::cout << "Frame Rate: " << fps << " FPS\n";
		_frames = 0;
		_fpsDelta = 0;
	}
}

void Renderer::ProcessInput() {

	glfwPollEvents();

	if (_cameraLock) {
		return;
	}

	if(glfwGetKey(_window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		_focused = false;
	}

	if (!_focused && glfwGetMouseButton(_window.get(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		_focused = true;
		_initial = true;
	}

	bool moveForward = (glfwGetKey(_window.get(), GLFW_KEY_W) == GLFW_PRESS);
	bool moveBackward = (glfwGetKey(_window.get(), GLFW_KEY_S) == GLFW_PRESS);
	bool moveLeft = (glfwGetKey(_window.get(), GLFW_KEY_A) == GLFW_PRESS);
	bool moveRight = (glfwGetKey(_window.get(), GLFW_KEY_D) == GLFW_PRESS);
	bool moveUp = (glfwGetKey(_window.get(), GLFW_KEY_SPACE) == GLFW_PRESS);
	bool moveDown = (glfwGetKey(_window.get(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);

	_camera.MoveCamera({ moveForward, moveBackward, moveLeft, moveRight, moveUp, moveDown }, _deltaTime);
}

void Renderer::MouseCallback(GLFWwindow* _window, double _crtX, double _crtY) {
	
	if (_cameraLock) {
		return;
	}

	if (_focused) {
		if (_initial) {
			_prevX = _crtX;
			_prevY = _crtY;
			_initial = false;
		}
		
		else {
			float offsetX = static_cast<float>(_crtX - _prevX);
			float offsetY =  static_cast<float>(_prevY - _crtY);
			_prevX = _crtX;
			_prevY = _crtY;
		
			_camera.RotateCamera(offsetX, offsetY);
		}
	}
}

Renderer::Renderer() :
	_projectionMatrix(glm::identity<glm::mat4>()),
	_camera(),
	_lightSource(),
	_focused(false),
	_initial(true),
	_cameraLock(false)
{

	//TODO: critical, try and get away with not using this
	timeBeginPeriod(1);
	//init and setup glfw
	GLenum res =  glfwInit();
	if (res == GLFW_FALSE) {
		std::cerr << "GLFW initialization failed\n";
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//setup and create window
	_window = std::unique_ptr<GLFWwindow, GLFWwindowDeleter>(
		glfwCreateWindow(_windowWidth, _windowHeight, "TavaGL V0.6a", nullptr, nullptr)
	);

	if (_window == nullptr)
	{
		std::cerr << "Failed to create GLFW window \n";
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(_window.get());

	//init glew
	res = glewInit();
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

	glfwSetCursorPosCallback(_window.get(), 
		[](GLFWwindow* _window, double _crtX, double _crtY) {
			_instance->MouseCallback(_window, _crtX, _crtY); 
		}
	);
	//VSync 1 = set to Refresh Rate 0 = Unbound
	glfwSwapInterval(1);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	
	//configuring face culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	//loading the default shader
	ShaderFactory("shader.vert", "shader.frag");
}

//TODO:
//multithread
void Renderer::Run() {

	_lastTime = glfwGetTime();
	_frames = 0;
	while (!glfwWindowShouldClose(_window.get())) {
		ProcessInput();
		RenderFunction();
		ComputeTime();
	}
}

Renderer::~Renderer() {

	timeEndPeriod(1);
	CleanupFunction();
	glfwTerminate();
}