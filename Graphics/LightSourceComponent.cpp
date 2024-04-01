module Graphics.Components:LightSourceComponent;

using namespace TVGE;
using namespace TVGE::Graphics;

LightSourceComponent::LightSourceComponent(Entity& p_entity, 
					const glm::vec3& p_ambient, const glm::vec3& p_diffuse, const glm::vec3& p_specular):
	Component(p_entity),
	_ambientColor(p_ambient),
	_diffuseColor(p_diffuse),
	_specularColor(p_specular)
{}
