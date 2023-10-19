export module Graphics.Components:RenderComponent;

import Common;
import Graphics.Resources;

import <glm/mat4x4.hpp>;

import <memory>;

export class RenderComponent : public Component {

	friend class Renderer;
private:
	//Renderer class should have unrestricted access to GraphicsComponents
	unsigned int _vao, _vbo, _ebo;

public:
	std::shared_ptr<Mesh> mesh;
	//TODO: texture unit support for texture blending?
	std::shared_ptr<Texture> texture;
	//TODO: store in a Material class?
	std::shared_ptr<ShaderProgram> shaderProgram;

	RenderComponent(Entity& p_entity);
	glm::mat4 GetModelTransformation() const;
};