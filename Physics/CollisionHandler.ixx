export module Physics:CollisionHandler;

import Physics.Components;
import Physics.Resources;

import <memory>;
import <vector>;

export class CollisionHandler
{
public:
	CollisionHandler();
	std::vector<Collision> DetectCollisions();
	void AddCollider(std::weak_ptr<ColliderComponent> p_collider);

private:
	//TODO: octree
	std::vector<std::weak_ptr<ColliderComponent>> _colliders;
};