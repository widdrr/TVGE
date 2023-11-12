module;

//don't think this can be converted to a module

//TODO: investigate C5105
#include <Windows.h>
#include <gl/glew.h>
#include <gl/glfw3.h>

#include <assimp/material.h>

module Rendering;

import <glm/gtc/type_ptr.hpp>;
import <assimp/Importer.hpp>;
import <assimp/scene.h>;
import <assimp/postprocess.h>;

import <iostream>;
import <fstream>;

std::unique_ptr<Renderer> Renderer::_instance = nullptr;

void Renderer::GLFWwindowDeleter::operator()(GLFWwindow* p_ptr) 
{	
	glfwDestroyWindow(p_ptr);
}

Renderer::Renderer() :
	_projectionMatrix(glm::identity<glm::mat4>()),
	_camera(),
	_focused(false),
	_initial(true),
	_cameraLock(false)
{
	//TODO: critical, try and get away with not using this
	timeBeginPeriod(1);
	//init and setup glfw
	GLenum res = glfwInit();
	if (res == GLFW_FALSE) {
		std::cerr << "GLFW initialization failed\n";
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//setup and create window
	_window = std::unique_ptr<GLFWwindow, GLFWwindowDeleter>(
		glfwCreateWindow(_windowWidth, _windowHeight, "TavaGL V0.9a", nullptr, nullptr)
	);

	if (_window == nullptr) {
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
	_defaultShader = ShaderFactory("shader.vert", "shader.frag");
}

Renderer& Renderer::GetInstance() 
{
	if (_instance == nullptr) {
		_instance = std::unique_ptr<Renderer>(new Renderer());
	}

	return *_instance;
}

Renderer::~Renderer()
{
	timeEndPeriod(1);
	CleanupFunction();
	glfwTerminate();
}

void Renderer::RenderFunction()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto&& entity : _entities) {

		//TODO: expired components removal;
		if (entity.expired()) {
			continue;
		}
		auto&& component = entity.lock();

		for (auto&& mesh : component->_meshes) {
			glBindVertexArray(mesh->_vao);

			//If there is a shader associated we use it
			//else fallback to the default shader
			auto& shader = mesh->_material->_shader;

			shader.SetVariable(UniformVariables::modelMatrix, component->GetModelTransformation());
			_camera.SetCameraVariables(shader);
			_lightSource.lock()->SetLightVariables(shader);
			mesh->_material->SetMaterialVariables();

			glUseProgram(shader._id);

			//TODO: IMPORTANT!! design a mechanism to store drawing logic in the object
			//to permit customizing this
			glDrawElements(GL_TRIANGLES, mesh->_indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}
	}

	glfwSwapBuffers(_window.get());
}

void Renderer::CleanupFunction()
{
	glUseProgram(0);
}

std::shared_ptr<ShaderProgram> Renderer::ShaderFactory(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath) 
{
	const std::string concatPath = p_vertexShaderPath + p_fragmentShaderPath;
	if (_shaders.contains(concatPath)) {
		return _shaders[concatPath];
	}
	
	auto thisShader = std::shared_ptr<ShaderProgram>(new ShaderProgram(p_vertexShaderPath, p_fragmentShaderPath));
	
	if (thisShader->_failed) {
		std::cerr << "Shader Program creation failed, nullptr is returned\n";
		return nullptr;
	}
	
	_shaders[concatPath] = thisShader;
	thisShader->SetVariable(UniformVariables::projectionMatrix, _projectionMatrix);

	return thisShader;
}

std::shared_ptr<Texture> Renderer::TextureFactory(const std::string& p_texturePath)
{
	if (_textures.contains(p_texturePath)) {
		return _textures[p_texturePath];
	}

	auto thisTexture = std::shared_ptr<Texture>(new Texture(p_texturePath));

	if (thisTexture->_textureData == nullptr) {
		std::cerr << "Texture creation failed, nullptr is returned\n";
		return nullptr;
	}
	_textures[p_texturePath] = thisTexture;

	return thisTexture;
}

//TODO: Cache meshes
std::shared_ptr<Mesh> Renderer::MeshFactory(const std::vector<Vertex>& p_vertices, const std::vector<unsigned int>& p_indices, const std::shared_ptr<Material>& p_material, bool p_genNormal)
{
	return std::shared_ptr<Mesh>(new Mesh(p_vertices, p_indices, p_material, p_genNormal));
}

void Renderer::LoadModel(ModelComponent& p_model, const std::string& p_path)
{
	Assimp::Importer importer;
	//TODO: look into preprocess options
	const aiScene* scene = importer.ReadFile(p_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (scene == nullptr){
		std::cerr << "Error loading model: " << importer.GetErrorString() << "\n";
		return;
	}

	if (scene->mRootNode == nullptr) {
		std::cerr << "Assimp Scene is empty\n";
		return;
	}
	
	ProcessAssimpNode(scene->mRootNode, scene, p_model);
}

std::shared_ptr<ShaderProgram> Renderer::DefaultShader()
{
	return _defaultShader;
}

void Renderer::ProcessAssimpNode(aiNode* p_node, const aiScene* p_scene, ModelComponent& p_model)
{
	for (unsigned int i = 0; i < p_node->mNumMeshes; ++i) {
		auto mesh = p_scene->mMeshes[p_node->mMeshes[i]];
		p_model._meshes.push_back(MeshFactory(mesh, p_scene));
	}

	for (unsigned int i = 0; i < p_node->mNumChildren; ++i) {
		ProcessAssimpNode(p_node->mChildren[i], p_scene, p_model);
	}
}

std::shared_ptr<Mesh> Renderer::MeshFactory(aiMesh* p_mesh, const aiScene* p_scene)
{
	std::vector<Vertex> vertices;

	bool hasTextCoords = p_mesh->HasTextureCoords(0);

	for (unsigned int i = 0; i < p_mesh->mNumVertices; ++i) {

		vertices.emplace_back(
			p_mesh->mVertices[i].x, p_mesh->mVertices[i].y, p_mesh->mVertices[i].z,
			hasTextCoords ? p_mesh->mTextureCoords[0][i].x : 0.f,
			hasTextCoords ? p_mesh->mTextureCoords[0][i].y : 0.f,
			p_mesh->mNormals[i].x, p_mesh->mNormals[i].y, p_mesh->mNormals[i].z);
	}

	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < p_mesh->mNumFaces; i++) {
		const aiFace& face = p_mesh->mFaces[i];
		
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	auto material = std::shared_ptr<Material>(new Material(*_defaultShader));
	
	auto assimpMaterial = p_scene->mMaterials[p_mesh->mMaterialIndex];

	aiColor3D output;

	assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, output);
	material->_lightProperties.ambient = glm::vec3(output.r, output.g, output.b);
	assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, output);
	material->_lightProperties.diffuse = glm::vec3(output.r, output.g, output.b);
	assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, output);
	material->_lightProperties.specular = glm::vec3(output.r, output.g, output.b);

	assimpMaterial->Get(AI_MATKEY_SHININESS, material->_lightProperties.shininess);

	aiString texture_path;
	if (assimpMaterial->GetTextureCount(aiTextureType_AMBIENT) > 0) {
		assimpMaterial->GetTexture(aiTextureType_AMBIENT, 0, &texture_path);
		material->_ambientMap = TextureFactory(texture_path.C_Str());
	}

	if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
		material->_diffuseMap = TextureFactory(texture_path.C_Str());
	}

	if (assimpMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0) {
		assimpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texture_path);
		material->_specularMap = TextureFactory(texture_path.C_Str());
	}

	return MeshFactory(vertices, indices, material);
}

