#include "Vertex.h"

Vertex::Vertex() :
	_position(),
	_color(1.f)
{}

Vertex::Vertex(float p_posX, float p_posY, float p_posZ) :
	_position(p_posX, p_posY, p_posZ),
	_color(1.f)
{}

const glm::vec3& Vertex::Position(){
	return _position;
}

const glm::vec3& Vertex::Color(){
	return _color;
}
