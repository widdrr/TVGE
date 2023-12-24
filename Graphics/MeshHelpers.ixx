export module MeshHelpers;

import Graphics.Resources;
import Graphics;

import <glm/vec2.hpp>;
import <glm/vec4.hpp>;

import <vector>;
import <memory>;

constexpr float PI = 3.141529f;

export namespace Common2DMeshes 
{
	std::pair<std::vector<Vertex>, std::vector<unsigned int>> Circle(
		unsigned int p_radius, 
		unsigned int p_vertexCount);
}

export namespace CommonMeshes 
{
	std::pair<std::vector<Vertex>, std::vector<unsigned int>> Cube();
}