export module Physics.Resources:BoundingBox;

import <glm/vec3.hpp>;

export struct BoundingBox
{
	glm::vec3 _center;
	glm::vec3 _extents;
};