module Physics.CollisionFunctions;

import :ContactPoints;

import <glm/gtc/epsilon.hpp>;

import <numeric>;
import <iostream>;

using namespace TVGE::Physics;

std::optional<CollisionEvent> CollisionFunctions::IntersectBox_Box(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider)
{
	const BoxColliderComponent& firstBox = static_cast<const BoxColliderComponent&>(p_firstCollider);
	const BoxColliderComponent& secondBox = static_cast<const BoxColliderComponent&>(p_secondCollider);

	glm::mat3 firstBoxAxes = firstBox.GetAxes();
	glm::mat3 secondBoxAxes = secondBox.GetAxes();
	glm::vec3 firstBoxExtents = firstBox.GetExtents();
	glm::vec3 secondBoxExtents = secondBox.GetExtents();

	glm::vec3 centerDifference = secondBox.GetCenter() - firstBox.GetCenter();
	//TODO: compute these on a per-needed basis
	glm::mat3 coefficients = glm::transpose(glm::transpose(firstBoxAxes) * secondBoxAxes);
	glm::mat3 absCoefficients = coefficients;
	absCoefficients[0] = glm::abs(absCoefficients[0]);
	absCoefficients[1] = glm::abs(absCoefficients[1]);
	absCoefficients[2] = glm::abs(absCoefficients[2]);

	float minOverlap = std::numeric_limits<float>::infinity();
	glm::vec3 axis(0);

	//Separating Axes for first box
	for (int i = 0; i < 3; ++i) {
		float firstRadius = firstBoxExtents[i];
		float secondRadius = glm::dot(secondBoxExtents, absCoefficients[i]);

		float distance = glm::abs(glm::dot(firstBoxAxes[i], centerDifference));

		if (distance > firstRadius + secondRadius) {
			return std::nullopt;
		}

		float overlap = firstRadius + secondRadius - distance;
		if (overlap < minOverlap) {
			minOverlap = overlap;
			axis = firstBoxAxes[i];
		}
	}

	//Separating Axes for second box
	for (int j = 0; j < 3; ++j) {
		float firstRadius = glm::dot(firstBoxExtents, glm::vec3(absCoefficients[0][j], absCoefficients[1][j], absCoefficients[2][j]));
		float secondRadius = secondBoxExtents[j];

		float distance = glm::abs(glm::dot(secondBoxAxes[j], centerDifference));

		if (distance > firstRadius + secondRadius) {
			return std::nullopt;
		}
		float overlap = firstRadius + secondRadius - distance;
		if (overlap < minOverlap) {
			minOverlap = overlap;
			axis = secondBoxAxes[j];
		}
	}

	//Separating Axes for edge collisions
	for (int i = 0; i < 3; ++i) {
		int i0 = (i + 1) % 3;
		int i1 = (i + 2) % 3;

		for (int j = 0; j < 3; ++j) {

			int j0 = (j + 1) % 3;
			int j1 = (j + 2) % 3;

			if (glm::epsilonEqual(glm::abs(glm::dot(firstBoxAxes[i], secondBoxAxes[j])), 1.f, EPSILON)) {
				continue;
			}

			float firstRadius =
				firstBoxExtents[i0] * absCoefficients[i1][j] +
				firstBoxExtents[i1] * absCoefficients[i0][j];

			float secondRadius =
				secondBoxExtents[j0] * absCoefficients[i][j1] +
				secondBoxExtents[j1] * absCoefficients[i][j0];

			float distance = glm::abs(
				glm::dot(coefficients[i0][j] * firstBoxAxes[i1],
						 centerDifference)
				-
				glm::dot(coefficients[i1][j] * firstBoxAxes[i0],
						 centerDifference)
			);

			if (distance > firstRadius + secondRadius) {
				return std::nullopt;
			}

			auto crossAxis = glm::cross(firstBoxAxes[i], secondBoxAxes[j]);
			float overlap = (firstRadius + secondRadius - distance) / glm::length(crossAxis);
			if (overlap < minOverlap) {
				minOverlap = overlap;
				axis = crossAxis;
			}
		}
	}

	if (minOverlap == 0.f) {
		minOverlap = EPSILON;
	}

	glm::vec3 normal = glm::normalize(axis) * minOverlap;
	if (glm::dot(normal, centerDifference) >= 0.f) {
		normal *= -1.f;
	}

	auto&& [contactPoint1, contactPoint2] = ContactPoints::GetBox_BoxContacts(firstBox, secondBox, normal);

	return CollisionEvent(p_firstCollider.entity, p_secondCollider.entity, contactPoint1, contactPoint2, normal);
}

