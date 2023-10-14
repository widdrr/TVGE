#pragma once

#include "ShaderProgram.h"
#include "Texture.h"
#include "GraphicsComponent.h"
#include "Entity.h"
#include "Camera.h"
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
	
	const struct UniformVariables {
		static const std::string viewMatrix;
		static const std::string projectionMatrix;
		static const std::string modelMatrix;
		static const std::string hasTexture;
	};
	//TODO: figure out a nice way to modify objects 
	//later read: WHAT?
	static std::shared_ptr<Renderer> _instance;
	
	std::vector<std::shared_ptr<ShaderProgram>> _shaders;
	std::vector<std::shared_ptr<Texture>> _textures;

	std::vector<std::shared_ptr<GraphicsComponent>> _entities;
	glm::mat4 _projectionMatrix;
	//TODO: multicamera?
	Camera _camera;
	
	static void RenderCallback();
	static void CleanupCallback();
	static void KeyboardCallback(unsigned char key, int x, int y);

	Renderer();
	void RenderFunction();
	void CleanupFunction();

public:
	static std::shared_ptr<Renderer> GetInstance();
	
	//TODO use weak_ptrs
	std::shared_ptr<ShaderProgram> ShaderFactory(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath);
	std::shared_ptr<Texture> TextureFactory(const std::string& p_texturePath);
	void SetPerspective(float p_fov, float p_nearPlane, float p_farPlane);
	void AddObject(Entity& p_object);
	
	void Run();
};

