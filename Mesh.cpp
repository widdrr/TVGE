#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& p_vertices, const std::vector<unsigned int>& p_indices, const GLenum p_mode) :
	_vertices(p_vertices),
	_indices(p_indices),
	_drawMode(p_mode)
{}

const std::vector<Vertex>& Mesh::GetVertices() const {
	
	return _vertices;
}

const std::vector<unsigned int>& Mesh::GetElements() const {
	
	return _indices;
}

const size_t Mesh::GetElementCount() const {
	
	return _indices.size();
}

const GLenum Mesh::GetDrawMode() const {
	return _drawMode;
}

