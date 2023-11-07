module Graphics.Components:LightSourceComponent;

LightSourceComponent::LightSourceComponent(Entity& p_entity, 
					const glm::vec3& p_ambient, const glm::vec3& p_diffuse, const glm::vec3& p_specular):
	Component(p_entity),
	_ambientColor(p_ambient),
	_diffuseColor(p_diffuse),
	_specularColor(p_specular)
{}

void LightSourceComponent::SetLightVariables(ShaderProgram& p_shader)
{
	using namespace UniformVariables::Light;

	p_shader.SetVariable(lightPosition, _entity.position);
	p_shader.SetVariable(lightAmbientColor, _ambientColor);
	p_shader.SetVariable(lightDiffuseColor, _diffuseColor);
	p_shader.SetVariable(lightSpecularColor, _specularColor);
}