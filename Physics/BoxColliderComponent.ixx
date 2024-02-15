export module Physics.Components:BoxColliderComponent;

import :ColliderComponent;

import <glm/vec3.hpp>;
import <glm/mat3x3.hpp>;

export class BoxColliderComponent : public ColliderComponent
{

public:
	BoxColliderComponent(Entity& p_entity, 
						 glm::vec3 p_extents = { 0.5f, 0.5f, 0.5f }, 
						 glm::vec3 p_center = { 0.f, 0.f, 0.f }, 
						 glm::mat3 p_axes = glm::mat3(1.f));

	std::optional<Collision> Intersects(const ColliderComponent& p_other) const override;
	void ApplyTransformations() override;

private:
	std::optional<Collision> Intersects(const BoxColliderComponent& p_other) const;

	glm::vec3 _localExtents;
	glm::vec3 _localCenter;
	glm::mat3 _localAxes;

	glm::vec3 _extents;
	glm::vec3 _center;
	glm::mat3 _axes;
};