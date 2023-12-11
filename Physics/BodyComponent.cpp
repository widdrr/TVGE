module Physics.Components:BodyComponent;

import <iostream>;

BodyComponent::BodyComponent(Entity& p_entity, const float p_mass)
	:
	Component(p_entity),
	mass(p_mass),
	velocity(0),
	angularVelocity(0),
	force(0),
	torque(0)
{}

void BodyComponent::AddForce(glm::vec3 p_force)
{
	force += p_force;
}

void BodyComponent::AddForce(float p_forceX, float p_forceY, float p_forceZ)
{
	AddForce(glm::vec3(p_forceX, p_forceY, p_forceZ));
}

void BodyComponent::AddTorque(glm::vec3 p_torque)
{
	torque += p_torque;
}

void BodyComponent::AddTorque(float p_torqueX, float p_torqueY, float p_torqueZ)
{
	AddTorque(glm::vec3(p_torqueX, p_torqueY, p_torqueZ));
}

void BodyComponent::Update(float p_deltaTime)
{
	glm::vec3 acceleration = force / mass;
	glm::vec3 displacement = velocity * p_deltaTime + acceleration * p_deltaTime * p_deltaTime / 2.f;
	velocity += acceleration * p_deltaTime;
	_entity.Translate(displacement);

	glm::vec3 angularAcceleration = torque / mass;
	glm::vec3 rotation = angularVelocity * p_deltaTime + angularAcceleration * p_deltaTime * p_deltaTime / 2.f;
	angularVelocity += angularAcceleration * p_deltaTime;
	if (glm::length(rotation) > 0) {
		_entity.Rotate(rotation);
	}

	torque = glm::vec3(0);
	force = glm::vec3(0);
}

