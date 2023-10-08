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
	static std::shared_ptr<Renderer> _instance;
	
	//TODO: enable the use of different shader programs for different objects
	std::shared_ptr<const ShaderProgram> _activeShaderProgram;
	std::vector<std::shared_ptr<const Object>> _objects;
	
	static void RenderCallback();
	static void CleanupCallback();

	Renderer();
	void RenderFunction();
	void CleanupFunction();

public:
	static std::shared_ptr<Renderer> GetInstance();
	void SetShaderProgram(const ShaderProgram& p_program);
	void AddObject(Object& p_object);
	
	void Run();
};

