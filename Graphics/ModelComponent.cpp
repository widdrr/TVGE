module Graphics.Components:ModelComponent;

import <glm/gtc/matrix_transform.hpp>;

using namespace TVGE;
using namespace TVGE::Graphics;

ModelComponent::ModelComponent(Entity& p_entity) :
	Component(p_entity)
{}

glm::mat4 ModelComponent::GetModelTransformation() const
{
	auto position = entity.GetAbsolutePosition();
	auto scaling = entity.GetAbsoluteScaling();
	auto rotation = entity.GetAbsoluteRotation();
	//Translating object to desired position
	glm::mat4 modelTransformation = glm::translate(glm::identity<glm::mat4>(), position);

	//Rotating object around 0,0
	modelTransformation = modelTransformation * glm::mat4_cast(rotation);

	//scaling along main axes 
	modelTransformation = glm::scale(modelTransformation, scaling);

	return modelTransformation;
}

std::shared_ptr<Component> ModelComponent::Clone(Entity& p_entity) const
{
	auto component = p_entity.CreateComponentOfType<ModelComponent>().lock();
	component->_meshes = _meshes;

	return component;
}