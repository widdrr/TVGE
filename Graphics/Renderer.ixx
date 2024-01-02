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

	void RenderAndDisplayScene();

	void RenderFrame();
	void RenderShadows(std::shared_ptr<LightSourceComponent> p_caster);
	void RenderFrame(ShaderProgram& p_shader);

	void DisplayScene();
	//TODO: move Input code to the Input class
	void ProcessInput();

	//TODO: move this to Window
	void InitializeTime();
	double ComputeTime();

	void SetBackgroundColor(float p_red, float p_green, float p_blue, float p_alpha = 1.f);
	void SetSkybox(const std::string& p_frontPath,
				   const std::string& p_rightPath,
				   const std::string& p_leftPath,
				   const std::string& p_topPath,
				   const std::string& p_bottomPath,
				   const std::string& p_backPath);

	void AddObject(const Entity& p_object);
	void AddLightSource(const Entity& p_object);
	void SetShadowCaster(const Entity& p_object);

	void Set2DMode(float p_width, float p_height);
	void SetPerspective(float p_fov, float p_nearPlane, float p_farPlane);

	void LoadModel(ModelComponent& p_model, const std::string& p_path, bool p_flipUVs = true);

	/*********************************************************************/

	std::shared_ptr<ShaderProgram> GenerateShader(const std::string& p_vertexShaderPath,
												  const std::string& p_fragmentShaderPath,
												  const std::string& p_geometryShaderPath = "");

	std::shared_ptr<Texture2D> GenerateTexture2D(const std::string& p_texturePath);
	std::shared_ptr<Cubemap> GenerateCubemap(const std::string& p_frontPath,
											 const std::string& p_rightPath,
											 const std::string& p_leftPath,
											 const std::string& p_topPath,
											 const std::string& p_bottomPath,
											 const std::string& p_backPath);

	std::shared_ptr<Mesh> GenerateMesh(const std::vector<Vertex>& p_vertices,
									   const std::vector<unsigned int>& p_indices,
									   const std::shared_ptr<Material>& p_material,
									   bool p_genNormal = false);

	std::shared_ptr<ShaderProgram> DefaultShader();

private:
	Renderer(GLFWwindow* p_window);

	void DrawSkybox();
	void SetShadowVariables(ShaderProgram& p_shader);
	void ProcessAssimpNode(aiNode* p_node, const aiScene* p_scene, ModelComponent& p_model);

	void LockCamera(bool p_lock);
	void MouseCallback(GLFWwindow* _window, double _crtX, double _crtY);

	/*********************************************************************/

	GLFWwindow* _window;
	std::shared_ptr<Mesh> GenerateMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<std::weak_ptr<ModelComponent>> _models;
	Entity _skybox;

	std::vector<std::weak_ptr<LightSourceComponent>> _lightSources;
	Entity _defaultLight;

	std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> _shaders;
	std::shared_ptr<ShaderProgram> _defaultShader;

	std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;

	std::weak_ptr<LightSourceComponent> _shadowCaster;
	std::shared_ptr<ShaderProgram> _shadowsShader;
	std::unique_ptr<FrameBuffer> _shadowBuffer;
	static unsigned int _shadowWidth, _shadowHeight;


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