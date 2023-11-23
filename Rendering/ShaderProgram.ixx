export module Graphics.Resources:ShaderProgram;

import <glm/mat4x4.hpp>;
import <string>;

export class ShaderProgram
{
	//Shaders should be instantiated by the Renderer
	friend class Renderer;

private:
	static constexpr unsigned int _dumpSize = 1024;

	ShaderProgram(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath);
public:
	~ShaderProgram();

private:
	void AddShader(std::string p_shaderText, unsigned int p_shaderType);
	static std::string ReadShaderFromFile(std::string p_fileName);
	unsigned int GetUniformLocation(std::string_view p_variableName, bool p_debug);
public:

	void SetVariable(std::string_view p_variableName, glm::mat4 p_value, bool p_debug = false);
	void SetVariable(std::string_view p_variableName, float p_value, bool p_debug = false);
	void SetVariable(std::string_view p_variableName, int p_value, bool p_debug = false);
	void SetVariable(std::string_view p_variableName, glm::vec3 p_value, bool p_debug = false);
	void SetVariable(std::string_view p_variableName, glm::vec4 p_value, bool p_debug = false);

public:
	const std::string _vertexShaderPath;
	const std::string _fragmentShaderPath;
private:
	unsigned int _id;
	bool _failed;

};

