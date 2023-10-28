module;

#include <gl/glew.h>
#define GLFW_DLL
#include <gl/glfw3.h>

export module Rendering;

//TODO rework header includes
import Common;
import Graphics.Components;
import Graphics.Resources;

import <glm/mat4x4.hpp>;

import <string>;
import <memory>;
import <vector>;

export class Renderer
{
private:
	static constexpr unsigned int _windowHeight = 600;
	static constexpr unsigned int _windowWidth = 800;

	struct GLFWwindowDeleter {

		void operator()(GLFWwindow* p_ptr);
	};
	
	static std::unique_ptr<Renderer> _instance;
	std::unique_ptr<GLFWwindow, GLFWwindowDeleter> _window;
	
	std::vector<std::shared_ptr<ShaderProgram>> _shaders;
	std::vector<std::shared_ptr<Texture>> _textures;
	
	std::vector<std::weak_ptr<RenderComponent>> _entities;
	//TODO: add to camera?
	glm::mat4 _projectionMatrix;
	//TODO: multicamera?
	Camera _camera;
	//TODO multi lightsource?
	std::weak_ptr<LightSourceComponent> _lightSource;

	double _lastTime;
	float _deltaTime;
	double _fpsDelta;
	unsigned int _frames;
	double _prevX, _prevY;
	bool _focused;
	bool _initial;
	bool _cameraLock;
	
	Renderer();

	void RenderFunction();
	void CleanupFunction();
	void ProcessInput();
	void ComputeTime();
	void MouseCallback(GLFWwindow* _window, double _crtX, double _crtY);

public:
	static Renderer& GetInstance();
	
	std::shared_ptr<ShaderProgram> ShaderFactory(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath);
	std::shared_ptr<Texture> TextureFactory(const std::string& p_texturePath);
	std::shared_ptr<Mesh> MeshFactory(const std::vector<Vertex>& p_vertices, const std::vector<unsigned int>& p_indices, const GLenum p_mode = GL_TRIANGLES);
	//TODO: rework so that Renderer automatically has access to all components
	void SetPerspective(float p_fov, float p_nearPlane, float p_farPlane);
	void SetLightSource(const Entity& p_object);
	//TODO: rework Cameras
	void SetCameraLock(bool p_lock);
	void AddObject(const Entity& p_object);

	void Set2DMode(float p_width, float p_height);
	
	void Run();
	~Renderer();
};

