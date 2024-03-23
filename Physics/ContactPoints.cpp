module Physics.CollisionFunctions:ContactPoints;

import Common;

import <glm/glm.hpp>;
import <glm/gtc/epsilon.hpp>;

import <iostream>;
import <algorithm>;

std::pair<glm::vec3, glm::vec3> ContactPoints::GetBox_BoxContacts(const BoxColliderComponent& p_firstCollider, const BoxColliderComponent& p_secondCollider, glm::vec3 p_normal)
{
	glm::vec3 normal = glm::normalize(p_normal);
	auto firstSupport = GetBoxSupports(p_firstCollider, -normal);
	auto secondSupport = GetBoxSupports(p_secondCollider, normal);

	bool swap = false;
	if (firstSupport.size() < secondSupport.size()) {
		swap = true;
		std::swap(firstSupport, secondSupport);
	}

	try {
		auto& contactFunction = contactHandlers.at({ firstSupport.size(), secondSupport.size() });
		auto&& [firstPoint, secondPoint] = contactFunction(firstSupport, secondSupport);
		if (swap) {
			std::swap(firstPoint, secondPoint);
		}

		auto firstSupport = GetBoxSupports(p_firstCollider, -p_normal);
		auto secondSupport = GetBoxSupports(p_secondCollider, p_normal);

		return { firstPoint, secondPoint };
	}
	catch (std::out_of_range) {
		std::cerr << "There is no defined collision function between provided types";
		return {};
	}


}

std::vector<glm::vec3> ContactPoints::GetBoxSupports(const BoxColliderComponent& p_collider, glm::vec3 p_normal)
{
	auto&& boxAxes = p_collider.GetAxes();
	auto&& boxExtents = p_collider.GetExtents();

	glm::vec3 normal = glm::normalize(p_normal * boxAxes);

	std::vector<glm::vec3> support;

	for (int i = 0; i < 3; ++i) {
		glm::vec3 axis(0);
		axis[i] = 1;

		float dot = glm::dot(axis, normal);
		if (glm::epsilonEqual(glm::abs(dot), 1.f, EPSILON)) {
			float sign = dot < 0 ? -1.0 : 1.0;

			static constexpr float signs[4][2] = {
				{-1.0, -1.0},
				{1.0, -1.0},
				{1.0, 1.0},
				{-1.0, 1.0}
			};

			for (int j = 0; j < 4; ++j) {
				glm::vec3 point(0);
				point[i] = boxExtents[i];

				int i0 = (i + 1) % 3;
				int i1 = (i + 2) % 3;

				point[i0] = signs[j][0] * boxExtents[i0];
				point[i1] = signs[j][1] * boxExtents[i1];

				support.push_back(boxAxes * sign * point + p_collider.GetCenter());
			}

			if (sign == -1.f) {
				std::swap(support[0], support[2]);
				std::swap(support[1], support[3]);
			}

			return support;
		}
	}

	for (int i = 0; i < 3; ++i) {
		glm::vec3 axis(0);
		axis[i] = 1;

		float dot = glm::dot(axis, normal);
		if (glm::epsilonEqual(glm::abs(dot), 0.f, EPSILON)) {
			glm::vec3 point = -boxExtents;

			int i0 = (i + 1) % 3;
			int i1 = (i + 2) % 3;

			if (normal[i0] >= 0.f) {
				point[i0] *= -1.f;
			}

			if (normal[i1] >= 0.f) {
				point[i1] *= -1.f;
			}

			support.push_back(boxAxes * point + p_collider.GetCenter());
			point[i] *= -1.f;
			support.push_back(boxAxes * point + p_collider.GetCenter());

			return support;
		}
	}

	glm::vec3 point = boxExtents;

	if (normal.x < 0.f) {
		point.x *= -1.f;
	}

	if (normal.y < 0.f) {
		point.y *= -1.f;
	}

	if (normal.z < 0.f) {
		point.z *= -1.f;
	}

	support.push_back(boxAxes * point + p_collider.GetCenter());

	return support;
}

