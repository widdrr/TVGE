#include "Vertex.h"

Vertex::Vertex() :
	_position(),
	_color(1.f),
	_textureCoordinates()
{}

Vertex::Vertex(float p_posX, float p_posY, float p_posZ, 
	           float p_colR, float p_colG, float p_colB, float p_colA,
			   float p_textS, float p_textT) :
	_position(p_posX, p_posY, p_posZ),
	_color(p_colR, p_colG, p_colB, p_colA),
	_textureCoordinates(p_textS, p_textT)
{}

const glm::vec3& Vertex::Position() const {
	
	return _position;
}

const glm::vec4& Vertex::Color() const {
	
	return _color;
}

const glm::vec2& Vertex::TextureCoordinates() const {
	
	return _textureCoordinates;
}
