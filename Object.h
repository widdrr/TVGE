#pragma once
#include "Mesh.h"
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <vector>
#include <memory>

//TODO: for now this is actually the GraphicsObject, in the future
//I should have GraphicsObject, PhysicsObject and Object
//in order to decouple the Physics from the Rendering as much as I can
class Object
{
private:
	static unsigned short _current_id;
	unsigned short _id;

	glm::vec3 _position;
	glm::quat _rotation;
	
public:
	const Mesh& mesh;
	unsigned int vao, vbo, ebo;
	Object(const Mesh& p_mesh);
	//TODO: cache the transformation and only recalculate changed parts
	//according to flags that are set;
	glm::mat4 GetModelTransformation();
};

