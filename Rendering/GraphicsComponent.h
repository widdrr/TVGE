#pragma once

#include <Common/Component.h>
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include <gl/glew.h>
#include <memory>

class GraphicsComponent : public Component {

	friend class Renderer;
private:
	//Renderer class should have unrestricted access to GraphicsComponents
	GLuint _vao, _vbo, _ebo;

public:
	std::shared_ptr<Mesh> mesh;
	//TODO: texture unit support for texture blending?
	std::shared_ptr<Texture> texture;
	//TODO: store in a Material class?
	std::shared_ptr<ShaderProgram> shaderProgram;

	GraphicsComponent(Entity& p_entity);
	glm::mat4 GetModelTransformation() const;
};