#include "GraphicsComponent.h"
#include "Entity.h"

GraphicsComponent::GraphicsComponent(Entity& p_entity) :
	Component(p_entity),
	_vao(),
	_vbo(),
	_ebo()
{}

glm::mat4 GraphicsComponent::GetModelTransformation() const {

	glm::mat4 modelTransformation = glm::translate(glm::identity<glm::mat4>(), _entity.position);
	//TODO: improve quaternion usage? is there anything to improve?
	modelTransformation = modelTransformation * glm::mat4_cast(_entity.rotation);
	modelTransformation = glm::scale(modelTransformation, _entity.scaling);

	return modelTransformation;
}