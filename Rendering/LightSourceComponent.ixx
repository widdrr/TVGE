export module Graphics.Components:LightSourceComponent;

import Common;
import Graphics.Resources;

import <glm/vec3.hpp>;

import <memory>;

//TODO: big, integrate light sources into the ECS cleanly
export class LightSourceComponent : public Component 
{
public:
	LightSourceComponent(Entity& p_entity,
		const glm::vec3 & p_ambient, const glm::vec3& p_diffuse, const glm::vec3& p_specular);

	void SetLightVariables(ShaderProgram& p_shaderProgram);

private:
	const glm::vec3 _ambientColor;
	const glm::vec3 _diffuseColor;
	const glm::vec3 _specularColor;
};