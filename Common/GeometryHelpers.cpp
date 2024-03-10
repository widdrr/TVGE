module Common:GeometryHelpers;

glm::vec3 GeometryHelpers::ProjectOnLine(glm::vec3 p_point, glm::vec3 p_end1, glm::vec3 p_end2)
{
	glm::vec3 segment = p_end2 - p_end1;
	glm::vec3 pointVector = p_point - p_end1;
	float squareLength = glm::length2(segment);

	if (glm::epsilonEqual(squareLength, 0.f, EPSILON)) {
		return p_end2;
	}

	float interp = glm::dot(segment, pointVector) / squareLength;

	return p_end1 + segment * interp;
}

glm::vec3 GeometryHelpers::ProjectOnPlane(glm::vec3 p_point, glm::vec3 p_plane1, glm::vec3 p_plane2, glm::vec3 p_plane3)
{
	glm::vec3 normal = glm::normalize(glm::cross(p_plane2 - p_plane1, p_plane3 - p_plane1));
	float planeDistance = glm::dot(normal, p_plane1);

	return p_point - normal * DistanceToPlane(p_point, normal, planeDistance);
}

float GeometryHelpers::DistanceToPlane(glm::vec3 p_point, glm::vec3 p_planeNormal, float p_planeDistance)
{
	return  glm::dot(p_planeNormal, p_point) - p_planeDistance;
}

glm::vec3 GeometryHelpers::GetUpVector(glm::vec3 p_direction)
{
	auto right = glm::cross(glm::vec3(0.f, 1.f, 0.f), p_direction);
	if (glm::epsilonEqual(glm::length2(right), 0.f, EPSILON)) {
		right = glm::cross(glm::vec3(0.f, 0.f, 1.f), p_direction);
	}

	return glm::normalize(glm::cross(p_direction, right));
}
