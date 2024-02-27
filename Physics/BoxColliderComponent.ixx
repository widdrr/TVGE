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
						 glm::mat3 p_axes = glm::mat3(1.f),
						 const bool p_physical = true);

	void ApplyTransformations() override;
	glm::mat3 ComputeInertiaMatrix(float p_mass) override;

	glm::vec3 GetExtents() const;
	glm::vec3 GetCenter() const;
	glm::mat3 GetAxes() const;

	glm::vec3 localExtents;
	glm::vec3 localCenter;
	glm::mat3 localAxes;

protected:
	std::shared_ptr<Component> Clone(Entity& p_entity) const override;

private:
	glm::vec3 _extents;
	glm::vec3 _center;
	glm::mat3 _axes;
};