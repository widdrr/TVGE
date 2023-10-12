#pragma once

#include "ShaderProgram.h"
#include "GraphicsComponent.h"
#include "Entity.h"
#include <string>
#include <memory>
#include <vector>

class Renderer
{
private:
	static constexpr unsigned int _windowHeight = 600;
	static constexpr unsigned int _windowWidth = 800;
	//TODO: figure out a nice way to modify objects 
	//later read: WHAT?
	static std::shared_ptr<Renderer> _instance;
	
	std::vector<std::shared_ptr<ShaderProgram>> _shaders;

	std::vector<std::shared_ptr<GraphicsComponent>> _entities;
	glm::mat4 _projectionMatrix;
	
	static void RenderCallback();
	static void CleanupCallback();

	Renderer();
	void RenderFunction();
	void CleanupFunction();

public:
	static std::shared_ptr<Renderer> GetInstance();
	
	std::shared_ptr<ShaderProgram> ShaderFactory(std::string p_vertexShaderPath, std::string p_fragmentShaderPath);
	void SetPerspective(float p_fov, float p_nearPlane, float p_farPlane);
	void AddObject(Entity& p_object);
	
	void Run();
};

