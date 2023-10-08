#pragma once
#include <GL/glew.h>
#include <string>
class ShaderProgram
{
	GLuint _id;

	void AddShader(std::string p_shaderText, GLenum p_shaderType);
	static std::string ReadShaderFromFile(std::string p_fileName);

public:
	ShaderProgram(std::string p_vertexShaderPath, std::string p_fragmentShaderPath);
	const GLuint Id() const;
};

