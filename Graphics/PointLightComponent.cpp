module Graphics.Components:LightSourceComponent;

PointLightComponent::PointLightComponent(Entity& p_entity, 
		const glm::vec3& p_ambient, const glm::vec3& p_diffuse, const glm::vec3& p_specular,
		const float p_quadraticAttenuation, const float p_linearAttenuation, const float p_constantAttenuation):
	LightSourceComponent(p_entity, p_ambient, p_diffuse, p_specular),
	_constantAttenuation(p_constantAttenuation),
	_linearAttenuation(p_linearAttenuation),
	_quadraticAttenuation(p_quadraticAttenuation)
{}

void PointLightComponent::SetLightVariables(ShaderProgram& p_shader, Camera& p_camera, int p_index)
{
	using namespace UniformVariables::Light;

	p_shader.SetVariable(InsertArrayIndex(lightPositionArray, p_index), p_camera.GetViewTransformation() * glm::vec4(_entity.position, 1.f));
	
	p_shader.SetVariable(InsertArrayIndex(lightAmbientColorArray, p_index), _ambientColor);
	p_shader.SetVariable(InsertArrayIndex(lightDiffuseColorArray, p_index), _diffuseColor);
	p_shader.SetVariable(InsertArrayIndex(lightSpecularColorArray, p_index), _specularColor);

	p_shader.SetVariable(InsertArrayIndex(lightAttenuationConstantArray, p_index), _constantAttenuation);
	p_shader.SetVariable(InsertArrayIndex(lightAttenuationLinearArray, p_index), _linearAttenuation);
	p_shader.SetVariable(InsertArrayIndex(lightAttenuationQuadraticArray, p_index), _quadraticAttenuation);
}