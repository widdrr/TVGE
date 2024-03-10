module;

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <assimp/material.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

module Graphics:Renderer;

import MeshHelpers;
import :Shaders;

import <glm/gtc/type_ptr.hpp>;
import <glm/gtx/matrix_cross_product.hpp>;

import <iostream>;
import <fstream>;

unsigned int Renderer::_shadowWidth = 2048;
unsigned int Renderer::_shadowHeight = 2048;

Renderer::Renderer(GLFWwindow* p_window) :
	_window(p_window),
	_mainCamera()
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
	_defaultShader = GenerateShaderFromText(ShaderSources::defaultVertex.data(),
											ShaderSources::defaultFragment.data());

	_wireframeShader = GenerateShaderFromText(ShaderSources::wireframeVertex.data(),
											  ShaderSources::wireframeFragment.data());

	_mainCamera.SetCameraDirection(0, 0, -1);

	//initializing shadow framebuffer and shader
	_pointShadowsShader = GenerateShaderFromText(ShaderSources::pointShadowVertex.data(),
												 ShaderSources::pointShadowFragment.data(),
												 ShaderSources::pointShadowGeometry.data());
	_pointShadowMap = std::shared_ptr<Cubemap>(new Cubemap(_shadowWidth, _shadowHeight, GL_DEPTH_COMPONENT));

	_directionalShadowsShader = GenerateShaderFromText(ShaderSources::directionalShadowVertex.data(),
													   ShaderSources::directionalShadowFragment.data());

	_directionalShadowMap = std::shared_ptr<Texture2D>(new Texture2D(_shadowWidth, _shadowHeight, GL_DEPTH_COMPONENT));
	_shadowBuffer = FrameBufferBuilder::Init().AttachDepthCubemap(_pointShadowMap).NoColorBuffer().Build();

	//creating the one true ray, all other rays are instances of this
	glGenVertexArrays(1, &_rayVao);
	glGenBuffers(1, &_rayVbo);

	glBindVertexArray(_rayVao);
	glBindBuffer(GL_ARRAY_BUFFER, _rayVbo);

	std::vector<float> vertices = { 0.f, 0.f, 0.f, 1.f, 0.f, 0.f };
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), reinterpret_cast<void*>(vertices.data()), GL_STATIC_DRAW);

	glVertexAttribPointer(VertexAttributes::Position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(VertexAttributes::Position);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Renderer::~Renderer()
{
	glUseProgram(0);

	//Cleaning up ray primitive
	glBindVertexArray(_rayVao);

	glDisableVertexAttribArray(VertexAttributes::Position);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &_rayVbo);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &_rayVao);
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

	auto viewMatrix = _mainCamera.GetViewTransformation();

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
			shader.SetVariable(UniformVariables::cameraPosition, _mainCamera.GetPosition());
			shader.SetVariable(UniformVariables::lightMatrix, _shadowLightMatrix);

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
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->_indices.size()), GL_UNSIGNED_INT, 0);
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

	float farPlane = 100.f;
	auto position = p_caster->GetPosition();
	std::shared_ptr<ShaderProgram> shadowShader;
	std::vector<glm::mat4> shadowMatrices;

	//Point shadow, use omnidirectional shadow mapping
	if (position.w == 1) {
		auto lightPosition = glm::vec3(position);
		shadowShader = _pointShadowsShader;
		_shadowBuffer->SetDepthCubemap(_pointShadowMap);
		_pointShadowsShader->SetVariable(shadowCasterPosition, lightPosition);

		auto shadowProjection = glm::perspective(glm::radians(90.0f), static_cast<float>(_shadowWidth / _shadowHeight), 1.f, farPlane);

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
	}
	//Directional light, fall back to simpler shadow mapping
	else {
		auto lightPosition = glm::vec3(position) * farPlane * 0.75f;
		shadowShader = _directionalShadowsShader;
		_shadowBuffer->SetDepthTexture2D(_directionalShadowMap);
		_directionalShadowsShader->SetVariable(shadowCasterPosition, lightPosition);

		auto shadowProjection = glm::ortho(-farPlane / 2.f, farPlane / 2.f, farPlane / 2.f, -farPlane / 2.f, 1.f, farPlane);
		auto upVector = glm::vec3(0.f, 1.f, 0.f);
		if (glm::epsilonEqual(glm::abs(glm::dot(upVector, glm::normalize(lightPosition))), 1.f, EPSILON)) {
			upVector = glm::normalize(upVector + glm::vec3(0.1f, 0.f, 0.1f));
		}

		_shadowLightMatrix = shadowProjection * glm::lookAt(lightPosition, glm::vec3(0.f, 0.f, 0.f), upVector);
		shadowMatrices.push_back(_shadowLightMatrix);
	}

	shadowShader->SetVariable(shadowFarPlane, farPlane);
	for (int i = 0; i < shadowMatrices.size(); ++i) {
		shadowShader->SetVariable(UniformVariables::InsertArrayIndex(shadowMatricesArray, i), shadowMatrices[i]);
	}

	_shadowBuffer->Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	glUseProgram(shadowShader->_id);

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

			shadowShader->SetVariable(UniformVariables::modelMatrix, component->GetModelTransformation());

			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->_indices.size()), GL_UNSIGNED_INT, 0);
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
	p_shader.SetVariable(UniformVariables::viewMatrix, _mainCamera.GetViewTransformation());
	p_shader.SetVariable(UniformVariables::cameraPosition, _mainCamera.GetPosition());

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
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->_indices.size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

		}
	}
	glUseProgram(0);
}

void Renderer::RenderWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	_wireframeShader->SetVariable(UniformVariables::color, glm::vec3(1.f, 0.f, 0.f));
	RenderFrame(*_wireframeShader);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::DrawRayBetweenPoints(glm::vec3 p_start, glm::vec3 p_end, glm::vec3 p_color)
{
	DrawRayAtPosition(p_start, p_end - p_start, p_color);
}

