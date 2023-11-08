export module Graphics.Resources:Mesh;

import :Vertex;
import :Material;

import <glm/vec3.hpp>;
import <glm/vec2.hpp>;
import <glm/vec4.hpp>;

import <vector>;
import <memory>;

export class Mesh
{
	friend class Renderer;

private:
	Mesh(const std::vector<Vertex>& p_vertices, const std::vector<unsigned int>& p_indices, const std::shared_ptr<Material>& p_material, bool genNormals = false);
public:
	~Mesh();

	std::shared_ptr<Material> _material;

private:
	std::vector<Vertex> _vertices;
	const std::vector<unsigned int> _indices;
	

	unsigned int _vao, _vbo, _ebo;
};