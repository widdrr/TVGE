module;

#include <gl/glew.h>
#include <gl/glfw3.h>

export module Rendering;

//TODO rework header includes
import Common;
import Camera;
import Graphics.Components;
import Graphics.Resources;


import <glm/mat4x4.hpp>;

import <string>;
import <memory>;
import <vector>;

enum ShaderAttributes{
	Position,
	Color,
	TextureCoordinates,
	Normal
};

export class Renderer
{
private:
	static constexpr unsigned int _windowHeight = 600;
	static constexpr unsigned int _windowWidth = 800;

	struct GLFWwindowDeleter {

		void operator()(GLFWwindow* p_ptr);
	};
	
	const struct UniformVariables {
		static const std::string viewMatrix;
		static const std::string projectionMatrix;
		static const std::string modelMatrix;
		static const std::string hasTexture;
		static const std::string lightColor;
		static const std::string lightPosition;
		static const std::string lightAmbianceStrength;
		static const std::string lightSpecularStrength;
		static const std::string cameraPosition;
	};

	static std::shared_ptr<Renderer> _instance;
	std::unique_ptr<GLFWwindow, GLFWwindowDeleter> _window;
	
	std::vector<std::shared_ptr<ShaderProgram>> _shaders;
	std::vector<std::shared_ptr<Texture>> _textures;
	std::vector<std::shared_ptr<RenderComponent>> _entities;
	//TODO: add to camera?
	glm::mat4 _projectionMatrix;
	//TODO: multicamera?
	Camera _camera;
	//TODO multi lightsource?
	std::shared_ptr<LightSourceComponent> _lightSource;

	double _lastTime;
	float _deltaTime;
	double _fpsDelta;
	unsigned int _frames;
	double _prevX, _prevY;
	bool _focused;
	bool _initial;
	
	Renderer();

	void RenderFunction();
	void CleanupFunction();
	void ProcessInput();
	void ComputeTime();
	void MouseCallback(GLFWwindow* _window, double _crtX, double _crtY);

public:
	static std::shared_ptr<Renderer> GetInstance();
	
	//TODO use weak_ptrs
	std::shared_ptr<ShaderProgram> ShaderFactory(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath);
	std::shared_ptr<Texture> TextureFactory(const std::string& p_texturePath);
	void SetPerspective(float p_fov, float p_nearPlane, float p_farPlane);
	void SetLightSource(const Entity& p_object);
	void AddObject(const Entity& p_object);
	
	void Run();
	~Renderer();
};

