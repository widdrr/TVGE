export module Common;

import <glm/vec3.hpp>;
import <glm/gtc/quaternion.hpp>;

import <vector>;
import <memory>;

export class Component {
	friend class Entity;

protected:
	Entity& _entity;
	//TODO: implement the use for this -> component removal
	std::vector<Component>::size_type _index;
	Component(Entity& p_entity);
public:
	//declaring a virtual default destructor makes the hierarchy polymorphic
	virtual ~Component() = default;
};

template <class TComponent>
concept IsComponentType = std::is_base_of<Component, TComponent>::value;

export class Entity {
private:
	static unsigned short _current_id;
	unsigned short _id;

	std::vector<std::shared_ptr<Component>> _components;

public:
	//coordinates of center in 3D Space
	glm::vec3 position;
	//scaling along each axis
	glm::vec3 scaling;
	//rotation quaternion
	glm::quat rotation;

	Entity();

	//template definitions have to be defined here to save a lot of headaches
	template <IsComponentType TComponent, class... TArgs>
	const std::shared_ptr<TComponent> CreateComponent(TArgs... args) {

		auto component = std::make_shared<TComponent>(*this, std::forward<TArgs>(args)...);
		component->_index = _components.size();
		_components.push_back(component);

		return component;
	}

	template <IsComponentType TComponent>
	const std::shared_ptr<TComponent> GetComponentOfType() const {

		for (auto& component : _components) {
			auto castComponent = std::dynamic_pointer_cast<TComponent>(component);
			if (castComponent) {
				return castComponent;
			}
		}
		return nullptr;
	}

	void Scale(float p_scaleX, float p_scaleY, float p_scaleZ);
	void Rotate(float p_axisX, float p_axisY, float p_axisZ, float p_thetaDeg);
	void Translate(float p_dispX, float p_dispY, float p_dispZ);
};