std::pair<glm::vec3, glm::vec3> ContactPoints::GetFace_FaceContacts(std::vector<glm::vec3> p_firstSupport, std::vector<glm::vec3> p_secondSupport)
{
	std::vector<glm::vec3> clippedPoints = p_firstSupport;

	//normal for the face we're using to clip
	glm::vec3 faceNormal = glm::normalize(glm::cross(p_secondSupport[1] - p_secondSupport[0], p_secondSupport[2] - p_secondSupport[1]));
	float faceDistance = glm::dot(faceNormal, p_secondSupport[0]);

	float debug1, debug2;

	for (int i = 0; i < p_secondSupport.size(); ++i) {
		bool isEdge = clippedPoints.size() == 2;

		std::vector<glm::vec3> nextClippedPoints;
		int iPrev = (i - 1) % p_secondSupport.size();

		glm::vec3 clipEdgeEnd1 = p_secondSupport[i];
		glm::vec3 clipEdgeEnd2 = p_secondSupport[iPrev];

		//normal for the clip plane described by the edge
		//special case for rectangular faces
		//TODO: don't forget to implement the generic case when doing general convex faces
		glm::vec3 clipNormal = glm::normalize(clipEdgeEnd1 - clipEdgeEnd2);
		float clipDistance = glm::dot(clipNormal, clipEdgeEnd1);

		for (int j = 0; j < clippedPoints.size(); ++j) {
			int jNext = (j + 1) % clippedPoints.size();

			glm::vec3 testEdgeEnd1 = clippedPoints[j];
			glm::vec3 testEdgeEnd2 = clippedPoints[jNext];

			float dist1 = GeometryHelpers::DistanceToPlane(testEdgeEnd1, clipNormal, clipDistance);
			float dist2 = GeometryHelpers::DistanceToPlane(testEdgeEnd2, clipNormal, clipDistance);

			debug1 = dist1;
			debug2 = dist2;

			if (dist1 <= 0) {
				nextClippedPoints.push_back(testEdgeEnd1);
			}

			//if we only have an edge to clip, avoid clipping it twice
			if (j != 0 && isEdge) {
				continue;
			}

			if (dist1 * dist2 < 0) {
				glm::vec3 testEdge = testEdgeEnd2 - testEdgeEnd1;

				float interp = -(glm::dot(clipNormal, testEdgeEnd1) - clipDistance) / glm::dot(clipNormal, testEdge);
				glm::vec3 clipPoint = testEdgeEnd1 + testEdge * interp;
				nextClippedPoints.push_back(clipPoint);
			}
		}

		clippedPoints = nextClippedPoints;
	}

	glm::vec3 firstPoint(0);
	glm::vec3 secondPoint(0);

	for (auto&& point : clippedPoints) {
		firstPoint += point;
		
		float dist = GeometryHelpers::DistanceToPlane(point, faceNormal, faceDistance);
		glm::vec3 closestPoint = point - faceNormal * dist;
		secondPoint += closestPoint;
	}
	firstPoint /= clippedPoints.size();
	secondPoint /= clippedPoints.size();

	return { firstPoint, secondPoint };
}

std::pair<glm::vec3, glm::vec3> ContactPoints::GetFace_EdgeContacts(std::vector<glm::vec3> p_firstSupport, std::vector<glm::vec3> p_secondSupport)
{
	//Both Face-Edge and Face-Face are resolved through Polygon Clipping
	return GetFace_FaceContacts(p_secondSupport, p_firstSupport);
}

std::pair<glm::vec3, glm::vec3> ContactPoints::GetFace_PointContacts(std::vector<glm::vec3> p_firstSupport, std::vector<glm::vec3> p_secondSupport)
{
	glm::vec3 projectionPoint = GeometryHelpers::ProjectOnPlane(p_secondSupport[0], p_firstSupport[0], p_firstSupport[1], p_firstSupport[2]);
	return { projectionPoint, p_secondSupport[0] };
}

std::pair<glm::vec3, glm::vec3> ContactPoints::GetEdge_EdgeContacts(std::vector<glm::vec3> p_firstSupport, std::vector<glm::vec3> p_secondSupport)
{
	glm::vec3 edge1 = p_firstSupport[1] - p_firstSupport[0];
	glm::vec3 edge2 = p_secondSupport[1] - p_secondSupport[0];

	//Vector coplanar with edge1 and 2 that is perpendicular to edge2
	glm::vec3 edge2Perpendicular = glm::cross(glm::cross(edge1, edge2), edge2);

	//Segments are parallel and coplanar 
	if (glm::epsilonEqual(glm::dot(edge1, edge2Perpendicular), 0.f, EPSILON)) {
		glm::vec3 midAxis = glm::normalize(edge1);

		std::vector<float> midAxisProjections = {
			glm::dot(midAxis, p_firstSupport[0]),
			glm::dot(midAxis, p_firstSupport[1]),
			glm::dot(midAxis, p_secondSupport[0]),
			glm::dot(midAxis, p_secondSupport[0]),
		};

		std::sort(midAxisProjections.begin(), midAxisProjections.end());

		float midPoint = (midAxisProjections[1] + midAxisProjections[2]) / 2.f;

		//perpendicular distance vectors 
		glm::vec3 edge1Base = p_firstSupport[0] - midAxis * glm::dot(midAxis, p_firstSupport[0]);
		glm::vec3 edge2Base = p_secondSupport[0] - midAxis * glm::dot(midAxis, p_secondSupport[0]);

		glm::vec3 point1 = edge1Base + midAxis * midPoint;
		glm::vec3 point2 = edge2Base + midAxis * midPoint;

		return { point1, point2 };
	}

	//interpolation factor for closest point on edge1 to edge2
	float interp = (glm::dot(edge2Perpendicular, p_secondSupport[0]) - glm::dot(edge2Perpendicular, p_firstSupport[0]))
		/ glm::dot(edge2Perpendicular, edge1);
	interp = glm::clamp(interp, 0.f, 1.f);

	glm::vec3 point1 = p_firstSupport[0] + edge1 * interp;
	glm::vec3 point2 = GeometryHelpers::ProjectOnLine(point1, p_secondSupport[0], p_secondSupport[1]);

	return { point1, point2 };
}

std::pair<glm::vec3, glm::vec3> ContactPoints::GetEdge_PointContacts(std::vector<glm::vec3> p_firstSupport, std::vector<glm::vec3> p_secondSupport)
{
	glm::vec3 closestPoint = GeometryHelpers::ProjectOnLine(p_secondSupport[0], p_firstSupport[0], p_firstSupport[1]);
	return { closestPoint, p_secondSupport[0] };
}

std::pair<glm::vec3, glm::vec3> ContactPoints::GetPoint_PointContacts(std::vector<glm::vec3> p_firstSupport, std::vector<glm::vec3> p_secondSupport)
{
	return { p_firstSupport[0], p_secondSupport[0] };
}
