export module Graphics.Components:LightSourceComponent;

import Common;
import <glm/vec3.hpp>;

//TODO: big, integrated light sources into the ECS cleanly
export class LightSourceComponent : public Component {

private:
	glm::vec3 _lightColor;
	const float _ambienceStrength;
	const float _specularStrength;
public:
	LightSourceComponent(Entity& p_entity, 
						 float p_lightR = 1.f, float p_lightG = 1.f, float p_lightB = 1.f, 
						 float p_ambience = 0.1f, float p_specular = 0.3f);

	const glm::vec3& getLightColor() const;
	const glm::vec3& getLightPosition() const;
	const float getAmbienceStrength() const;
	const float getSpecularStrength() const;
};

