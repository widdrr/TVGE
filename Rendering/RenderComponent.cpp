module Graphics.Components:RenderComponent;

import <glm/gtc/matrix_transform.hpp>;

RenderComponent::RenderComponent(Entity& p_entity) :
	Component(p_entity),
	disabled(false)
{}

glm::mat4 RenderComponent::GetModelTransformation() const
{
	//Translating object to desired position
	glm::mat4 modelTransformation = glm::translate(glm::identity<glm::mat4>(), _entity.position);

	//Rotating object around 0,0
	modelTransformation = modelTransformation * glm::mat4_cast(_entity.rotation);

	//scaling along main axes 
	modelTransformation = glm::scale(modelTransformation, _entity.scaling);

	if (_entity._parent == nullptr) {
		return modelTransformation;
	}

	auto parentComponent = _entity._parent->TryGetComponentOfType<RenderComponent>();
	if (!parentComponent.expired()) {
		auto parentTransformation = parentComponent.lock()->GetModelTransformation();

		return parentTransformation * modelTransformation;
	}

	return modelTransformation;
}