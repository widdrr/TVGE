export module Physics.Components:ColliderComponent;

import Common;

import <functional>;
import <vector>;

export class ColliderComponent : Component
{
	friend class Entity;
	using Callback = std::function<void(const Entity&)>;

public:
	virtual bool Intersects(const ColliderComponent& p_other) const = 0;
	void AddCollisionEventHandler(Callback p_callback);

protected:
	void SendCollisionEvent(const Entity& p_other);
	ColliderComponent(Entity& p_entity);
	std::vector<Callback> _collisionCallbacks;
};