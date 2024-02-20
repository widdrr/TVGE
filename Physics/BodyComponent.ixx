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
	void AddForce(float p_forceX, float p_forceY, float p_forceZ);
	void AddTorque(glm::vec3 p_torque);
	void AddTorque(float p_torqueX, float p_torqueY, float p_torqueZ);

	float mass;
	bool gravity = true;
	glm::vec3 velocity;
	glm::vec3 angularVelocity;

protected:
	virtual std::shared_ptr<Component> Clone(Entity& p_entity) const override;

private:
	void Update(float p_deltaTime);

	glm::vec3 force;
	glm::vec3 torque;
};