export module Physics.Components:BodyComponent;

import Common;

import <glm/vec3.hpp>;

export class BodyComponent : public Component 
{
	friend class Simulator;
public:
	BodyComponent(Entity& p_entity,
				  const float p_mass = 1.f);

	void AddForce(glm::vec3 p_force);
	void AddTorque(glm::vec3 p_torque);

	float mass;
	glm::vec3 velocity;
	glm::vec3 angularVelocity;
	glm::vec3 force;

private:
	void Update(float p_deltaTime);
};