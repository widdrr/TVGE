export module Graphics.Components:SkyboxComponent;

import Common;
import Graphics.Resources;

import <memory>;

export class SkyboxComponent : public Component {

public:
	SkyboxComponent(Entity& p_entity, ShaderProgram& p_shader) :
		Component(p_entity),
		shader(p_shader)
	{
	};

	std::shared_ptr<Mesh> mesh;
	ShaderProgram& shader;
	std::shared_ptr<Cubemap> texture;
};