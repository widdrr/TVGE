export module Physics:CollisionHandler;

import Physics.Components;
import Physics.Resources;
import Physics.CollisionFunctions;

import <memory>;
import <vector>;
import <optional>;
import <unordered_map>;
import <functional>;

using ColliderPair = std::pair<ColliderTypes, ColliderTypes>;

struct PairHash final
{
public:
	size_t operator()(const ColliderPair& p) const
	{
		return std::hash<unsigned int>()(p.first) + std::hash<unsigned int>()(p.second);
	}
};

export class CollisionHandler
{
public:
	CollisionHandler();
	std::vector<Collision> DetectCollisions();
	void AddCollider(std::weak_ptr<ColliderComponent> p_collider);
	void CleanDanglingPointers();

private:
	std::optional<Collision> Intersect(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider);
	//TODO: octree
	std::unordered_map<ColliderPair, CollisionFunction, PairHash> _collisionFunctions;
	std::vector<std::weak_ptr<ColliderComponent>> _colliders;
};