export module Physics.Components:ColliderComponent;

import Common;
import Physics.Resources;

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
	using Callback = std::function<void(Entity&, const Collision&)>;

public:
	virtual void ApplyTransformations() = 0;
	void SendCollisionEvent(Entity& p_other, const Collision& p_info);
	void AddCollisionEventHandler(Callback p_callback);

	const ColliderTypes type;
	bool physical;

protected:
	ColliderComponent(Entity& p_entity, ColliderTypes p_type, const bool p_physical = true);
	std::vector<Callback> _collisionCallbacks;
};