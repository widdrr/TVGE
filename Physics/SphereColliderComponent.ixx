export module Physics.Components:SphereColliderComponent;

import :ColliderComponent;

import <glm/vec3.hpp>;

//TODO: spehroids
export class SphereColliderComponent : public ColliderComponent
{
public:
	SphereColliderComponent(Entity& p_entity,
							float p_radius = 1,
							glm::vec3 p_center = { 0.f, 0.f, 0.f },
							const bool p_physical = true);

	void ApplyTransformations() override;
	glm::mat3 ComputeInertiaMatrix(float p_mass) override;

	float GetRadius() const;
	glm::vec3 GetCenter() const;

	float localRadius;
	glm::vec3 localCenter;

protected:
	const BoundingBox ResizeBoundingBox() override;
	std::shared_ptr<Component> Clone(Entity& p_entity) const override;

private:
	float _radius;
	glm::vec3 _center;
};