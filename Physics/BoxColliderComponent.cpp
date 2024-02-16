module Physics.Components:BoxColliderComponent;

BoxColliderComponent::BoxColliderComponent(Entity& p_entity, glm::vec3 p_extents, glm::vec3 p_center, glm::mat3 p_axes) :
	ColliderComponent(p_entity, Box),
	localExtents(p_extents),
	localCenter(p_center),
	localAxes(p_axes)
{
	localExtents = glm::abs(localExtents);
}

void BoxColliderComponent::ApplyTransformations()
{
	_center = localCenter + entity.position;
	
	_extents = localExtents * entity.scaling;
	_extents = glm::abs(_extents);

	_axes = glm::mat3_cast(entity.rotation) * localAxes;
}

glm::vec3 BoxColliderComponent::GetExtents() const
{
	return _extents;
}

glm::vec3 BoxColliderComponent::GetCenter() const
{
	return _center;
}

glm::mat3 BoxColliderComponent::GetAxes() const
{
	return _axes;
}
