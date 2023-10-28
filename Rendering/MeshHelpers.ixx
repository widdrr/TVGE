export module MeshHelpers;

import Graphics.Resources;

import <glm/vec2.hpp>;
import <glm/vec4.hpp>;

import <vector>;

constexpr float PI = 3.141529f;

export namespace CommonMeshes {

	std::pair<std::vector<Vertex>, std::vector<unsigned int>> Circle(
		unsigned int p_radius, 
		unsigned int p_vertexCount,
		glm::vec4 p_color);
}