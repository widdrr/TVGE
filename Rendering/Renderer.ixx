module;

#include <gl/glew.h>
#define GLFW_DLL
#include <gl/glfw3.h>

export module Rendering;

import Common;
import Graphics.Components;
import Graphics.Resources;

import <glm/mat4x4.hpp>;
import <assimp/scene.h>;

import <string>;
import <memory>;
import <vector>;
import <unordered_map>;

export class Renderer
{
	//Types and Constants
private:
	static constexpr unsigned int _windowHeight = 600;
	static constexpr unsigned int _windowWidth = 800;

	struct GLFWwindowDeleter {

		void operator()(GLFWwindow* p_ptr);
	};

    //Constructors and Destructors
	Renderer();
public:
	static Renderer& GetInstance();
	~Renderer();

	//Methods
	//TODO: store default shaders in source
	std::shared_ptr<ShaderProgram> ShaderFactory(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath);
	std::shared_ptr<Texture> TextureFactory(const std::string& p_texturePath);
	std::shared_ptr<Mesh> MeshFactory(const std::vector<Vertex>& p_vertices, 
									  const std::vector<unsigned int>& p_indices, 
									  const std::shared_ptr<Material>& p_material,
									  bool p_genNormal = false);

	void LoadModel(ModelComponent& p_model, const std::string& p_path);
	std::shared_ptr<ShaderProgram> DefaultShader();
	
	//TODO: rework so that Renderer automatically has access to all components
	void SetPerspective(float p_fov, float p_nearPlane, float p_farPlane);
	void SetLightSource(const Entity& p_object);
	
	//TODO: rework Cameras
	void LockCamera(bool p_lock);
	void AddObject(const Entity& p_object);

	void Set2DMode(float p_width, float p_height);

	void Run();

private:
	void RenderFunction();
	void CleanupFunction();
	void ProcessInput();
	void ComputeTime();
	void MouseCallback(GLFWwindow* _window, double _crtX, double _crtY);

	void ProcessAssimpNode(aiNode* p_node, const aiScene* p_scene, ModelComponent& p_model);
	std::shared_ptr<Mesh> MeshFactory(aiMesh* mesh, const aiScene* scene);

	//Members
	static std::unique_ptr<Renderer> _instance;
	std::unique_ptr<GLFWwindow, GLFWwindowDeleter> _window;
	
	std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> _shaders;
	std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
	
	std::vector<std::weak_ptr<ModelComponent>> _entities;
	//TODO: add to camera?
	glm::mat4 _projectionMatrix;
	//TODO: multicamera?
	Camera _camera;
	//TODO multi lightsource?
	std::weak_ptr<LightSourceComponent> _lightSource;
	Entity _defaultLight;
	std::shared_ptr<ShaderProgram> _defaultShader;

	double _lastTime;
	float _deltaTime;
	double _fpsDelta;
	unsigned int _frames;
	double _prevX, _prevY;
	bool _focused;
	bool _initial;
	bool _cameraLock;
};