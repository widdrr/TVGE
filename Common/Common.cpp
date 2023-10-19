module Common;

Component::Component(Entity& p_entity) :
	_entity(p_entity),
	_index() {
}

unsigned short Entity::_current_id = 0;

Entity::Entity() :
	_id(_current_id),
	scaling(1.f),
	position(),
	rotation(glm::identity<glm::quat>()),
	pivot() {
	++_current_id;
}

template <IsComponentType TComponent, class... TArgs>
const std::shared_ptr<TComponent> Entity::CreateComponent(TArgs... args) {

	auto component = std::make_shared<TComponent>(*this, std::forward<TArgs>(args)...);
	component->_index = _components.size();
	_components.push_back(component);

	return component;
}

template <IsComponentType TComponent>
const std::shared_ptr<TComponent> Entity::GetComponentOfType() const {

	for (auto& component : _components) {
		auto castComponent = std::dynamic_pointer_cast<TComponent>(component);
		if (castComponent) {
			return castComponent;
		}
	}
	return nullptr;
}

void Entity::Scale(float p_scaleX, float p_scaleY, float p_scaleZ) {

	scaling *= glm::vec3(p_scaleX, p_scaleY, p_scaleZ);
}

void Entity::Translate(float p_dispX, float p_dispY, float p_dispZ) {

	position += glm::vec3(p_dispX, p_dispY, p_dispZ);
}

//TODO, find a way to compose pivot rotations
void Entity::Rotate(float p_axisX, float p_axisY, float p_axisZ, float p_thetaDeg,
	float p_pivotX, float p_pivotY, float p_pivotZ) {

	pivot = glm::vec3(p_pivotX, p_pivotY, p_pivotZ);
	glm::vec3 rotationAxis = glm::normalize(glm::vec3(p_axisX, p_axisY, p_axisZ));
	float angleRadians = glm::radians(p_thetaDeg);
	rotation = rotation * glm::angleAxis(angleRadians, rotationAxis);
}