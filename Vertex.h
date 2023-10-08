#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
class Vertex
{
private:
	glm::vec3 _position;
	glm::vec3 _color;

public:
	Vertex();
	Vertex(float p_posX, float p_posY, float p_posZ);
	const glm::vec3& Position();
	const glm::vec3& Color();
};

