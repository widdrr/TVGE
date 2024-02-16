export module Physics.CollisionFunctions;

import Physics.Components;
import Physics.Resources;

import <optional>;
import <functional>;

export using CollisionFunction = std::function<std::optional<Collision>(const ColliderComponent&, const ColliderComponent&)>;

export namespace CollisionFunctions
{
	//TODO: GPU collision detection
	std::optional<Collision> IntersectBox_Box(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider);
	std::optional<Collision> IntersectSphere_Sphere(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider);
	std::optional<Collision> IntersectSphere_Box(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider);
	
}