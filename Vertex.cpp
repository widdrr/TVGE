#include "Vertex.h"

Vertex::Vertex() :
	_position(),
	_color(1.f)
{}

Vertex::Vertex(float p_posX = 0.f, float p_posY = 0.f, float p_posZ = 0.f, float p_colR = 1.f, float p_colG = 1.f, float p_colB = 1.f) :
	_position(p_posX, p_posY, p_posZ),
	_color(p_colR, p_colG, p_colB)
{}

const glm::vec3& Vertex::Position() {
	
	return _position;
}

const glm::vec3& Vertex::Color() {
	
	return _color;
}
