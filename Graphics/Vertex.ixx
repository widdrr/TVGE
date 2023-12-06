export module Graphics.Resources:Vertex;

import <glm/vec2.hpp>;
import <glm/vec3.hpp>;

export struct Vertex {
public:
	Vertex(float p_posX = 0.f, float p_posY = 0.f, float p_posZ = 0.f,
		float p_textS = 0.f, float p_textT = 0.f,
		float p_normX = 0.f, float p_normY = 0.f, float p_normZ = 0.f);

	glm::vec3 _position;
	glm::vec2 _textureCoordinates;
	glm::vec3 _normal;
};
