module Graphics.Components:LightSourceComponent;

const glm::vec3 DirectionalLightComponent::_defaultDirection = glm::vec3(0.f, 1.f, 0.f);

DirectionalLightComponent::DirectionalLightComponent(Entity& p_entity, const glm::vec3& p_ambient, const glm::vec3& p_diffuse, const glm::vec3& p_specular) :
	LightSourceComponent(p_entity, p_ambient, p_diffuse, p_specular)
{
}

glm::vec4 DirectionalLightComponent::GetPosition() const
{
	return glm::vec4(_defaultDirection * entity.rotation, 0.f);
}

void DirectionalLightComponent::SetLightVariables(ShaderProgram& p_shader, int p_index)
{
	using namespace UniformVariables;
	using namespace UniformVariables::Lights;

	p_shader.SetVariable(InsertArrayIndex(lightPositionArray, p_index), GetPosition());
	p_shader.SetVariable(InsertArrayIndex(lightAmbientColorArray, p_index), _ambientColor);
	p_shader.SetVariable(InsertArrayIndex(lightDiffuseColorArray, p_index), _diffuseColor);
	p_shader.SetVariable(InsertArrayIndex(lightSpecularColorArray, p_index), _specularColor);


}

std::shared_ptr<Component> DirectionalLightComponent::Clone(Entity& p_entity) const
{
	auto component = p_entity.CreateComponentOfType<DirectionalLightComponent>(_ambientColor,
																		 _diffuseColor,
																		 _specularColor).lock();

	return component;
}
