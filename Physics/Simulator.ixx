export module Physics:Simulator;

import Common;
import Physics.Components;
import Physics.Resources;
import :CollisionHandler;

import <glm/vec3.hpp>;

import <vector>;
import <memory>;

namespace TVGE::Physics
{
	export class Simulator
	{
	public:
		Simulator();
		void SimulateStep(float p_delta);
		void UpdateBodies(float p_delta);

		void AddObject(const Entity& p_object);
		void CleanDanglingPointers();

		static float gravityStrength;
		static float airDynamicFriction;
		static float airStaticFriction;
		static float efRestCoef;


	private:
		void ResolveCollisions(std::vector<CollisionEvent> p_collisions);
		void ApplyCollisionStatic(BodyComponent& p_body, glm::vec3 p_point, glm::vec3 p_normal);
		void ApplyCollisionDynamic(BodyComponent& p_body, BodyComponent& p_other, glm::vec3 p_point, glm::vec3 p_otherPoint, glm::vec3 p_normal);

		std::vector<std::weak_ptr<BodyComponent>> _bodies;
		std::unique_ptr<CollisionHandler> _collisionHandler;
	};
}