std::optional<CollisionEvent> CollisionFunctions::IntersectSphere_Sphere(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider)
{
	const SphereColliderComponent& firstSphere = static_cast<const SphereColliderComponent&>(p_firstCollider);
	const SphereColliderComponent& secondSphere = static_cast<const SphereColliderComponent&>(p_secondCollider);

	glm::vec3 centerDifference = secondSphere.GetCenter() - firstSphere.GetCenter();
	float distance = glm::length(centerDifference);
	float radiusSum = firstSphere.GetRadius() + secondSphere.GetRadius();
	if (radiusSum > distance) {
		glm::vec3 point1 = glm::normalize(centerDifference) * firstSphere.GetRadius() + firstSphere.GetCenter();
		glm::vec3 point2 = glm::normalize(-centerDifference) * secondSphere.GetRadius() + secondSphere.GetCenter();

		return CollisionEvent(p_firstCollider.entity, p_secondCollider.entity, point1, point2, point2 - point1);
	}

	return std::nullopt;
}

std::optional<CollisionEvent> CollisionFunctions::IntersectSphere_Box(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider)
{
	const SphereColliderComponent* spherePointer;
	const BoxColliderComponent* boxPointer;
	bool swap = false;

	if (p_firstCollider.type == Sphere) {
		spherePointer = static_cast<const SphereColliderComponent*>(&p_firstCollider);
		boxPointer = static_cast<const BoxColliderComponent*>(&p_secondCollider);
	}
	else {
		swap = true;
		spherePointer = static_cast<const SphereColliderComponent*>(&p_secondCollider);
		boxPointer = static_cast<const BoxColliderComponent*>(&p_firstCollider);
	}

	const SphereColliderComponent& sphere = *spherePointer;
	const BoxColliderComponent& box = *boxPointer;

	glm::vec3 boxLocalSphere = (sphere.GetCenter() - box.GetCenter()) * box.GetAxes();

	glm::vec3 closestPointLocal = glm::clamp(boxLocalSphere, -box.GetExtents(), box.GetExtents());

	glm::vec3 vectorToPoint = closestPointLocal - boxLocalSphere;

	float distance = glm::length(vectorToPoint);
	if (sphere.GetRadius() > distance) {
		float centerInBox = 1.f;

		if (glm::epsilonEqual(distance, 0.f, EPSILON)) {
			centerInBox = -1.f;

			int extentId = 0;
			float extent = 0;
			float absDif = std::numeric_limits<float>().max();
			glm::vec3 extents = box.GetExtents();

			for (int i = 0; i < 3; ++i) {
				float posDif = glm::abs(closestPointLocal[i] - extents[i]);
				if (posDif < absDif) {
					absDif = posDif;
					extentId = i;
					extent = extents[i];
				}

				float negDif = glm::abs(closestPointLocal[i] + extents[i]);
				if (negDif < absDif) {
					absDif = negDif;
					extentId = i;
					extent = -extents[i];
				}
			}

			closestPointLocal[extentId] = extent;
		}

		glm::vec3 boxPoint = box.GetAxes() * closestPointLocal + box.GetCenter();

		glm::vec3 spherePoint = centerInBox * glm::normalize(boxPoint - sphere.GetCenter()) * sphere.GetRadius() + sphere.GetCenter();

		glm::vec3 normal = spherePoint - boxPoint;
		
		if(glm::epsilonEqual(glm::length(normal), 0.f, EPSILON)){
			normal =  vectorToPoint * 0.01f;
		}

		if (swap) {
			return CollisionEvent(box.entity, sphere.entity, boxPoint, spherePoint, normal);
		}
		return CollisionEvent(sphere.entity, box.entity, spherePoint, boxPoint, -normal);
	}

	return std::nullopt;
}

bool CollisionFunctions::BoundingBoxOverlap(const BoundingBox& b1, const BoundingBox& b2)
{
	return
		b1.min.x <= b2.max.x &&
		b1.max.x >= b2.min.x &&
		b1.min.y <= b2.max.y &&
		b1.max.y >= b2.min.y &&
		b1.min.z <= b2.max.z &&
		b1.max.z >= b2.min.z;
}
