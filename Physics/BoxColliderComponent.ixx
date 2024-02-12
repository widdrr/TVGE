export module Physics.Components:BoxColliderComponent;

import :ColliderComponent;

import <glm/vec3.hpp>;

export class BoxColliderComponent : ColliderComponent
{
	friend class Entity;
public:
	bool Intersects(const ColliderComponent& p_other) const override;
private:
	BoxColliderComponent(Entity& p_entity, glm::vec3 p_bounds, glm::vec3 p_center = {0.f,0.f,0.f});

	glm::vec3 _center;
	glm::vec3 _bounds;
};