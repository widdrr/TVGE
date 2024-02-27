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

glm::mat3 BoxColliderComponent::ComputeInertiaMatrix(float p_mass)
{
	float factor = p_mass / 3.f;
	float inertiaXX = factor * (_extents.y * _extents.y + _extents.z * _extents.z);
	float inertiaYY = factor * (_extents.x * _extents.x + _extents.z * _extents.z);
	float inertiaZZ = factor * (_extents.x * _extents.x + _extents.y * _extents.y);
	
	return glm::mat3(inertiaXX, 0.f, 0.f, 
					 0.f, inertiaYY, 0.f,  
					 0.f, 0.f, inertiaZZ);
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