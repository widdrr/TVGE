export module Graphics.Components:LightSourceComponent;

import Common;

import <glm/vec3.hpp>;

import <memory>;

//TODO: big, integrate light sources into the ECS cleanly
export class LightSourceComponent : public Component 
{
public:
	LightSourceComponent(Entity& p_entity,
		float p_lightR = 1.f, float p_lightG = 1.f, float p_lightB = 1.f,
		float p_ambience = 0.1f, float p_diffuse = 1.f, float p_specular = 0.3f);

	const glm::vec3& GetLightColor() const;
	const glm::vec3& GetLightPosition() const;
	const float GetAmbienceStrength() const;
	const float GetDiffuseStrength() const;
	const float GetSpecularStrength() const;

private:
	glm::vec3 _lightColor;
	//TODO: these might be moved to materials
	const float _ambienceStrength;
	const float _diffuseStrength;
	const float _specularStrength;
};