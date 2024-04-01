module Physics.Components:BodyComponent;

using namespace TVGE;
using namespace TVGE::Physics;

BodyComponent::BodyComponent(Entity& p_entity, const float p_mass)
	:
	Component(p_entity),
	velocity(0),
	angularVelocity(0),
	_force(0),
	_torque(0),
	_mass(0),
	_inertiaMatrix(1),
	gravity(true)
{
	RegisterCollider();
	SetMass(p_mass);
}

void BodyComponent::AddForce(glm::vec3 p_force)
{
	_force += p_force;
}

void BodyComponent::AddForce(float p_forceX, float p_forceY, float p_forceZ)
{
	AddForce(glm::vec3(p_forceX, p_forceY, p_forceZ));
}

void BodyComponent::AddTorque(glm::vec3 p_torque)
{
	_torque += p_torque;
}

void BodyComponent::AddTorque(float p_torqueX, float p_torqueY, float p_torqueZ)
{
	AddTorque(glm::vec3(p_torqueX, p_torqueY, p_torqueZ));
}

float BodyComponent::GetMass() const
{
	return _mass;
}

void BodyComponent::SetMass(float p_mass)
{
	if (_mass != abs(p_mass)) {
		_mass = abs(p_mass);
		_inverseMass = 1.f / _mass;
		UpdateInertiaMatrix();
	}
}

void BodyComponent::RegisterCollider()
{
	auto collider = entity.TryGetComponentOfType<ColliderComponent>();
	if (!collider.expired()) {
		_collider = collider;
	}
}

void BodyComponent::UpdateInertiaMatrix()
{
	if (_collider.expired()) {
		_inertiaMatrix = glm::mat3(0.4f * _mass);
		_inverseInertiaMatrix = glm::inverse(_inertiaMatrix);
		return;
	}

	auto matrix = _collider.lock()->ComputeInertiaMatrix(_mass);
	if (_inertiaMatrix != matrix) {
		_inertiaMatrix = matrix;
		_inverseInertiaMatrix = glm::inverse(_inertiaMatrix);
	}
}

std::shared_ptr<Component> BodyComponent::Clone(Entity& p_entity) const
{
	auto component = p_entity.CreateComponentOfType<BodyComponent>(_mass).lock();
	component->velocity = velocity;
	component->angularVelocity = angularVelocity;
	component->_force = _force;
	component->_torque = _torque;
	component->gravity = gravity;

	return component;
}

void BodyComponent::Update(float p_deltaTime)
{
	glm::vec3 acceleration = _force * _inverseMass;
	glm::vec3 displacement = velocity * p_deltaTime + acceleration * p_deltaTime * p_deltaTime * 0.5f;

	velocity = displacement / p_deltaTime;

	entity.Translate(displacement);

	glm::vec3 angularAcceleration = _inverseInertiaMatrix * _torque;
	glm::vec3 rotation = angularVelocity * p_deltaTime + angularAcceleration * p_deltaTime * p_deltaTime * 0.5f;
	
	angularVelocity += angularAcceleration * p_deltaTime;
	if (glm::length2(rotation) > 0) {
		entity.Rotate(rotation);
	}

	_torque = glm::vec3(0);
	_force = glm::vec3(0);
}

