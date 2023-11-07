export module Graphics.Resources:Material;

import :Texture;
import :ShaderProgram;

import <glm/vec3.hpp>;
import <glm/vec4.hpp>;

import <memory>;

export struct LightProperties 
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emissive;
	float shininess;
};

export class Material 
{
public:
	Material(ShaderProgram& p_shader);
	void SetMaterialVariables();

	//TODO: multiple texture types
	std::weak_ptr<Texture> _texture;
	ShaderProgram& _shader;
	//TODO: per-vertex coloring?
	LightProperties _lightProperties;
};