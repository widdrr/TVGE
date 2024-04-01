export module Physics.Resources:BoundingBox;

import <glm/vec3.hpp>;

namespace TVGE::Physics
{
	export struct BoundingBox
	{
		glm::vec3 min;
		glm::vec3 max;
	};
}