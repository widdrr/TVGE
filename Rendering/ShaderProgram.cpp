module Graphics.Resources:ShaderProgram;

import <glm/gtc/type_ptr.hpp>;
import <gl/glew.h>;

import <fstream>;
import <iostream>;

ShaderProgram::ShaderProgram(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath){
	
	_id = glCreateProgram();

	if (_id == 0) {
		std::cerr << "Error creating shader program\n";
		exit(1);
	}

	AddShader(ReadShaderFromFile(p_vertexShaderPath), GL_VERTEX_SHADER);
	AddShader(ReadShaderFromFile(p_fragmentShaderPath), GL_FRAGMENT_SHADER);

	glLinkProgram(_id);

	GLint linkSuccess;
	glGetProgramiv(_id, GL_LINK_STATUS, &linkSuccess);

	if (!linkSuccess) {
		GLchar dump[_dumpSize];
		glGetProgramInfoLog(_id, _dumpSize, NULL, dump);
		std::cerr << "Error while linking shader program: " << dump << "\n";
		exit(1);
	}

	glValidateProgram(_id);

	GLint validateSuccess;
	glGetProgramiv(_id, GL_VALIDATE_STATUS, &validateSuccess);

	if (!validateSuccess) {
		GLchar dump[_dumpSize];
		glGetProgramInfoLog(_id, _dumpSize, NULL, dump);
		std::cerr << "Error while validating shader program: " << dump << "\n";
		exit(1);
	}

	glUseProgram(_id);

}

void ShaderProgram::SetVariable(std::string p_variableName, glm::mat4 p_value, bool p_debug) {
	
	GLuint location = glGetUniformLocation(_id, p_variableName.c_str());
	if (location == -1) {
		if (p_debug) {
			std::cerr << "Variable " << p_variableName << " not defined in Vertex Shader\n";
			return;
		}
	}

	glProgramUniformMatrix4fv(_id, location, 1, GL_FALSE, glm::value_ptr(p_value));
}

void ShaderProgram::SetVariable(std::string p_variableName, glm::vec3 p_value, bool p_debug) {

	GLuint location = glGetUniformLocation(_id, p_variableName.c_str());
	if (location == -1) {
		if (p_debug) {
			std::cerr << "Variable " << p_variableName << " not defined in Vertex Shader\n";
			return;
		}
	}

	glProgramUniform3fv(_id, location, 1, glm::value_ptr(p_value));
}


void ShaderProgram::SetVariable(std::string p_variableName, float p_value, bool p_debug) {

	GLuint location = glGetUniformLocation(_id, p_variableName.c_str());
	if (location == -1) {
		if (p_debug) {
			std::cerr << "Variable " << p_variableName << " not defined in Vertex Shader\n";
			return;
		}
	}

	glProgramUniform1f(_id, location, p_value);
}

void ShaderProgram::AddShader(std::string p_shaderText, unsigned int p_shaderType){
	
	GLuint shader = glCreateShader(p_shaderType);

	if (shader == 0) {
		std::cerr << "Error creating shader object \n";
		exit(1);
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
		exit(1);
	}

	glAttachShader(_id, shader);

	glDeleteShader(shader);
}


std::string ShaderProgram::ReadShaderFromFile(std::string p_fileName){
	
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
