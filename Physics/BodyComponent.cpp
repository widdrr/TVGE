module Physics.Components:BodyComponent;

BodyComponent::BodyComponent(Entity& p_entity, const float p_mass)
	:
	Component(p_entity),
	mass(p_mass),
	velocity(0),
	angularVelocity(0),
	force(0)
{}

void BodyComponent::AddForce(glm::vec3 p_force)
{
	force += p_force;
}

void BodyComponent::Update(float p_deltaTime)
{
	glm::vec3 acceleration = force / mass;
	velocity += acceleration * p_deltaTime;
	_entity.Translate(velocity * p_deltaTime);

	force = glm::vec3(0);
}

