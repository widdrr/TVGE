#pragma once
#include <vector>
#include <memory>
class Entity; // Forward declaration

class Component
{
	friend class Entity;

protected:
	Entity& _entity;
	//TODO: implement the use for this -> component removal
	std::vector<Component>::size_type _index;
	Component(Entity& p_entity);
	
};

template <class TComponent>
concept IsComponentType = std::is_base_of<Component, TComponent>::value;