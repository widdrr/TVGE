#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& p_vertices, const std::vector<unsigned int>& p_indices) :
	_vertices(p_vertices),
	_indices(p_indices) 
{}

const std::vector<Vertex>& Mesh::GetVertices() const {
	
	return _vertices;
}

const std::vector<unsigned int>& Mesh::GetElements() const {
	
	return _indices;
}

unsigned int Mesh::GetElementCount() const {
	
	return _indices.size();
}

