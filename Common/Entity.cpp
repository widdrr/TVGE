module Common;

unsigned int Entity::_current_id = 0;

Entity::Entity() :
	_id(_current_id),
	scaling(1.f),
	position(),
	rotation(glm::identity<glm::quat>()),
	_parent(nullptr)
{
	++_current_id;
}

Entity::Entity(const Entity& p_other) :
	_id(_current_id),
	scaling(p_other.scaling),
	position(p_other.position),
	rotation(p_other.rotation),
	_parent(nullptr)
{
	++_current_id;
	for (auto&& component : p_other._components) {
		component->Clone(*this);
	}
}

void Entity::Scale(float p_scaleX, float p_scaleY, float p_scaleZ) 
{
	Scale(glm::vec3(p_scaleX, p_scaleY, p_scaleZ));
}

void Entity::Scale(const glm::vec3& p_scaling) 
{
	scaling *= p_scaling;
}

void Entity::Translate(float p_dispX, float p_dispY, float p_dispZ) 
{
	Translate(glm::vec3(p_dispX, p_dispY, p_dispZ));
}

void Entity::Translate(const glm::vec3& p_translation) 
{
	position += p_translation;
}

void Entity::Rotate(float p_axisX, float p_axisY, float p_axisZ, float p_thetaDeg) 
{
	Rotate(glm::vec3(p_axisX, p_axisY, p_axisZ), p_thetaDeg);
}

void Entity::Rotate(const glm::vec3& p_axis, float p_thetaDeg) 
{
	glm::vec3 rotationAxis = glm::normalize(p_axis);
	float angleRadians = glm::radians(p_thetaDeg);
	rotation = glm::angleAxis(angleRadians, rotationAxis) * rotation;
}

void Entity::Rotate(const glm::vec3& p_rotation)
{
	glm::vec3 rotationAxis = glm::normalize(p_rotation);
	float angleRadians = glm::length(p_rotation);
	rotation = rotation * glm::angleAxis(angleRadians, rotationAxis);
}

void Entity::SetParent(const Entity& p_parent) 
{
	_parent = &p_parent;
}