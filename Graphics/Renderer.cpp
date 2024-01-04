module;

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <assimp/material.h>

module Graphics:Renderer;

import MeshHelpers;

import <glm/gtc/type_ptr.hpp>;
import <assimp/Importer.hpp>;
import <assimp/scene.h>;
import <assimp/postprocess.h>;

import <iostream>;
import <fstream>;

unsigned int Renderer::_shadowWidth = 2048;
unsigned int Renderer::_shadowHeight = 2048;

Renderer::Renderer(GLFWwindow* p_window) :
	_window(p_window),
	_camera()
{
	//VSync 1 = set to Refresh Rate 0 = Unbound
	glfwSwapInterval(1);
	glClearColor(0.f, 0.f, 0.f, 0.f);

	//configuring face culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//loading the default shader
	_defaultShader = GenerateShader("shader.vert", "shader.frag");
	_camera.SetCameraDirection(0, 0, -1);

	//initializing shadow framebuffer and shader
	_shadowsShader = GenerateShader("shadows.vert", "shadows.frag", "shadows.geom");

	auto shadowMap = std::shared_ptr<Cubemap>(new Cubemap(_shadowWidth, _shadowHeight, GL_DEPTH_COMPONENT));
	_shadowBuffer = FrameBufferBuilder::Init().AttachDepthCubemap(shadowMap).NoColorBuffer().Build();
}

Renderer::~Renderer()
{
	glUseProgram(0);
}

void Renderer::RenderAndDisplayScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderFrame();
	glfwSwapBuffers(_window);
}

