module Graphics.Components:LightSourceComponent;

PointLightComponent::PointLightComponent(Entity& p_entity,
										 const glm::vec3& p_ambient,
										 const glm::vec3& p_diffuse,
										 const glm::vec3& p_specular,
										 const glm::vec3& p_positionOffset,
										 const float p_quadraticAttenuation,
										 const float p_linearAttenuation,
										 const float p_constantAttenuation) :
	LightSourceComponent(p_entity, p_ambient, p_diffuse, p_specular),
	_offset(p_positionOffset),
	_constantAttenuation(p_constantAttenuation),
	_linearAttenuation(p_linearAttenuation),
	_quadraticAttenuation(p_quadraticAttenuation)
{
}

glm::vec4 PointLightComponent::GetPosition() const
{
	return glm::vec4(glm::vec4(entity.position + _offset, 1.f));
}

void PointLightComponent::SetLightVariables(ShaderProgram& p_shader, int p_index)
{
	using namespace UniformVariables;
	using namespace UniformVariables::Lights;

	p_shader.SetVariable(InsertArrayIndex(lightPositionArray, p_index), GetPosition());

	p_shader.SetVariable(InsertArrayIndex(lightAmbientColorArray, p_index), _ambientColor);
	p_shader.SetVariable(InsertArrayIndex(lightDiffuseColorArray, p_index), _diffuseColor);
	p_shader.SetVariable(InsertArrayIndex(lightSpecularColorArray, p_index), _specularColor);

	p_shader.SetVariable(InsertArrayIndex(lightAttenuationConstantArray, p_index), _constantAttenuation);
	p_shader.SetVariable(InsertArrayIndex(lightAttenuationLinearArray, p_index), _linearAttenuation);
	p_shader.SetVariable(InsertArrayIndex(lightAttenuationQuadraticArray, p_index), _quadraticAttenuation);
}