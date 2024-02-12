export module Physics:CollisionHandler;

import Physics.Components;

import <memory>;

export class CollisionHandler
{
	CollisionHandler();

	virtual void HandleCollisions() = 0;
	//virtual void AddCollider(std::weak_ptr<ColliderComponent> p_collider);
};