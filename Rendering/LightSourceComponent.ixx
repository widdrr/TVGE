export module Graphics.Components:LightSourceComponent;

import Common;
import Graphics.Resources;

import <glm/vec3.hpp>;

import <memory>;

export class LightSourceComponent : public Component 
{
protected:
	LightSourceComponent(Entity& p_entity,
		const glm::vec3 & p_ambient, const glm::vec3& p_diffuse, const glm::vec3& p_specular);
public:
	virtual void SetLightVariables(ShaderProgram& p_shaderProgram, int index) = 0;

protected:
	const glm::vec3 _ambientColor;
	const glm::vec3 _diffuseColor;
	const glm::vec3 _specularColor;
};

export class PointLightComponent : public LightSourceComponent 
{
public:
	PointLightComponent(Entity& p_entity,
		const glm::vec3& p_ambient, const glm::vec3& p_diffuse, const glm::vec3& p_specular,
		const float p_quadraticAttenuation = 0.007f, const float p_linearAttenuation = 0.022f, const float p_constantAttenuation = 1.f);
public:
	void SetLightVariables(ShaderProgram& p_shaderProgram, int index = 0) override;

private:
	float _constantAttenuation;
	float _linearAttenuation;
	float _quadraticAttenuation;
};

export class DirectionalLightComponent : public LightSourceComponent 
{
private:
	static const glm::vec3 _defaultDirection;

public:
	DirectionalLightComponent(Entity& p_entity,
		const glm::vec3& p_ambient, const glm::vec3& p_diffuse, const glm::vec3& p_specular);

public:
	void SetLightVariables(ShaderProgram& p_shaderProgram, int index = 0) override;
};