export module Physics.CollisionFunctions;

import Physics.Components;
import Physics.Resources;

import <glm/vec3.hpp>;

import <optional>;
import <functional>;

namespace TVGE::Physics
{
	export using CollisionFunction = std::function<std::optional<CollisionEvent>(const ColliderComponent&, const ColliderComponent&)>;

	export namespace CollisionFunctions
	{
		//TODO: GPU collision detection
		std::optional<CollisionEvent> IntersectBox_Box(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider);
		std::optional<CollisionEvent> IntersectSphere_Sphere(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider);
		std::optional<CollisionEvent> IntersectSphere_Box(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider);


		bool BoundingBoxOverlap(const BoundingBox& b1, const BoundingBox& b2);
	}
}