#include "Object.h"

unsigned short Object::_current_id = 0;

Object::Object(const Mesh& p_mesh) :
	_id(_current_id),
	_scaling(1.f),
	_position(),
	_rotation(glm::identity<glm::quat>()),
	mesh(p_mesh),
	vao(),
	vbo(),
	ebo()
{
	++_current_id;
}

glm::mat4 Object::GetModelTransformation() const {

	glm::mat4 modelTransformation = glm::translate(glm::identity<glm::mat4>(), _position);
	//TODO: improve quaternion usage
	modelTransformation = modelTransformation * glm::mat4_cast(_rotation);
	modelTransformation = glm::scale(modelTransformation, _scaling);

	return modelTransformation;
}

void Object::Scale(float p_scaleX, float p_scaleY, float p_scaleZ) {

	//TODO: does this ever have vanishing problems?
	_scaling *= glm::vec3(p_scaleX, p_scaleY, p_scaleZ);
}

void Object::SetScaling(float p_scaleX, float p_scaleY, float p_scaleZ) {

	_scaling = glm::vec3(p_scaleX, p_scaleY, p_scaleZ);
}

void Object::Translate(float p_dispX, float p_dispY, float p_dispZ) {

	_position += glm::vec3(p_dispX, p_dispY, p_dispZ);
}

void Object::SetPosition(float p_posX, float p_posY, float p_posZ) {

	_position = glm::vec3(p_posX, p_posY, p_posZ);
}

void Object::Rotate(float p_axisX, float p_axisY, float p_axisZ, float p_thetaDeg) {

	glm::vec3 rotationAxis = glm::normalize(glm::vec3(p_axisX, p_axisY, p_axisZ));
	float angleRadians = glm::radians(p_thetaDeg);
	_rotation = _rotation * glm::angleAxis(angleRadians, rotationAxis);
}

void Object::SetRotation(float p_axisX, float p_axisY, float p_axisZ, float p_thetaDeg) {

	glm::vec3 rotationAxis = glm::normalize(glm::vec3(p_axisX, p_axisY, p_axisZ));
	float angleRadians = glm::radians(p_thetaDeg);
	_rotation = glm::angleAxis(angleRadians, rotationAxis);
}