module Physics.Components:BoxColliderComponent;

BoxColliderComponent::BoxColliderComponent(Entity& p_entity, glm::vec3 p_extents, glm::vec3 p_center, glm::mat3 p_axes, const bool p_physical) :
	ColliderComponent(p_entity, Box, p_physical),
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

	_axes = localAxes * glm::mat3_cast(entity.rotation);
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

std::shared_ptr<Component> BoxColliderComponent::Clone(Entity& p_entity) const
{
	auto component = p_entity.CreateComponentOfType<BoxColliderComponent>(localExtents, localCenter, localAxes, physical).lock();
	return component;
}