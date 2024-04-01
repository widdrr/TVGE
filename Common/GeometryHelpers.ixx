export module Common:GeometryHelpers;

import <glm/vec3.hpp>;
//for some reason it only compiles with these here
//can't be bothered to troubleshoot now
import <glm/gtc/epsilon.hpp>;
import <glm/gtx/norm.hpp>;

export constexpr float EPSILON = 0.00001f;

export namespace TVGE::GeometryHelpers
{
	glm::vec3 ProjectOnLine(glm::vec3 p_point, glm::vec3 p_end1, glm::vec3 p_end2);
	glm::vec3 ProjectOnPlane(glm::vec3 p_point, glm::vec3 p_plane1, glm::vec3 p_plane2, glm::vec3 p_plane3);
	float DistanceToPlane(glm::vec3 p_point, glm::vec3 p_planeNormal, float p_planeDistance);

	glm::vec3 GetUpVector(glm::vec3 p_direction);
}