module;

#include <gl/glew.h>
export module Graphics.Resources:Mesh;

import <glm/vec3.hpp>;
import <glm/vec2.hpp>;
import <glm/vec4.hpp>;

import <vector>;

export class Vertex {
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


export class Mesh
{
	friend class Renderer;
private:
	std::vector<Vertex> _vertices;
	const std::vector<unsigned int> _indices;
	const GLenum _drawMode;
	GLuint _vao, _vbo, _ebo;

	Mesh(const std::vector<Vertex>& p_vertices, const std::vector<unsigned int>& p_indices, const GLenum p_mode = GL_TRIANGLES);
public:
	const std::vector<Vertex>& GetVertices() const;
	const std::vector<unsigned int>& GetElements() const;
	const GLsizei GetElementCount() const;
	const GLenum GetDrawMode() const;

	~Mesh();
};

