module;

#include <gl/glew.h>

export module Graphics.Resources:Mesh;

import :Vertex;

import <glm/vec3.hpp>;
import <glm/vec2.hpp>;
import <glm/vec4.hpp>;

import <vector>;

export class Mesh
{
	friend class Renderer;

private:
	Mesh(const std::vector<Vertex>& p_vertices, const std::vector<unsigned int>& p_indices, const GLenum p_mode = GL_TRIANGLES);
public:
	~Mesh();

	const std::vector<Vertex>& GetVertices() const;
	const std::vector<unsigned int>& GetElements() const;
	const GLsizei GetElementCount() const;
	const GLenum GetDrawMode() const;

private:
	std::vector<Vertex> _vertices;
	const std::vector<unsigned int> _indices;
	const GLenum _drawMode;
	GLuint _vao, _vbo, _ebo;
};