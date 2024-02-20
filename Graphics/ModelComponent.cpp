module Graphics.Components:ModelComponent;

import <glm/gtc/matrix_transform.hpp>;

ModelComponent::ModelComponent(Entity& p_entity) :
	Component(p_entity)
{}

glm::mat4 ModelComponent::GetModelTransformation() const
{
	//Translating object to desired position
	glm::mat4 modelTransformation = glm::translate(glm::identity<glm::mat4>(), entity.position);

	//Rotating object around 0,0
	modelTransformation = modelTransformation * glm::mat4_cast(entity.rotation);

	//scaling along main axes 
	modelTransformation = glm::scale(modelTransformation, entity.scaling);

	if (entity._parent == nullptr) {
		return modelTransformation;
	}

	auto parentComponent = entity._parent->TryGetComponentOfType<ModelComponent>();
	if (!parentComponent.expired()) {
		auto parentTransformation = parentComponent.lock()->GetModelTransformation();

		return parentTransformation * modelTransformation;
	}

	return modelTransformation;
}

std::shared_ptr<Component> ModelComponent::Clone(Entity& p_entity) const
{
	auto component = p_entity.CreateComponentOfType<ModelComponent>().lock();
	component->_meshes = _meshes;

	return component;
}