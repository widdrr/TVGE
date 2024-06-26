module;

#include <GL/glfw3.h>
#include <assimp/scene.h>

export module Graphics:Renderer;

import Common;
import Graphics.Resources;
import Graphics.Components;

import <glm/mat4x4.hpp>;
import <glm/gtc/quaternion.hpp>;

import <string>;
import <memory>;
import <vector>;
import <unordered_map>;

namespace TVGE
{
	class Window;
}

namespace TVGE::Graphics
{
	export class Renderer
	{
		friend class TVGE::Window;

	public:
		~Renderer();

		void RenderAndDisplayScene();

		void RenderScene();
		void RenderShadows(std::shared_ptr<LightSourceComponent> p_caster);
		void RenderScene(ShaderProgram& p_shader);
		void RenderWireframe();
		void DrawRayBetweenPoints(glm::vec3 p_start, glm::vec3 p_end, glm::vec3 p_color);
		void DrawRayAtPosition(glm::vec3 p_position, glm::vec3 p_ray, glm::vec3 p_color);
		void DrawBox(glm::vec3 p_color,
					 glm::vec3 p_position = glm::vec3(0),
					 glm::vec3 p_scaling = glm::vec3(1),
					 glm::quat p_rotation = glm::identity<glm::quat>());

		void DrawBoxFromExtents(glm::vec3 p_color,
								glm::vec3 p_min,
								glm::vec3 p_max);

		void DisplayScene();

		CameraComponent& GetMainCamera();

		void SetBackgroundColor(float p_red, float p_green, float p_blue, float p_alpha = 1.f);
		void SetSkybox(const std::string& p_frontPath,
					   const std::string& p_rightPath,
					   const std::string& p_leftPath,
					   const std::string& p_topPath,
					   const std::string& p_bottomPath,
					   const std::string& p_backPath);

		void AddObject(const Entity& p_object);
		void SetShadowSource(const Entity& p_object,
							 float p_shadowFarPlane = 100.f,
							 float p_directionalShadowHeight = 50.f,
							 float p_directionalShadowSize = 200.f,
							 float p_shadowNearPlane = 1.f);

		void Set2DMode(float p_width, float p_height);
		void SetPerspective(float p_fov, float p_nearPlane, float p_farPlane);

		void CleanDanglingPointers();
		void SetVSync(bool p_value);

		void LoadModel(ModelComponent& p_model, const std::string& p_path, bool p_flipUVs = true);

		/*********************************************************************/

		std::shared_ptr<ShaderProgram> GenerateShaderFromFiles(const std::string& p_vertexShaderPath,
															   const std::string& p_fragmentShaderPath,
															   const std::string& p_geometryShaderPath = "");
		std::shared_ptr<ShaderProgram> GenerateShaderFromText(const std::string& p_vertexShaderText,
															  const std::string& p_fragmentShaderText,
															  const std::string& p_geometryShaderText = "");

		std::shared_ptr<Texture2D> GenerateTexture2D(const std::string& p_texturePath, const bool p_repeat = true);

		std::shared_ptr<Cubemap> GenerateCubemap(const std::string& p_frontPath,
												 const std::string& p_rightPath,
												 const std::string& p_leftPath,
												 const std::string& p_topPath,
												 const std::string& p_bottomPath,
												 const std::string& p_backPath);

		std::shared_ptr<Mesh> GenerateMesh(const std::string& p_name,
										   const std::vector<Vertex>& p_vertices = {},
										   const std::vector<unsigned int>& p_indices = {},
										   const std::shared_ptr<Material>& p_material = nullptr,
										   bool p_genNormal = false);

		std::shared_ptr<ShaderProgram> DefaultShader();

	private:
		Renderer(GLFWwindow* p_window);

		void GenerateRayPrimitive();
		void GenerateBoxPrimitive();

		void DrawSkybox();
		void SetShadowVariables(ShaderProgram& p_shader);
		void ProcessAssimpNode(aiNode* p_node, const aiScene* p_scene, const std::string& p_path, ModelComponent& p_model);

		void LockCamera(bool p_lock);

		/*********************************************************************/

		GLFWwindow* _window;
		std::shared_ptr<Mesh> GenerateMesh(aiMesh* mesh, const aiScene* scene, const std::string& p_name);

		std::vector<std::weak_ptr<ModelComponent>> _models;
		Entity _skybox;

		std::vector<std::weak_ptr<LightSourceComponent>> _lightSources;
		Entity _defaultLight;

		std::unordered_map<unsigned int, std::shared_ptr<ShaderProgram>> _shaders;
		std::shared_ptr<ShaderProgram> _defaultShader;
		std::shared_ptr<ShaderProgram> _wireframeShader;

		std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
		std::unordered_map<std::string, std::shared_ptr<Mesh>> _meshes;

		std::weak_ptr<LightSourceComponent> _shadowSource;
		std::shared_ptr<ShaderProgram> _pointShadowsShader;
		std::shared_ptr<ShaderProgram> _directionalShadowsShader;
		std::shared_ptr<Cubemap> _pointShadowMap;
		std::shared_ptr<Texture2D> _directionalShadowMap;
		std::unique_ptr<FrameBuffer> _shadowBuffer;
		glm::mat4 _shadowLightMatrix;
		float _shadowNearPlane, _shadowFarPlane, _directionalShadowHeight, _directionalShadowSize;

		static unsigned int _shadowWidth, _shadowHeight;
		unsigned int _rayVao, _rayVbo;
		unsigned int _boxVao, _boxVbo, _boxEbo;

		//TODO: add to camera?
		glm::mat4 _projectionMatrix;
		//TODO: multicamera?
		Entity _cameraEntity;
		std::shared_ptr<CameraComponent> _mainCamera;

		double _lastTime;
		float _deltaTime;
		double _fpsDelta;
		unsigned int _frames;
		bool _cameraLock;
	};
}