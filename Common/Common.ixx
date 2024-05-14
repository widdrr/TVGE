export module Common;

import <glm/vec3.hpp>;
import <glm/gtc/quaternion.hpp>;

import <vector>;
import <memory>;

export import :GeometryHelpers;

namespace TVGE
{
	export class Component
	{
		//Entity acts as a component factory
		friend class Entity;

	public:
		//declaring a virtual default destructor makes the hierarchy polymorphic
		virtual ~Component() = default;
		Entity& entity;

	protected:
		Component(Entity& p_entity);
		virtual std::shared_ptr<Component> Clone(Entity& p_entity) const = 0;
		//TODO: implement the use for this -> component removal
		std::vector<Component>::size_type _index;

	};

	template <class TComponent>
	concept IsComponentType = std::is_base_of<Component, TComponent>::value;

	export class Entity
	{
	public:
		Entity();
		Entity(const Entity& p_other);

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

		unsigned int GetId() const; 

		void Scale(float p_scaleX, float p_scaleY, float p_scaleZ);
		void Scale(const glm::vec3& p_scaling);
		void Scale(float p_scaling);
		void Rotate(float p_axisX, float p_axisY, float p_axisZ, float p_thetaDeg);
		void Rotate(const glm::vec3& p_axis, float p_thetaDeg);
		void Rotate(const glm::vec3& p_rotation);
		void Translate(float p_dispX, float p_dispY, float p_dispZ);
		void Translate(const glm::vec3& p_translation);

		void SetParent(const Entity& p_parent);

		const glm::vec3 GetAbsolutePosition() const;
		const glm::quat GetAbsoluteRotation() const;
		const glm::vec3 GetAbsoluteScaling() const;

		//TODO: polish the parent system implementation
		const Entity* _parent;

		//coordinates of center in 3D Space
		glm::vec3 relativePosition;
		//scaling along each axis
		glm::vec3 relativeScaling;
		//rotation quaternion
		glm::quat relativeRotation;

	private:
		static unsigned int _current_id;
		unsigned int _id;

		//TODO: better data structure for efficient retrieval(hashmap via class name?)
		//TODO2: how to properly query for base types?
		std::vector<std::shared_ptr<Component>> _components;
	};
}

module : private;
using namespace TVGE;
Component::Component(Entity& p_entity) :
	entity(p_entity),
	_index()
{}