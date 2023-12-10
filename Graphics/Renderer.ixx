module;

#include <GL/glfw3.h>

export module Graphics:Renderer;

import Common;
import Graphics.Components;
import Graphics.Resources;

import <glm/mat4x4.hpp>;
import <assimp/scene.h>;

import <string>;
import <memory>;
import <vector>;
import <thread>;
import <unordered_map>;

export class Renderer 
{
	friend class Window;

public:
	~Renderer();

	void RenderFrame();
	//TODO: move Input code to the Input class
	void ProcessInput();

	//TODO: move this to Window
	void InitializeTime();
	double ComputeTime();

	void AddObject(const Entity& p_object);
	void AddLightSource(const Entity& p_object);

	void Set2DMode(float p_width, float p_height);
	void SetPerspective(float p_fov, float p_nearPlane, float p_farPlane);

	void LoadModel(ModelComponent& p_model, const std::string& p_path);

	/*********************************************************************/

	std::shared_ptr<ShaderProgram> GenerateShader(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath);
	std::shared_ptr<Texture> GenerateTexture(const std::string& p_texturePath);
	std::shared_ptr<Mesh> GenerateMesh(const std::vector<Vertex>& p_vertices,
									   const std::vector<unsigned int>& p_indices,
									   const std::shared_ptr<Material>& p_material,
									   bool p_genNormal = false);

	std::shared_ptr<ShaderProgram> DefaultShader();

private:
	Renderer(GLFWwindow* p_window);

	void ProcessAssimpNode(aiNode* p_node, const aiScene* p_scene, ModelComponent& p_model);
	
	void LockCamera(bool p_lock);
	void MouseCallback(GLFWwindow* _window, double _crtX, double _crtY);
	
	/*********************************************************************/

	GLFWwindow* _window;
	std::shared_ptr<Mesh> GenerateMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<std::weak_ptr<ModelComponent>> _models;

	std::vector<std::weak_ptr<LightSourceComponent>> _lightSources;
	Entity _defaultLight;

	std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> _shaders;
	std::shared_ptr<ShaderProgram> _defaultShader;
	std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;

	//TODO: add to camera?
	glm::mat4 _projectionMatrix;
	//TODO: multicamera?
	Camera _camera;

	double _lastTime;
	float _deltaTime;
	double _fpsDelta;
	unsigned int _frames;
	double _prevX, _prevY;
	bool _focused;
	bool _initial;
	bool _cameraLock;

};