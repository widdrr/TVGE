#pragma once

#include "ShaderProgram.h"
#include "Object.h"
#include <string>
#include <memory>
#include <vector>

class Renderer
{
private:
	static constexpr unsigned int _windowHeight = 600;
	static constexpr unsigned int _windowWidth = 800;
	//TODO: figure out a nice way to modify objects 
	static std::shared_ptr<Renderer> _instance;
	
	//TODO: enable the use of different shader programs for different objects
	std::shared_ptr<const ShaderProgram> _activeShaderProgram;
	//TODO: figure a way to used shared pointers
	std::vector<Object*> _objects;
	GLuint _glModelMatrixLocation;
	GLuint _glProjectionMatrixLocation;
	glm::mat4 _projectionMatrix;
	
	static void RenderCallback();
	static void CleanupCallback();

	Renderer();
	void RenderFunction();
	void CleanupFunction();

public:
	static std::shared_ptr<Renderer> GetInstance();
	void SetShaderProgram(const ShaderProgram& p_program);
	void SetPerspective(float p_fov, float p_nearPlane, float p_farPlane);
	void AddObject(Object& p_object);
	
	void Run();
};

