export module Physics:CollisionHandler;

import Physics.Components;
import Physics.Resources;
import Physics.CollisionFunctions;

import <memory>;
import <vector>;
import <optional>;
import <unordered_map>;
import <functional>;

namespace TVGE::Physics
{
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
		virtual std::vector<CollisionEvent> DetectCollisions() = 0;
		virtual void AddCollider(std::weak_ptr<ColliderComponent> p_collider);
		virtual void CleanDanglingPointers();

	protected:
		virtual std::optional<CollisionEvent> Intersect(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider);

		std::unordered_map<ColliderPair, CollisionFunction, PairHash> _collisionFunctions;
		std::vector<std::weak_ptr<ColliderComponent>> _colliders;
	};
}