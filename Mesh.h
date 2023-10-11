#pragma once
#include "Vertex.h"
#include <vector>

class Mesh
{
private:
	const std::vector<Vertex> _vertices;
	const std::vector<unsigned int> _indices;

public:
	Mesh(const std::vector<Vertex>& p_vertices, const std::vector<unsigned int>& p_indices);
	//TODO: have a mechanism so the getters return readonly vectors
	//currently, the actual vertices can be modified
	const std::vector<Vertex>& GetVertices() const;
	const std::vector<unsigned int>& GetElements() const;
	unsigned int GetElementCount() const;
};

