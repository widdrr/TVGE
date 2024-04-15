export module Graphics.Components:SkyboxComponent;

import Common;
import Graphics.Resources;

import <memory>;

namespace TVGE::Graphics
{
	export class SkyboxComponent : public Component
	{
	public:
		SkyboxComponent(Entity& p_entity, ShaderProgram& p_shader);

		std::shared_ptr<Mesh> mesh;
		ShaderProgram& shader;
		std::shared_ptr<Cubemap> texture;

	protected:
		std::shared_ptr<Component> Clone(Entity& p_entity) const override;
	};
}