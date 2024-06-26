export module Graphics.Components:ModelComponent;

import Common;
import Graphics.Resources;

import <glm/mat4x4.hpp>;

import <vector>;
import <memory>;

export namespace TVGE::Graphics
{
	export class ModelComponent : public Component
	{
	public:
		ModelComponent(Entity& p_entity);

		glm::mat4 GetModelTransformation() const;

		std::vector<std::weak_ptr<Mesh>> meshes;

	protected:
		std::shared_ptr<Component> Clone(Entity& p_entity) const override;
	};
}