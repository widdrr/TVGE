export module Physics.Components:ColliderComponent;

import Common;
import Physics.Resources;

import <glm/mat3x3.hpp>;

import <functional>;
import <vector>;
import <optional>;

export enum ColliderTypes
{
	Undefined = 0,
	Box,
	Sphere
};

export class ColliderComponent : public Component
{
	using Callback = std::function<void(Entity&, const CollisionEvent)>;

public:
	virtual void ApplyTransformations() = 0;
	const BoundingBox GetBoundingBox() const;
	void SendCollisionEvent(Entity& p_other, const CollisionEvent p_info);
	void AddCollisionEventHandler(Callback p_callback);
	virtual glm::mat3 ComputeInertiaMatrix(float p_mass) = 0;

	const ColliderTypes type;
	bool physical;

protected:
	ColliderComponent(Entity& p_entity, ColliderTypes p_type, const bool p_physical = true);
	virtual const BoundingBox ResizeBoundingBox() = 0;
	std::vector<Callback> _collisionCallbacks;
	BoundingBox _boundingBox;
};