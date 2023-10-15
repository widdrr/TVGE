#pragma once

#include "ShaderProgram.h"
#include "Texture.h"
#include "GraphicsComponent.h"
#include "Entity.h"
#include "Camera.h"

#define GLFW_DLL
#include <gl/glew.h>
#include <gl/glfw3.h>

#include <string>
#include <memory>
#include <vector>

enum ShaderAttributes{
	Position,
	Color,
	TextureCoordinates
};

class Renderer
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
	};
	//TODO: figure out a nice way to modify objects 
	//later read: WHAT?
	static std::shared_ptr<Renderer> _instance;

	std::unique_ptr<GLFWwindow, GLFWwindowDeleter> _window;
	
	std::vector<std::shared_ptr<ShaderProgram>> _shaders;
	std::vector<std::shared_ptr<Texture>> _textures;

	std::vector<std::shared_ptr<GraphicsComponent>> _entities;
	glm::mat4 _projectionMatrix;
	//TODO: multicamera?
	Camera _camera;

	double _fpsStart, _fpsEnd;
	unsigned int _frames;
	
	Renderer();
	void RenderFunction();
	void CleanupFunction();
	void ComputeFPS();

public:
	static std::shared_ptr<Renderer> GetInstance();
	
	//TODO use weak_ptrs
	std::shared_ptr<ShaderProgram> ShaderFactory(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath);
	std::shared_ptr<Texture> TextureFactory(const std::string& p_texturePath);
	void SetPerspective(float p_fov, float p_nearPlane, float p_farPlane);
	void AddObject(Entity& p_object);
	
	void Run();
};

