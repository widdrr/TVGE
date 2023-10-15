#include "Entity.h"
#include "GraphicsComponent.h"

unsigned short Entity::_current_id = 0;

Entity::Entity():
	_id(_current_id),
	scaling(1.f),
	position(),
	rotation(glm::identity<glm::quat>())
{
	++_current_id;
}

template <IsComponentType TComponent, class... TArgs>
const std::shared_ptr<TComponent> Entity::CreateComponent(TArgs... args){
	
	auto component = std::make_shared<TComponent>(*this, std::forward<TArgs>(args)...);
	component->_index = _components.size();
	_components.push_back(component);

	return component;
}

//TODO, find some other way to fix this horrible mess
template const std::shared_ptr<GraphicsComponent> Entity::CreateComponent();

template <IsComponentType TComponent>
const std::shared_ptr<TComponent> Entity::GetComponentOfType() const {
	
	for (auto& component : _components) {
		auto castComponent = std::static_pointer_cast<TComponent>(component);
		if (castComponent) {
			return castComponent;
		}
	}
	return nullptr;
}

//TODO, find some other way to fix this horrible mess
template const std::shared_ptr<GraphicsComponent> Entity::GetComponentOfType() const;

void Entity::Scale(float p_scaleX, float p_scaleY, float p_scaleZ) {

	//TODO: does this ever have vanishing problems?
	scaling *= glm::vec3(p_scaleX, p_scaleY, p_scaleZ);
}

void Entity::Translate(float p_dispX, float p_dispY, float p_dispZ) {

	position += glm::vec3(p_dispX, p_dispY, p_dispZ);
}

void Entity::Rotate(float p_axisX, float p_axisY, float p_axisZ, float p_thetaDeg) {

	glm::vec3 rotationAxis = glm::normalize(glm::vec3(p_axisX, p_axisY, p_axisZ));
	float angleRadians = glm::radians(p_thetaDeg);
	rotation = rotation * glm::angleAxis(angleRadians, rotationAxis);
}

