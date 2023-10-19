export module Graphics.Resources:ShaderProgram;

import <GL/glew.h>;
import <glm/mat4x4.hpp>;

import <string>;

export class ShaderProgram
{
private:
	static constexpr unsigned int _dumpSize = 1024;
	//Shaders should be instantiated by the Renderer
	friend class Renderer;

	GLuint _id;

	ShaderProgram(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath);

	void AddShader(std::string p_shaderText, GLenum p_shaderType);
	static std::string ReadShaderFromFile(std::string p_fileName);

public:

	void SetVariable(std::string p_variableName, glm::mat4 p_value, bool p_debug = false);
	void SetVariable(std::string p_variableName, float p_value, bool p_debug = false);
	void SetVariable(std::string p_variableName, glm::vec3 p_value, bool p_debug = false);
};

