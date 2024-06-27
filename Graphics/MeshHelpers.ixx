export module MeshHelpers;

import Graphics;

import <glm/vec2.hpp>;
import <glm/vec4.hpp>;

import <vector>;
import <memory>;

export constexpr float PI = 3.141529f;

export namespace TVGE::Common2DMeshes 
{
	std::pair<std::vector<TVGE::Graphics::Vertex>, std::vector<unsigned int>> Circle(
		unsigned int p_radius, 
		unsigned int p_vertexCount);
}

export namespace TVGE::Common3DMeshes
{
	std::pair<std::vector<TVGE::Graphics::Vertex>, std::vector<unsigned int>> Cube(glm::vec3 textureScale = {1.f, 1.f, 1.f});
	std::pair<std::vector<TVGE::Graphics::Vertex>, std::vector<unsigned int>> CubeWithMultiFace();
}