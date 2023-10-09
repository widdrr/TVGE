#pragma once
#include "Mesh.h"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
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

	//coordinates of center in 3D Space
	glm::vec3 _position;
	//scale along each axis
	glm::vec3 _scaling;
	//rotation quaternion
	glm::quat _rotation;
	
public:
	const Mesh& mesh;
	unsigned int vao, vbo, ebo;
	Object(const Mesh& p_mesh);
	void Scale(float p_scaleX, float p_scaleY, float p_scaleZ);
	void SetScaling(float p_scaleX, float p_scaleY, float p_scaleZ);
	void Rotate(float p_axisX, float p_axisY, float p_axisZ, float p_thetaDeg);
	void SetRotation(float p_axisX, float p_axisY, float p_axisZ, float p_thetaDeg);
	void Translate(float p_dispX, float p_dispY, float p_dispZ);
	void SetPosition(float p_posX, float p_posY, float p_posZ);
	//TODO: cache the transformation and only recalculate changed parts
	//according to set flags;
	glm::mat4 GetModelTransformation() const;
};

