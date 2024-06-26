export module Physics.Components:BodyComponent;

import Common;
import :ColliderComponent;

import <glm/vec3.hpp>;
import <glm/mat3x3.hpp>;


namespace TVGE::Physics
{
	export class BodyComponent : public Component
	{
		friend class Simulator;
	public:
		BodyComponent(Entity& p_entity,
					  const float p_mass = 1.f,
					  glm::vec3 p_center = glm::vec3(0.f, 0.f, 0.f));

		void AddForce(glm::vec3 p_force);
		void AddForce(float p_forceX, float p_forceY, float p_forceZ);
		void AddForceInstant(glm::vec3 p_force);
		void AddForceInstant(float p_forceX, float p_forceY, float p_forceZ);
		void AddTorque(glm::vec3 p_torque);
		void AddTorque(float p_torqueX, float p_torqueY, float p_torqueZ);
		void AddTorqueInstant(glm::vec3 p_torque);
		void AddTorqueInstant(float p_torqueX, float p_torqueY, float p_torqueZ);
		float GetMass() const;
		float GetElasticity() const;
		const glm::vec3 GetAbsoluteMassCenter() const;
		void SetMass(float p_mass);
		void SetElasticity(float p_elasticity);
		void RegisterCollider();
		void UpdateInertiaMatrix();

		bool gravity = true;
		bool fixed = false;
		glm::vec3 velocity;
		glm::vec3 angularVelocity;
		

	protected:
		virtual std::shared_ptr<Component> Clone(Entity& p_entity) const override;

	private:
		virtual void Update(float p_deltaTime);

		float _mass;
		float _inverseMass;
		float _elasticity;
		glm::mat3 _inertiaMatrix;
		glm::mat3 _inverseInertiaMatrix;
		glm::vec3 _force;
		glm::vec3 _torque;
		glm::vec3 _relativeCenter;

		std::weak_ptr<ColliderComponent> _collider;
	};
}