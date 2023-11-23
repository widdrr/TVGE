module;

#include <gl/glew.h>

module Graphics.Resources:ShaderProgram;

import :Utilities;

import <glm/gtc/type_ptr.hpp>;

import <fstream>;
import <iostream>;

ShaderProgram::ShaderProgram(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath):
	_vertexShaderPath(p_vertexShaderPath),
	_fragmentShaderPath(p_fragmentShaderPath),
	_failed(false)
{	
	_id = glCreateProgram();

	if (_id == 0) {
		std::cerr << "Error creating shader program\n";
		_failed = true;
		return;
	}

	AddShader(ReadShaderFromFile(p_vertexShaderPath), GL_VERTEX_SHADER);
	AddShader(ReadShaderFromFile(p_fragmentShaderPath), GL_FRAGMENT_SHADER);

	glLinkProgram(_id);

	GLint linkSuccess;
	glGetProgramiv(_id, GL_LINK_STATUS, &linkSuccess);

	if (!linkSuccess) {
		GLchar dump[_dumpSize];
		glGetProgramInfoLog(_id, _dumpSize, nullptr, dump);
		std::cerr << "Error while linking shader program: " << dump << "\n";
		_failed = true;
		return;
	}

	glValidateProgram(_id);

	GLint validateSuccess;
	glGetProgramiv(_id, GL_VALIDATE_STATUS, &validateSuccess);

	if (!validateSuccess) {
		GLchar dump[_dumpSize];
		glGetProgramInfoLog(_id, _dumpSize, nullptr, dump);
		std::cerr << "Error while validating shader program: " << dump << "\n";
		_failed = true;
		return;
	}
	
	SetVariable(UniformVariables::Material::materialAmbientMap, TextureUnits::Ambient);
	SetVariable(UniformVariables::Material::materialDiffuseMap, TextureUnits::Diffuse);
	SetVariable(UniformVariables::Material::materialSpecularMap, TextureUnits::Specular);

}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(_id);
}

void ShaderProgram::SetVariable(std::string_view p_variableName, glm::mat4 p_value, bool p_debug) 
{	
	GLuint location = GetUniformLocation(p_variableName, p_debug);
	if (location == -1) {
		return;
	}

	glProgramUniformMatrix4fv(_id, location, 1, GL_FALSE, glm::value_ptr(p_value));
}

void ShaderProgram::SetVariable(std::string_view p_variableName, glm::vec3 p_value, bool p_debug) 
{
	GLuint location = GetUniformLocation(p_variableName, p_debug);
	if (location == -1) {
		return;
	}

	glProgramUniform3fv(_id, location, 1, glm::value_ptr(p_value));
}

void ShaderProgram::SetVariable(std::string_view p_variableName, glm::vec4 p_value, bool p_debug)
{
	GLuint location = GetUniformLocation(p_variableName, p_debug);
	if (location == -1) {
		return;
	}

	glProgramUniform4fv(_id, location, 1, glm::value_ptr(p_value));
}

void ShaderProgram::SetVariable(std::string_view p_variableName, float p_value, bool p_debug) 
{
	GLuint location = GetUniformLocation(p_variableName, p_debug);
	if (location == -1) {
		return;
	}

	glProgramUniform1f(_id, location, p_value);
}

void ShaderProgram::SetVariable(std::string_view p_variableName, int p_value, bool p_debug)
{
	GLuint location = GetUniformLocation(p_variableName, p_debug);
	if (location == -1) {
		return;
	}

	glProgramUniform1i(_id, location, p_value);
}

void ShaderProgram::AddShader(std::string p_shaderText, unsigned int p_shaderType)
{	
	GLuint shader = glCreateShader(p_shaderType);

	if (shader == 0) {
		std::cerr << "Error creating shader object \n";
		_failed = true;
		return;
	}

	const GLchar* shaderText[] = { p_shaderText.c_str() };
	GLint shaderLengths[] = { static_cast<GLint>(p_shaderText.length()) };

	glShaderSource(shader, 1, shaderText, shaderLengths);

	glCompileShader(shader);

	GLint compileSuccess;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);

	if (!compileSuccess) {
		GLchar dump[_dumpSize];
		glGetShaderInfoLog(shader, _dumpSize, NULL, dump);
		std::cerr << "Error while compiling shader: " << dump << "\n";
		_failed = true;
		return;
	}

	glAttachShader(_id, shader);

	glDeleteShader(shader);
}


std::string ShaderProgram::ReadShaderFromFile(std::string p_fileName)
{	
	std::ifstream file(p_fileName);

	if (!file.is_open()) {
		std::cerr << "Error while reading file " << p_fileName << "\n";
		//TODO: replace exits with exceptions
		exit(1);
	}

	std::string content;
	std::string line;
	while (std::getline(file, line)) {
		content += line + "\n";
	}
	return content;
}

unsigned int ShaderProgram::GetUniformLocation(std::string_view p_variableName, bool p_debug)
{
	GLuint location = glGetUniformLocation(_id, p_variableName.data());
	if (location == -1 && p_debug) {
		std::cerr << "Variable " << p_variableName << " not defined in Shader\n";
	}

	return location;
}
