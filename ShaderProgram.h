#pragma once
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <string>
class ShaderProgram
{
private:
	static constexpr unsigned int _dumpSize = 1024;
	//Shaders should be instantiated by the Renderer
	friend class Renderer;

	GLuint _id;

	ShaderProgram() = default;
	ShaderProgram(std::string p_vertexShaderPath, std::string p_fragmentShaderPath);

	void AddShader(std::string p_shaderText, GLenum p_shaderType);
	static std::string ReadShaderFromFile(std::string p_fileName);

public:

	void SetVariable(std::string p_variableName, glm::mat4 p_value);
	void SetVariable(std::string p_variableName, float p_value);
};

