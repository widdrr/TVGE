export module Physics:Simulator;

import Common;
import Physics.Components;
import Physics.Resources;
import :CollisionHandler;
import :SweepCollisionHandler;
import :NaiveCollisionHandler;

import <glm/vec3.hpp>;

import <vector>;
import <memory>;
import <type_traits>;
import <glm/vec3.hpp>;
import <glm/geometric.hpp>;
import <glm/gtx/norm.hpp>;

import <iostream>;

namespace TVGE::Physics
{
	template<class TCollisionHandler>
	concept IsCollisionHandler = std::is_base_of<CollisionHandler, TCollisionHandler>::value;

export	
    template<IsCollisionHandler TCollisionHandler = SweepCollisionHandler>
	class Simulator
	{
	public:
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
		TCollisionHandler _collisionHandler;
	};

	template<IsCollisionHandler T>
	float Simulator<T>::gravityStrength = 9.8f;

	template<IsCollisionHandler T>
	float Simulator<T>::airDynamicFriction = 1.f;

	template<IsCollisionHandler T>
	float Simulator<T>::efRestCoef = -1.5;

	template<IsCollisionHandler T>
	void Simulator<T>::SimulateStep(float p_delta)
	{
		auto collisions = _collisionHandler.DetectCollisions();
		ResolveCollisions(collisions);
		UpdateBodies(p_delta);
	}

	template<IsCollisionHandler T>
	void Simulator<T>::UpdateBodies(float p_delta)
	{
		for (auto&& wbody : _bodies) {
			if (wbody.expired()) {
				continue;
			}
			auto body = wbody.lock();
			if (body->gravity) {
				body->AddForce(glm::vec3(0.f, -1.f, 0.f) * body->_mass * gravityStrength);
			
				glm::vec3 linearFriction = -body->velocity * airDynamicFriction;
				auto frictionMag = glm::length2(linearFriction);
				if (frictionMag != 0.f && frictionMag <= EPSILON) {
					linearFriction *= 10.f;
				}
				body->AddForce(linearFriction);

				glm::vec3 angularFriction = -body->angularVelocity * airDynamicFriction;
				frictionMag = glm::length2(angularFriction);
				if (frictionMag != 0.f && frictionMag <= EPSILON) {
					angularFriction *= 10.f;
				}
				body->AddTorque(angularFriction);
			}
			body->UpdateInertiaMatrix();
			body->Update(p_delta);
		}
	}

	template<IsCollisionHandler T>
	void Simulator<T>::AddObject(const Entity& p_object)
	{
		//obtaining the GraphicsComponent of the Entity
		auto body = p_object.TryGetComponentOfType<BodyComponent>();
		if (body.expired()) {
			std::cerr << "Object does not have a Body Component\n";
		}
		else {
			_bodies.push_back(body);
		}

		auto collider = p_object.TryGetComponentOfType<ColliderComponent>();
		if (!collider.expired()) {
			_collisionHandler.AddCollider(collider);
		}
	}

	template<IsCollisionHandler T>
	void Simulator<T>::CleanDanglingPointers()
	{
		std::vector<std::weak_ptr<BodyComponent>> validBodies;

		for (auto&& comp : _bodies) {
			if (!comp.expired()) {
				validBodies.push_back(comp);
			}
		}

		_bodies = validBodies;

		_collisionHandler.CleanDanglingPointers();
	}

	template<IsCollisionHandler T>
	void Simulator<T>::ResolveCollisions(std::vector<CollisionEvent> p_collisions)
	{
		for (auto&& collision : p_collisions) {
			auto wbody1 = collision.entity1.TryGetComponentOfType<BodyComponent>();
			auto wbody2 = collision.entity2.TryGetComponentOfType<BodyComponent>();

			if (!wbody1.expired() && !wbody2.expired()) {
				auto body1 = wbody1.lock();
				auto body2 = wbody2.lock();

				ApplyCollisionDynamic(*body1, *body2, collision.contactPoint1, collision.contactPoint2, collision.collisionNormal);
			}
			else if (!wbody1.expired()) {
				auto body1 = wbody1.lock();

				ApplyCollisionStatic(*body1, collision.contactPoint1, collision.collisionNormal);
			}
			else if (!wbody2.expired()) {
				auto body2 = wbody2.lock();

				ApplyCollisionStatic(*body2, collision.contactPoint2, -1.f * collision.collisionNormal);
			}
		}
	}

	template<IsCollisionHandler T>
	void Simulator<T>::ApplyCollisionStatic(BodyComponent& p_body, glm::vec3 p_point, glm::vec3 p_normal)
	{
		glm::vec3 collisionNormal = glm::normalize(p_normal);

		glm::vec3 support = p_point - p_body.entity.GetAbsolutePosition();
		glm::vec3 collisionVelocity = p_body.velocity + glm::cross(p_body.angularVelocity, support);

		p_body.entity.Translate(p_normal);

		glm::vec3 scaledAngularVel = glm::cross(p_body._inverseInertiaMatrix * glm::cross(support, collisionNormal), support);

		float impulse = efRestCoef * glm::dot(collisionVelocity, collisionNormal) /
			(p_body._inverseMass + glm::dot(scaledAngularVel, collisionNormal));

		p_body.AddForceInstant(collisionNormal * impulse);
		p_body.AddTorqueInstant(impulse * glm::cross(support, collisionNormal));
	}

	template<IsCollisionHandler T>
	void Simulator<T>::ApplyCollisionDynamic(BodyComponent& p_body, BodyComponent& p_other, glm::vec3 p_point, glm::vec3 p_otherPoint, glm::vec3 p_normal)
	{
		glm::vec3 collisionNormal = glm::normalize(p_normal);

		glm::vec3 relativeVelocity = p_body.velocity - p_other.velocity;
		glm::vec3 support = p_point - p_body.entity.GetAbsolutePosition();
		glm::vec3 otherSupport = p_otherPoint - p_other.entity.GetAbsolutePosition();

		p_body.entity.Translate(p_normal * p_body._mass / ( p_body._mass + p_other._mass)	);
		p_other.entity.Translate(p_normal * -p_other._mass / (p_body._mass + p_other._mass));

		float numerator = glm::dot(efRestCoef * relativeVelocity, collisionNormal);
		glm::vec3 scaledAngularVel = glm::cross(p_body._inverseInertiaMatrix * glm::cross(support, collisionNormal), support);
		glm::vec3 otherScaledAngularVel = glm::cross(p_other._inverseInertiaMatrix * glm::cross(otherSupport, collisionNormal), otherSupport);

		float denominator = p_body._inverseMass + p_other._inverseMass + glm::dot(scaledAngularVel + otherScaledAngularVel, collisionNormal);						
		float impulse = numerator / denominator;

		p_body.AddForceInstant(collisionNormal * impulse);
		p_other.AddForceInstant(-collisionNormal * impulse);

		p_body.AddTorqueInstant(glm::cross(support, impulse * collisionNormal));
		p_other.AddTorqueInstant(-glm::cross(otherSupport, impulse * collisionNormal));
	}
}
