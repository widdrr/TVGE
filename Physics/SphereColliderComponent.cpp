module Physics.Components:SphereColliderComponent;

import <glm/mat3x3.hpp>;

using namespace TVGE;
using namespace TVGE::Physics;

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

	_boundingBox = ResizeBoundingBox();
}

const BoundingBox SphereColliderComponent::ResizeBoundingBox()
{
	return BoundingBox(-_radius + _center, _radius + _center);
}

glm::mat3 SphereColliderComponent::ComputeInertiaMatrix(float p_mass)
{
	float inertia = 0.4f * p_mass * _radius * _radius;
	return glm::mat3(inertia);
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
