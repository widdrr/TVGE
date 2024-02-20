module Physics.Components:SphereColliderComponent;

import <glm/mat3x3.hpp>;

SphereColliderComponent::SphereColliderComponent(Entity& p_entity, float p_radius, glm::vec3 p_center, const bool p_physical):
	ColliderComponent(p_entity, Sphere, p_physical),
	localRadius(glm::abs(p_radius)),
	localCenter(p_center)
{}

void SphereColliderComponent::ApplyTransformations()
{
	_center = localCenter + entity.position;
	glm::vec3 absScaling = glm::abs(entity.scaling);
	_radius = localRadius * glm::max(absScaling.x, glm::max(absScaling.y, absScaling.z));
}

float SphereColliderComponent::GetRadius() const
{
	return _radius;
}

glm::vec3 SphereColliderComponent::GetCenter() const
{
	return _center;
}

std::shared_ptr<Component> SphereColliderComponent::Clone(Entity& p_entity) const
{
	auto component = p_entity.CreateComponentOfType<SphereColliderComponent>(localRadius, localCenter, physical).lock();
	return component;
}