void Renderer::LockCamera(bool p_lock)
{
	_cameraLock = p_lock;
}

void Renderer::AddObject(const Entity& p_object)
{
	//obtaining the GraphicsComponent of the Entity
	auto component = p_object.TryGetComponentOfType<ModelComponent>();
	if (component.expired()) {
		std::cerr << "Object does not have a Graphics Component";
		return;
	}

	_entities.push_back(component);
}

void Renderer::Set2DMode(float p_width, float p_height) 
{
	_camera.SetCameraPosition(0.f, 0.f, 1.f);
	_camera.SetCameraDirection(0.f, 0.f, -1.f);
	LockCamera(true);
	_projectionMatrix = glm::ortho(-p_width / 2, p_width / 2, 
									-p_height / 2, p_height / 2, 
									0.1f, 5.f);
	
	for (auto&& [key, shader] : _shaders) {
		shader->SetVariable(UniformVariables::projectionMatrix, _projectionMatrix);
	}
}

void Renderer::SetPerspective(float p_fov, float p_nearPlane, float p_farPlane) 
{
	float aspectRatio = static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight);
	_projectionMatrix = glm::perspective(p_fov / 2.f, aspectRatio, p_nearPlane, p_farPlane);

	for (auto&& [key, shader] : _shaders) {
		shader->SetVariable(UniformVariables::projectionMatrix, _projectionMatrix);
	}
}

void Renderer::SetLightSource(const Entity& p_object) 
{
	_lightSource = p_object.TryGetComponentOfType<LightSourceComponent>();
	if (_lightSource.expired()) {
		std::cerr << "Object does not contain a LightSourceComponent\n";
		return;
	}

	auto lightSource = _lightSource.lock();

	for (auto&& [key, shader] : _shaders) {
		lightSource->SetLightVariables(*shader);
	}
}

void Renderer::ComputeTime() 
{
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