void Renderer::RenderFrame()
{
	if (!_shadowCaster.expired()) {
		RenderShadows(_shadowCaster.lock());
	}

	auto viewMatrix = _camera.GetViewTransformation();

	for (auto&& model : _models) {

		//TODO: expired components removal;
		if (model.expired()) {
			continue;
		}
		auto&& component = model.lock();

		for (auto&& weakMesh : component->_meshes) {

			if (weakMesh.expired()) {
				continue;
			}

			auto mesh = weakMesh.lock();
			glBindVertexArray(mesh->_vao);

			//If there is a shader associated we use it
			//else fallback to the default shader
			auto& shader = mesh->_material->_shader;

			auto modelMatrix = component->GetModelTransformation();

			SetShadowVariables(shader);

			shader.SetVariable(UniformVariables::modelMatrix, modelMatrix);
			shader.SetVariable(UniformVariables::viewMatrix, viewMatrix);
			shader.SetVariable(UniformVariables::cameraPosition, _camera.GetPosition());

			//the model matrix to apply to normal vectors to correctly transform to view space
			//shader.SetVariable(UniformVariables::modelInverseTranspose, glm::mat3(glm::transpose(glm::inverse(modelMatrix))));

			unsigned int deadLights = 0;
			for (unsigned int i = 0; i < _lightSources.size(); ++i) {
				if (_lightSources[i].expired()) {
					++deadLights;
					continue;
				}
				_lightSources[i].lock()->SetLightVariables(shader, i - deadLights);
			}
			shader.SetVariable(UniformVariables::Lights::lightCount, static_cast<int>(_lightSources.size() - deadLights));

			mesh->_material->SetMaterialVariables();

			glUseProgram(shader._id);

			//TODO: IMPORTANT!! design a mechanism to store drawing logic in the object
			//to permit customizing this
			glDrawElements(GL_TRIANGLES, mesh->_indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}
	}

	DrawSkybox();
}

void Renderer::RenderShadows(std::shared_ptr<LightSourceComponent> p_caster)
{
	using namespace UniformVariables::Shadows;

	glViewport(0, 0, _shadowWidth, _shadowHeight);
	_shadowBuffer->Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

	auto lightPosition = glm::vec3(p_caster->GetPosition());

	_shadowsShader->SetVariable(shadowCasterPosition, lightPosition);
	_shadowsShader->SetVariable(shadowFarPlane, 100.f);

	auto shadowProjection = glm::perspective(glm::radians(90.0f), static_cast<float>(_shadowWidth / _shadowHeight), 1.f, 100.f);
	std::vector<glm::mat4> shadowMatrices;
	shadowMatrices.push_back(shadowProjection *
							 glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowMatrices.push_back(shadowProjection *
							 glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowMatrices.push_back(shadowProjection *
							 glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowMatrices.push_back(shadowProjection *
							 glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowMatrices.push_back(shadowProjection *
							 glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowMatrices.push_back(shadowProjection *
							 glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	for (int i = 0; i < 6; ++i) {
		_shadowsShader->SetVariable(UniformVariables::InsertArrayIndex(shadowMatricesArray, i), shadowMatrices[i]);
	}

	glUseProgram(_shadowsShader->_id);

	for (auto&& model : _models) {

		if (model.expired()) {
			continue;
		}
		auto&& component = model.lock();

		for (auto&& weakMesh : component->_meshes) {

			if (weakMesh.expired()) {
				continue;
			}

			auto mesh = weakMesh.lock();
			glBindVertexArray(mesh->_vao);

			_shadowsShader->SetVariable(UniformVariables::modelMatrix, component->GetModelTransformation());

			glDrawElements(GL_TRIANGLES, mesh->_indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);

	int width, height;
	glfwGetWindowSize(_window, &width, &height);
	glViewport(0, 0, width, height);
}

void Renderer::RenderFrame(ShaderProgram& p_shader)
{
	//TODO: fix this one
	glUseProgram(p_shader._id);
	p_shader.SetVariable(UniformVariables::viewMatrix, _camera.GetViewTransformation());
	p_shader.SetVariable(UniformVariables::cameraPosition, _camera.GetPosition());

	unsigned int deadLights = 0;
	for (unsigned int i = 0; i < _lightSources.size(); ++i) {
		if (_lightSources[i].expired()) {
			++deadLights;
			continue;
		}
		_lightSources[i].lock()->SetLightVariables(p_shader, i - deadLights);
	}

	SetShadowVariables(p_shader);

	p_shader.SetVariable(UniformVariables::Lights::lightCount, static_cast<int>(_lightSources.size() - deadLights));

	for (auto&& model : _models) {

		//TODO: expired components removal;
		if (model.expired()) {
			continue;
		}
		auto&& component = model.lock();

		for (auto&& weakMesh : component->_meshes) {

			if (weakMesh.expired()) {
				continue;
			}

			auto mesh = weakMesh.lock();
			glBindVertexArray(mesh->_vao);

			p_shader.SetVariable(UniformVariables::modelMatrix, component->GetModelTransformation());

			//TODO: IMPORTANT!! design a mechanism to store drawing logic in the object
			//to permit customizing this
			glDrawElements(GL_TRIANGLES, mesh->_indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

		}
	}
	glUseProgram(0);
}

void Renderer::DisplayScene()
{
	glfwSwapBuffers(_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::shared_ptr<ShaderProgram> Renderer::GenerateShader(const std::string& p_vertexShaderPath,
														const std::string& p_fragmentShaderPath,
														const std::string& p_geometryShaderPath)
{
	const std::string concatPath = p_vertexShaderPath + p_fragmentShaderPath + p_geometryShaderPath;
	if (_shaders.contains(concatPath)) {
		return _shaders[concatPath];
	}

	auto thisShader = std::shared_ptr<ShaderProgram>(new ShaderProgram(p_vertexShaderPath, p_fragmentShaderPath, p_geometryShaderPath));

	if (thisShader->_failed) {
		std::cerr << "Shader Program creation failed, nullptr is returned\n";
		return nullptr;
	}

	_shaders[concatPath] = thisShader;
	thisShader->SetVariable(UniformVariables::projectionMatrix, _projectionMatrix);

	return thisShader;
}

std::shared_ptr<Texture2D> Renderer::GenerateTexture2D(const std::string& p_texturePath)
{
	if (_textures.contains(p_texturePath)) {
		return dynamic_pointer_cast<Texture2D>(_textures[p_texturePath]);
	}

	auto thisTexture = std::shared_ptr<Texture2D>(new Texture2D(p_texturePath));

	if (thisTexture->_textureData == nullptr) {
		std::cerr << "Texture creation failed, nullptr is returned\n";
		return nullptr;
	}
	_textures[p_texturePath] = thisTexture;

	return thisTexture;
}

std::shared_ptr<Cubemap> Renderer::GenerateCubemap(const std::string& p_frontPath, const std::string& p_rightPath, const std::string& p_leftPath, const std::string& p_topPath, const std::string& p_bottomPath, const std::string& p_backPath)
{
	const auto& concatPath = p_frontPath + p_rightPath + p_leftPath + p_topPath + p_bottomPath + p_backPath;

	if (_textures.contains(concatPath)) {
		return dynamic_pointer_cast<Cubemap>(_textures[concatPath]);
	}

	auto thisTexture = std::shared_ptr<Cubemap>(new Cubemap(p_frontPath,
															p_rightPath,
															p_leftPath,
															p_topPath,
															p_bottomPath,
															p_backPath));

	if (thisTexture->_textureData == nullptr) {
		std::cerr << "Texture creation failed, nullptr is returned\n";
		return nullptr;
	}
	_textures[concatPath] = thisTexture;

	return thisTexture;
}

std::shared_ptr<Mesh> Renderer::GenerateMesh(const std::string& p_name,
											 const std::vector<Vertex>& p_vertices,
											 const std::vector<unsigned int>& p_indices, 
											 const std::shared_ptr<Material>& p_material,
											 bool p_genNormal)
{
	if (_meshes.contains(p_name)) {
		return _meshes[p_name];
	}

	_meshes[p_name] = std::shared_ptr<Mesh>(new Mesh(p_vertices, p_indices, p_material, p_genNormal));

	return _meshes[p_name];
}

void Renderer::LoadModel(ModelComponent& p_model, const std::string& p_path, const bool p_flipUVs)
{
	Assimp::Importer importer;
	//TODO: look into preprocess options
	unsigned int flags = aiProcess_Triangulate | aiProcess_GenNormals;
	if (p_flipUVs) {
		flags = flags | aiProcess_FlipUVs;
	}
	const aiScene* scene = importer.ReadFile(p_path, flags);

	if (scene == nullptr) {
		std::cerr << "Error loading model: " << importer.GetErrorString() << "\n";
		return;
	}

	if (scene->mRootNode == nullptr) {
		std::cerr << "Assimp Scene is empty\n";
		return;
	}

	ProcessAssimpNode(scene->mRootNode, scene, p_path, p_model);
}

std::shared_ptr<ShaderProgram> Renderer::DefaultShader()
{
	return _defaultShader;
}

void Renderer::DrawSkybox()
{
	auto skyboxComp = _skybox.TryGetComponentOfType<SkyboxComponent>();
	if (!skyboxComp.expired()) {
		glCullFace(GL_FRONT);

		auto&& skybox = skyboxComp.lock();

		glBindVertexArray(skybox->mesh->_vao);
		glUseProgram(skybox->shader._id);

		auto viewMatrix = glm::mat4(glm::mat3(_camera.GetViewTransformation()));
		skybox->shader.SetVariable(UniformVariables::viewMatrix, viewMatrix);

		glDrawElements(GL_TRIANGLES, skybox->mesh->_indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);
		glCullFace(GL_BACK);
	}
}

void Renderer::SetShadowVariables(ShaderProgram& p_shader)
{
	p_shader.SetVariable(UniformVariables::Shadows::hasShadows, false);
	if (!_shadowCaster.expired()) {
		using namespace UniformVariables::Shadows;
		auto _shadow = _shadowCaster.lock();
		p_shader.SetVariable(hasShadows, true);
		p_shader.SetVariable(shadowCasterPosition, glm::vec3(_shadow->GetPosition()));
		p_shader.SetVariable(shadowFarPlane, 100.f);

		_shadowBuffer->_depthTexture->Bind(TextureUnits::Shadow, GL_TEXTURE_CUBE_MAP);
	}
}

void Renderer::ProcessAssimpNode(aiNode* p_node, const aiScene* p_scene, const std::string& p_path, ModelComponent& p_model)
{
	for (unsigned int i = 0; i < p_node->mNumMeshes; ++i) {
		auto mesh = p_scene->mMeshes[p_node->mMeshes[i]];
		const std::string meshName = std::format("{}/{}/{}", p_path, p_node->mName.C_Str(), i);
		p_model._meshes.push_back(GenerateMesh(mesh, p_scene, meshName));
	}

	for (unsigned int i = 0; i < p_node->mNumChildren; ++i) {
		ProcessAssimpNode(p_node->mChildren[i], p_scene, p_path, p_model);
	}
}

std::shared_ptr<Mesh> Renderer::GenerateMesh(aiMesh* p_mesh, const aiScene* p_scene, const std::string& p_name)
{
	if (_meshes.contains(p_name)) {
		return _meshes[p_name];
	}

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

	assimpMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, output);
	material->_lightProperties.emissive = glm::vec3(output.r, output.g, output.b);

	aiString texture_path;
	if (assimpMaterial->GetTextureCount(aiTextureType_AMBIENT) > 0) {
		assimpMaterial->GetTexture(aiTextureType_AMBIENT, 0, &texture_path);
		material->_ambientMap = GenerateTexture2D(texture_path.C_Str());
	}

	if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
		material->_diffuseMap = GenerateTexture2D(texture_path.C_Str());
	}

	if (assimpMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0) {
		assimpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texture_path);
		material->_specularMap = GenerateTexture2D(texture_path.C_Str());
	}

	return GenerateMesh(p_name, vertices, indices, material);
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

	_models.push_back(component);
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
	int windowWidth, windowHeight;
	glfwGetWindowSize(_window, &windowWidth, &windowHeight);

	float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	_projectionMatrix = glm::perspective(p_fov / 2.f, aspectRatio, p_nearPlane, p_farPlane);

	for (auto&& [key, shader] : _shaders) {
		shader->SetVariable(UniformVariables::projectionMatrix, _projectionMatrix);
	}
}

void Renderer::AddLightSource(const Entity& p_object)
{
	auto lightSource = p_object.TryGetComponentOfType<LightSourceComponent>();
	if (lightSource.expired()) {
		std::cerr << "Object does not contain a LightSourceComponent\n";
		return;
	}

	_lightSources.push_back(lightSource);
}

void Renderer::SetShadowCaster(const Entity& p_object)
{
	auto lightSource = p_object.TryGetComponentOfType<LightSourceComponent>();
	if (lightSource.expired()) {
		std::cerr << "Object does not contain a LightSourceComponent\n";
		return;
	}

	_shadowCaster = lightSource;
}

void Renderer::InitializeTime()
{
	_lastTime = glfwGetTime();
	_frames = 0;
}

double Renderer::ComputeTime()
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

	return delta;
}

void Renderer::SetBackgroundColor(float p_red, float p_green, float p_blue, float p_alpha)
{
	glClearColor(p_red, p_green, p_blue, p_alpha);
}

void Renderer::SetSkybox(const std::string& p_frontPath,
						 const std::string& p_rightPath,
						 const std::string& p_leftPath,
						 const std::string& p_topPath,
						 const std::string& p_bottomPath,
						 const std::string& p_backPath)
{
	auto skyboxComp = _skybox.TryGetComponentOfType<SkyboxComponent>();
	if (skyboxComp.expired()) {
		auto&& shader = *GenerateShader("skybox.vert", "skybox.frag");
		skyboxComp = _skybox.CreateComponentOfType<SkyboxComponent>(std::ref(shader));
		auto&& [vertices, indices] = CommonMeshes::Cube();

		skyboxComp.lock()->mesh = GenerateMesh("skybox", vertices, indices, nullptr);
	}

	skyboxComp.lock()->texture = GenerateCubemap(p_frontPath, p_rightPath, p_leftPath, p_topPath, p_bottomPath, p_backPath);
	skyboxComp.lock()->texture->Bind(TextureUnits::Skybox);
}

void Renderer::ProcessInput()
{

	glfwPollEvents();

	if (_cameraLock) {
		return;
	}

	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		_focused = false;
	}

	if (!_focused && glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		_focused = true;
		_initial = true;
	}

	bool moveForward = (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS);
	bool moveBackward = (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS);
	bool moveLeft = (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS);
	bool moveRight = (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS);
	bool moveUp = (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS);
	bool moveDown = (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);

	_camera.MoveCamera({ moveForward, moveBackward, moveLeft, moveRight, moveUp, moveDown }, _deltaTime);
}

void Renderer::MouseCallback(GLFWwindow* _window, double _crtX, double _crtY)
{

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
			float offsetY = static_cast<float>(_prevY - _crtY);
			_prevX = _crtX;
			_prevY = _crtY;

			_camera.RotateCamera(offsetX, offsetY);
		}
	}
}