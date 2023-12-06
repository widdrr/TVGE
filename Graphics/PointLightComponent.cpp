module Graphics.Components:LightSourceComponent;

PointLightComponent::PointLightComponent(Entity& p_entity, 
		const glm::vec3& p_ambient, const glm::vec3& p_diffuse, const glm::vec3& p_specular,
		const float p_quadraticAttenuation, const float p_linearAttenuation, const float p_constantAttenuation):
	LightSourceComponent(p_entity, p_ambient, p_diffuse, p_specular),
	_constantAttenuation(p_constantAttenuation),
	_linearAttenuation(p_linearAttenuation),
	_quadraticAttenuation(p_quadraticAttenuation)
{}

void PointLightComponent::SetLightVariables(ShaderProgram& p_shader, int index)
{
	using namespace UniformVariables::Light;

	p_shader.SetVariable(InsertArrayIndex(lightPositionArray, index), glm::vec4(_entity.position, 1.f));
	
	p_shader.SetVariable(InsertArrayIndex(lightAmbientColorArray, index), _ambientColor);
	p_shader.SetVariable(InsertArrayIndex(lightDiffuseColorArray, index), _diffuseColor);
	p_shader.SetVariable(InsertArrayIndex(lightSpecularColorArray, index), _specularColor);

	p_shader.SetVariable(InsertArrayIndex(lightAttenuationConstantArray, index), _constantAttenuation);
	p_shader.SetVariable(InsertArrayIndex(lightAttenuationLinearArray, index), _linearAttenuation);
	p_shader.SetVariable(InsertArrayIndex(lightAttenuationQuadraticArray, index), _quadraticAttenuation);
}