module Physics.Components:BoxColliderComponent;

using namespace TVGE;
using namespace TVGE::Physics;

BoxColliderComponent::BoxColliderComponent(Entity& p_entity, const bool p_physical, glm::vec3 p_extents, glm::vec3 p_center, glm::mat3 p_axes) :
	ColliderComponent(p_entity, Box, p_physical),
	localExtents(p_extents),
	localCenter(p_center),
	localAxes(p_axes)
{
	localExtents = glm::abs(localExtents);
	for (int i = 0; i < 3; ++i) {
		localAxes[i] = glm::normalize(localAxes[i]);
	}
	ApplyTransformations();
}

void BoxColliderComponent::ApplyTransformations()
{
	auto rotation =entity.GetAbsoluteRotation();

	_center = rotation * localCenter + entity.GetAbsolutePosition();
	
	_extents = localExtents * entity.GetAbsoluteScaling();
	_extents = glm::abs(_extents);

	_axes = glm::mat3_cast(rotation) * localAxes ;

	_boundingBox = ResizeBoundingBox();
}

const BoundingBox BoxColliderComponent::ResizeBoundingBox()
{
	glm::vec3 axisExtents(0);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			axisExtents[i] += glm::abs(_axes[i][j]) * _extents[j];
		}
	}

	return BoundingBox(-axisExtents + _center, axisExtents + _center);
}

glm::mat3 BoxColliderComponent::ComputeInertiaMatrix(float p_mass)
{
	float factor = p_mass / 12.f;
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
	auto component = p_entity.CreateComponentOfType<BoxColliderComponent>(physical, localExtents, localCenter, localAxes).lock();
	return component;
}