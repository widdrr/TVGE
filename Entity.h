#pragma once
#include "Component.h"
#include "GraphicsComponent.h"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <memory>
#include <type_traits>

class Entity
{
private:
	static unsigned short _current_id;
	unsigned short _id;

	//TODO: low priority, set ownership for this object
	//TODO: critical, troubleshoot the object slicing
	std::vector<std::shared_ptr<Component>> _components;

public:
	//coordinates of center in 3D Space
	glm::vec3 position;
	//scaling along each axis
	glm::vec3 scaling;
	//rotation quaternion
	glm::quat rotation;

	Entity();
	
	template <IsComponentType TComponent>
	void AddComponent(TComponent& p_component);
	
	template <IsComponentType TComponent> 
	const std::shared_ptr<TComponent> GetComponentOfType() const;

	void Scale(float p_scaleX, float p_scaleY, float p_scaleZ);
	void Rotate(float p_axisX, float p_axisY, float p_axisZ, float p_thetaDeg);
	void Translate(float p_dispX, float p_dispY, float p_dispZ);
};

