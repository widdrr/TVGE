#include "GraphicsComponent.h"
#include "Entity.h"

GraphicsComponent::GraphicsComponent(Entity& p_entity) :
	Component(p_entity),
	_vao(),
	_vbo(),
	_ebo()
{}

glm::mat4 GraphicsComponent::GetModelTransformation() const {

	//Translating object to desired position
	glm::mat4 modelTransformation = glm::translate(glm::identity<glm::mat4>(), _entity.position);

	//Rotating around a pivot
	modelTransformation = glm::translate(modelTransformation, _entity.pivot);
	modelTransformation = modelTransformation * glm::mat4_cast(_entity.rotation);
	modelTransformation = glm::translate(modelTransformation, -_entity.pivot);
	
	modelTransformation = glm::scale(modelTransformation, _entity.scaling);

	return modelTransformation;
}