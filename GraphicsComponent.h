#pragma once
#include "Component.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include <gl/glew.h>
#include <memory>

class GraphicsComponent : public Component {

private:
	//Renderer class should have unrestricted access to GraphicsComponents
	friend class Renderer;
	GLuint _vao, _vbo, _ebo;

public:
	std::shared_ptr<Mesh> mesh;
	//TODO: store in a Material class?
	std::shared_ptr<ShaderProgram> shaderProgram;

	GraphicsComponent(Entity& p_entity);
	glm::mat4 GetModelTransformation() const;
};
