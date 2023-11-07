module;

#include <gl/glew.h>

module Graphics.Resources:Mesh;

import :Utilities;

import <glm/geometric.hpp>;

Mesh::Mesh(const std::vector<Vertex>& p_vertices, const std::vector<unsigned int>& p_indices, const std::shared_ptr<Material>& p_material, bool genNormals) :
	_vertices(p_vertices),
	_indices(p_indices),
	_material(p_material),
	_vao(),
	_vbo(),
	_ebo()
{
	if (genNormals) {
		for (size_t i = 0; i < _indices.size() - 3; i += 3) {
			auto& Vertex1 = _vertices[_indices[i]];
			auto& Vertex2 = _vertices[_indices[i + 1]];
			auto& Vertex3 = _vertices[_indices[i + 2]];

			glm::vec3 edge1 = Vertex2._position - Vertex1._position;
			glm::vec3 edge2 = Vertex3._position - Vertex2._position;

			glm::vec3 triangleNormal = glm::cross(edge1, edge2);

			Vertex1._normal = triangleNormal;
			Vertex2._normal = triangleNormal;
			Vertex3._normal = triangleNormal;
		}
	}

	//generating VAO to store buffer data
	glGenVertexArrays(1, &_vao);

	//generating the VBO and EBO
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	//activating VAO
	glBindVertexArray(_vao);

	//copying data to VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), reinterpret_cast<void*>(const_cast<Vertex*>(_vertices.data())), GL_STATIC_DRAW);

	//copying data to EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), reinterpret_cast<void*>(const_cast<unsigned int*>(_indices.data())), GL_STATIC_DRAW);

	//TODO: refactor this to not send worthless data if object has no texture
	//setting Position attribute
	glVertexAttribPointer(VertexAttributes::Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(VertexAttributes::Position);
	//setting Texture attribute
	glVertexAttribPointer(VertexAttributes::TextureCoordinates, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(VertexAttributes::TextureCoordinates);
	//setting Normal attribute
	glVertexAttribPointer(VertexAttributes::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(VertexAttributes::Normal);

	//deactivating VAO
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glBindVertexArray(_vao);

	glDisableVertexAttribArray(VertexAttributes::Position);
	glDisableVertexAttribArray(VertexAttributes::TextureCoordinates);
	glDisableVertexAttribArray(VertexAttributes::Normal);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &_vao);
}