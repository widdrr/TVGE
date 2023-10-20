module Graphics.Resources:Mesh;

import <glm/geometric.hpp>;
import <glm/vec3.hpp>;

import <vector>;

Mesh::Mesh(const std::vector<Vertex>& p_vertices, const std::vector<unsigned int>& p_indices, const GLenum p_mode) :
	_vertices(p_vertices),
	_indices(p_indices),
	_drawMode(p_mode)
{
	//TODO: make this work for something other than triangles
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

const std::vector<Vertex>& Mesh::GetVertices() const {
	
	return _vertices;
}

const std::vector<unsigned int>& Mesh::GetElements() const {
	
	return _indices;
}

const GLsizei Mesh::GetElementCount() const {
	
	return static_cast<GLsizei>(_indices.size());
}

const GLenum Mesh::GetDrawMode() const {
	return _drawMode;
}

