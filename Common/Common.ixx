export module Common;

import <glm/vec3.hpp>;
import <glm/gtc/quaternion.hpp>;

import <vector>;
import <memory>;

export class Component 
{
	//Entity acts as a component factory
	friend class Entity;

public:
	//declaring a virtual default destructor makes the hierarchy polymorphic
	virtual ~Component() = default;

protected:
	Component(Entity& p_entity);
	Entity& _entity;
	//TODO: implement the use for this -> component removal
	std::vector<Component>::size_type _index;
	
};

template <class TComponent>
concept IsComponentType = std::is_base_of<Component, TComponent>::value;

export class Entity 
{
public:
	Entity();

	//template definitions have to be defined here to save a lot of headaches
	template <IsComponentType TComponent, class... TArgs>
	const std::weak_ptr<TComponent> CreateComponentOfType(TArgs... args)
	{

		auto component = std::make_shared<TComponent>(*this, std::forward<TArgs>(args)...);
		component->_index = _components.size();
		_components.push_back(component);

		return component;
	}

	template <IsComponentType TComponent>
	const std::weak_ptr<TComponent> TryGetComponentOfType() const
	{

		for (auto& component : _components) {
			auto castComponent = std::dynamic_pointer_cast<TComponent>(component);
			if (castComponent) {
				return castComponent;
			}
		}
		return std::weak_ptr<TComponent>();
	}

	void Scale(float p_scaleX, float p_scaleY, float p_scaleZ);
	void Scale(const glm::vec3& p_scaling);
	void Rotate(float p_axisX, float p_axisY, float p_axisZ, float p_thetaDeg);
	void Rotate(const glm::vec3& p_axis, float p_thetaDeg);
	void Translate(float p_dispX, float p_dispY, float p_dispZ);
	void Translate(const glm::vec3& p_translation);

	void SetParent(const Entity& p_parent);

	//TODO: polish the parent system implementation
	const Entity* _parent;

	//coordinates of center in 3D Space
	glm::vec3 position;
	//scaling along each axis
	glm::vec3 scaling;
	//rotation quaternion
	glm::quat rotation;

private:
	static unsigned int _current_id;
	unsigned int _id;

	//TODO: better data structure for efficient retrieval(hashmap via class name?)
	std::vector<std::shared_ptr<Component>> _components;
};


module : private;
Component::Component(Entity& p_entity) :
	_entity(p_entity),
	_index()
{}