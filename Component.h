#pragma once
#include <vector>

class Entity; // Forward declaration

class Component
{
protected: 
	friend class Entity;

	Entity& _entity;
	std::vector<Component>::size_type _index;
	Component(Entity& p_entity);
};

template <class TComponent>
concept IsComponentType = std::is_base_of<Component, TComponent>::value;

