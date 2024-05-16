module Common;

unsigned int Entity::_current_id = 0;

Entity::Entity() :
	_id(_current_id),
	relativeScaling(1.f),
	relativePosition(),
	relativeRotation(glm::identity<glm::quat>()),
	_parent(nullptr)
{
	++_current_id;
}

Entity::Entity(const Entity& p_other) :
	_id(_current_id),
	relativeScaling(p_other.relativeScaling),
	relativePosition(p_other.relativePosition),
	relativeRotation(p_other.relativeRotation),
	_parent(p_other._parent)
{
	++_current_id;
	for (auto&& component : p_other._components) {
		component->Clone(*this);
	}
}

unsigned int TVGE::Entity::GetId() const
{
	return _id;
}

void Entity::Scale(float p_scaleX, float p_scaleY, float p_scaleZ)
{
	Scale(glm::vec3(p_scaleX, p_scaleY, p_scaleZ));
}

void Entity::Scale(const glm::vec3& p_scaling) 
{
	relativeScaling *= p_scaling;
}

void Entity::Scale(float p_scaling)
{
	Scale(glm::vec3(p_scaling));
}

void Entity::Translate(float p_dispX, float p_dispY, float p_dispZ) 
{
	Translate(glm::vec3(p_dispX, p_dispY, p_dispZ));
}

void Entity::Translate(const glm::vec3& p_translation) 
{
	relativePosition += p_translation;
}

void Entity::Rotate(float p_axisX, float p_axisY, float p_axisZ, float p_thetaDeg) 
{
	Rotate(glm::vec3(p_axisX, p_axisY, p_axisZ), p_thetaDeg);
}

void Entity::Rotate(const glm::vec3& p_axis, float p_thetaDeg) 
{
	if(glm::epsilonEqual(p_thetaDeg, 0.f, EPSILON) || 
	   glm::epsilonEqual(glm::length2(p_axis), 0.f, EPSILON))
	{
		return;
	}
	glm::vec3 rotationAxis = glm::normalize(p_axis);
	float angleRadians = glm::radians(p_thetaDeg);
	relativeRotation = glm::angleAxis(angleRadians, rotationAxis) * relativeRotation;
}

void Entity::Rotate(const glm::vec3& p_rotation)
{
	if (glm::epsilonEqual(glm::length2(p_rotation), 0.f, EPSILON)) {
		return;
	}
	glm::vec3 rotationAxis = glm::normalize(p_rotation);
	float angleRadians = glm::length(p_rotation);
	relativeRotation = glm::angleAxis(angleRadians, rotationAxis) * relativeRotation;
}

void Entity::SetParent(const Entity& p_parent) 
{
	_parent = &p_parent;

}

void TVGE::Entity::RemoveParent()
{
	_parent = nullptr;
}

const glm::vec3 TVGE::Entity::GetAbsolutePosition() const
{
	glm::vec3 parentPosition = _parent != nullptr ? _parent->GetAbsolutePosition() : glm::vec3(0.f,0.f,0.f);
	glm::vec3 parentScaling = _parent != nullptr ? _parent->GetAbsoluteScaling() : glm::vec3(1.f,1.f,1.f);
	glm::quat parentRotation = _parent != nullptr ? _parent->GetAbsoluteRotation() : glm::identity<glm::quat>();
	return parentRotation * (relativePosition * parentScaling) + parentPosition;
}

const glm::quat TVGE::Entity::GetAbsoluteRotation() const
{
	glm::quat parentRotation = _parent != nullptr ? _parent->GetAbsoluteRotation() : glm::identity<glm::quat>();
	return relativeRotation * parentRotation;
}

const glm::vec3 TVGE::Entity::GetAbsoluteScaling() const
{
	glm::vec3 parentScaling = _parent != nullptr ? _parent->GetAbsoluteScaling() : glm::vec3(1.f,1.f,1.f);
	return relativeScaling * parentScaling;
}

