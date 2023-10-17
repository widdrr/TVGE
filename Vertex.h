#pragma once
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
class Vertex
{
public:
	glm::vec3 _position;
	glm::vec4 _color;
	glm::vec2 _textureCoordinates;
	glm::vec3 _normal;

	Vertex(float p_posX = 0.f, float p_posY = 0.f, float p_posZ = 0.f,
		   float p_colR = 1.f, float p_colG = 1.f, float p_colB = 1.f, float p_colA = 1.f,
		   float p_textS = 0.f, float p_textT = 0.f,
		   float p_normX = 0.f, float p_normY = 0.f, float p_normZ = 0.f);
};

