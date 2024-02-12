module Physics.Components:BoxColliderComponent;

BoxColliderComponent::BoxColliderComponent(Entity& p_entity, glm::vec3 p_bounds, glm::vec3 p_center):
	ColliderComponent(p_entity),
	_bounds(p_bounds),
	_center(p_center)
{}

bool BoxColliderComponent::Intersects(const ColliderComponent& p_other) const
{
	return false;
}