void Renderer::DrawRayAtPosition(glm::vec3 p_position, glm::vec3 p_ray, glm::vec3 p_color)
{
	float length = glm::length(p_ray);
	glm::vec3 ray = glm::normalize(p_ray);

	auto modelMatrix = glm::translate(glm::identity<glm::mat4>(), p_position);

	float cosine = glm::dot(ray, glm::vec3(1.f, 0.f, 0.f));


	if (glm::epsilonNotEqual(cosine, -1.f, EPSILON)) {

		float factor = 1.f / (1.f + cosine);

		glm::vec3 cross = glm::cross(glm::vec3(1.f, 0.f, 0.f), ray);
		glm::mat4 crossProductMatrix = glm::matrixCross4(cross);
		glm::mat4 rotation = glm::identity<glm::mat4>() + crossProductMatrix + crossProductMatrix * crossProductMatrix * factor;

		modelMatrix = modelMatrix * rotation;
	}
	else {
		length *= -1.f;
	}

	modelMatrix = glm::scale(modelMatrix, glm::vec3(length));

	glUseProgram(_wireframeShader->_id);
	_wireframeShader->SetVariable(UniformVariables::color, p_color);
	_wireframeShader->SetVariable(UniformVariables::viewMatrix, _mainCamera.GetViewTransformation());
	_wireframeShader->SetVariable(UniformVariables::modelMatrix, modelMatrix);

	glBindVertexArray(_rayVao);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);

	glUseProgram(0);

}

void Renderer::DisplayScene()
{
	glfwSwapBuffers(_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Camera& Renderer::GetMainCamera()
{
	return _mainCamera;
}

std::shared_ptr<ShaderProgram> Renderer::GenerateShaderFromFiles(const std::string& p_vertexShaderPath,
																 const std::string& p_fragmentShaderPath,
																 const std::string& p_geometryShaderPath)
{
	const unsigned int sourceId = ShaderProgram::GenerateProgramSourceId(p_vertexShaderPath, p_fragmentShaderPath, p_geometryShaderPath);
	if (_shaders.contains(sourceId)) {
		return _shaders[sourceId];
	}

	auto thisShader = std::shared_ptr<ShaderProgram>(new ShaderProgram(p_vertexShaderPath, p_fragmentShaderPath, p_geometryShaderPath));

	if (thisShader->_failed) {
		std::cerr << "Shader Program creation failed, nullptr is returned\n";
		return nullptr;
	}

	_shaders[sourceId] = thisShader;
	thisShader->SetVariable(UniformVariables::projectionMatrix, _projectionMatrix);

	return thisShader;
}

std::shared_ptr<ShaderProgram> Renderer::GenerateShaderFromText(const std::string& p_vertexShaderText, const std::string& p_fragmentShaderText, const std::string& p_geometryShaderText)
{
	const unsigned int sourceId = ShaderProgram::GenerateProgramSourceId(p_vertexShaderText, p_fragmentShaderText, p_geometryShaderText);
	if (_shaders.contains(sourceId)) {
		return _shaders[sourceId];
	}

	auto thisShader = std::shared_ptr<ShaderProgram>(new ShaderProgram(p_vertexShaderText, p_fragmentShaderText, p_geometryShaderText, false));

	if (thisShader->_failed) {
		std::cerr << "Shader Program creation failed, nullptr is returned\n";
		return nullptr;
	}

	_shaders[sourceId] = thisShader;
	thisShader->SetVariable(UniformVariables::projectionMatrix, _projectionMatrix);

	return thisShader;
}

std::shared_ptr<Texture2D> Renderer::GenerateTexture2D(const std::string& p_texturePath, const bool p_repeat)
{
	if (_textures.contains(p_texturePath)) {
		return dynamic_pointer_cast<Texture2D>(_textures[p_texturePath]);
	}

	auto thisTexture = std::shared_ptr<Texture2D>(new Texture2D(p_texturePath, p_repeat));

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

		auto viewMatrix = glm::mat4(glm::mat3(_mainCamera.GetViewTransformation()));
		skybox->shader.SetVariable(UniformVariables::viewMatrix, viewMatrix);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(skybox->mesh->_indices.size()), GL_UNSIGNED_INT, 0);

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

		glm::vec4 position = _shadow->GetPosition();
		p_shader.SetVariable(hasShadows, true);
		p_shader.SetVariable(shadowFarPlane, 100.f);

		if (position.w == 1.f) {
			p_shader.SetVariable(shadowCasterPosition, position);
			_shadowBuffer->_depthTexture->Bind(TextureUnits::Shadow, GL_TEXTURE_CUBE_MAP);

		}
		else {
			p_shader.SetVariable(shadowCasterPosition, position);
			_shadowBuffer->_depthTexture->Bind(TextureUnits::Shadow, GL_TEXTURE_2D);
		}
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
	_mainCamera.SetCameraPosition(0.f, 0.f, 1.f);
	_mainCamera.SetCameraDirection(0.f, 0.f, -1.f);
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
		auto&& shader = *GenerateShaderFromFiles("skybox.vert", "skybox.frag");
		skyboxComp = _skybox.CreateComponentOfType<SkyboxComponent>(std::ref(shader));
		auto&& [vertices, indices] = CommonMeshes::Cube();

		skyboxComp.lock()->mesh = GenerateMesh("skybox", vertices, indices, nullptr);
	}

	skyboxComp.lock()->texture = GenerateCubemap(p_frontPath, p_rightPath, p_leftPath, p_topPath, p_bottomPath, p_backPath);
	skyboxComp.lock()->texture->Bind(TextureUnits::Skybox);
}