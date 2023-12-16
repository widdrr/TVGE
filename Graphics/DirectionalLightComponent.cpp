module Graphics.Components:LightSourceComponent;

const glm::vec3 DirectionalLightComponent::_defaultDirection = glm::vec3(0.f, 1.f, 0.f);

DirectionalLightComponent::DirectionalLightComponent(Entity& p_entity, const glm::vec3& p_ambient, const glm::vec3& p_diffuse, const glm::vec3& p_specular) :
	LightSourceComponent(p_entity, p_ambient, p_diffuse, p_specular)
{
}
void DirectionalLightComponent::SetLightVariables(ShaderProgram& p_shader, Camera& p_camera, int p_index)
{
	using namespace UniformVariables::Light;

	glm::vec3 direction = _defaultDirection * _entity.rotation;

	p_shader.SetVariable(InsertArrayIndex(lightPositionArray, p_index), p_camera.GetViewTransformation() * glm::vec4(direction, 0.f));
	p_shader.SetVariable(InsertArrayIndex(lightAmbientColorArray, p_index), _ambientColor);
	p_shader.SetVariable(InsertArrayIndex(lightDiffuseColorArray, p_index), _diffuseColor);
	p_shader.SetVariable(InsertArrayIndex(lightSpecularColorArray, p_index), _specularColor);
}