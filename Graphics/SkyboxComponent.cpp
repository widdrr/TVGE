module Graphics.Components:SkyboxComponent;

using namespace TVGE;
using namespace TVGE::Graphics;

SkyboxComponent::SkyboxComponent(Entity& p_entity, ShaderProgram& p_shader) :
	Component(p_entity),
	shader(p_shader)
{
}
std::shared_ptr<Component> SkyboxComponent::Clone(Entity& p_entity) const
{
	auto component = p_entity.CreateComponentOfType<SkyboxComponent>(std::ref(shader)).lock();
	component->mesh = mesh;
	component->texture = texture;

	return component;
}
;