module Graphics.Components:LightSourceComponent;

LightSourceComponent::LightSourceComponent(Entity& p_entity, 
										   float p_lightR, float p_lightG, float p_lightB,
										   float p_ambience, float p_diffuse, float p_specular):
	Component(p_entity),
	_lightColor(p_lightR, p_lightG, p_lightB),
	_ambienceStrength(p_ambience),
	_diffuseStrength(p_diffuse),
	_specularStrength(p_specular)
{
}

const glm::vec3& LightSourceComponent::getLightColor() const {
	
	return _lightColor;
}

const glm::vec3& LightSourceComponent::getLightPosition() const {
	
	return _entity.position;
}

const float LightSourceComponent::getAmbienceStrength() const {
	
	return _ambienceStrength;
}

const float LightSourceComponent::getDiffuseStrength() const {
	return _diffuseStrength;
}

const float LightSourceComponent::getSpecularStrength() const {
	
	return _specularStrength;
}
