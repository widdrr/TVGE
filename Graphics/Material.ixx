export module Graphics.Resources:Material;

import :Texture;
import :ShaderProgram;

import <glm/vec3.hpp>;
import <glm/vec4.hpp>;

import <memory>;

export struct LightProperties 
{
public:
	LightProperties();

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

	//TODO: multiple textures per type, more generic
	std::weak_ptr<Texture> _ambientMap;
	std::weak_ptr<Texture> _diffuseMap;
	std::weak_ptr<Texture> _specularMap;
	ShaderProgram& _shader;
	//TODO: per-vertex coloring?
	LightProperties _lightProperties;
};