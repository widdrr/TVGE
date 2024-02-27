module Physics.CollisionFunctions;

import <glm/gtc/epsilon.hpp>;

import <numeric>;

constexpr float EPSILON = 0.001f;

std::optional<Collision> CollisionFunctions::IntersectBox_Box(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider)
{
	const BoxColliderComponent& firstBox = static_cast<const BoxColliderComponent&>(p_firstCollider);
	const BoxColliderComponent& secondBox = static_cast<const BoxColliderComponent&>(p_secondCollider);

	glm::mat3 firstBoxAxes = firstBox.GetAxes();
	glm::mat3 secondBoxAxes = secondBox.GetAxes();
	glm::vec3 firstBoxExtents = firstBox.GetExtents();
	glm::vec3 secondBoxExtents = secondBox.GetExtents();

	glm::vec3 centerDifference = secondBox.GetCenter() - firstBox.GetCenter();
	//TODO: compute these on a per-needed basis
	glm::mat3 coefficients = glm::transpose(firstBoxAxes) * secondBoxAxes;
	glm::mat3 absCoefficients = coefficients;
	absCoefficients[0] = glm::abs(absCoefficients[0]);
	absCoefficients[1] = glm::abs(absCoefficients[1]);
	absCoefficients[2] = glm::abs(absCoefficients[2]);

	float minOverlap = std::numeric_limits<float>::infinity();
	int axisType = 0;
	int iAxis = 0, jAxis = 0;
	int sigma = 0;

	//Separating Axes for first box
	for (int i = 0; i < 3; ++i) {
		float firstRadius = firstBoxExtents[i];
		float secondRadius = glm::dot(secondBoxExtents, absCoefficients[i]);

		float distance = glm::abs(glm::dot(firstBoxAxes[i], centerDifference));

		if (distance > firstRadius + secondRadius) {
			return std::nullopt;
		}

		float overlap = firstRadius + secondRadius - distance;
		if (overlap != 0.f && overlap < minOverlap) {
			minOverlap = overlap;
			axisType = 0;
			iAxis = i;
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
		if (overlap != 0.f && overlap < minOverlap) {
			minOverlap = overlap;
			axisType = 1;
			jAxis = j;
		}
	}

	//Separating Axes for edge collisions
	for (int i = 0; i < 3; ++i) {
		int i0 = (i + 1) % 3;
		int i1 = (i + 2) % 3;

		for (int j = 0; j < 3; ++j) {

			int j0 = (j + 1) % 3;
			int j1 = (j + 2) % 3;

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

			float overlap = firstRadius + secondRadius - distance;
			if (overlap != 0.f && overlap < minOverlap) {
				minOverlap = overlap;
				axisType = 2;
				iAxis = i;
				jAxis = j;
			}
		}
	}

	glm::vec3 contactPoint(0);
	glm::vec3 axis(0);

	switch (axisType) {
		case 0:
			axis = firstBoxAxes[iAxis];
			minOverlap /= glm::dot(axis, axis);
			sigma = glm::dot(axis, centerDifference) > 0 ? 1.f : -1.f;

			for (int j = 0; j < 3; ++j) {
				float coefficient = 0;
				if (glm::epsilonNotEqual(coefficients[iAxis][j], 0.f, EPSILON)) {
					coefficient = -1.f * sigma * glm::sign(coefficients[iAxis][j]) * secondBoxExtents[j];
				}
				else {
					//auto indices = { 0, 1, 2 };
					//float sum = std::reduce(indices.begin(), indices.end(), 0.f, [&](int acc, int k) {
					//	return acc + absCoefficients[k][j] * firstBoxExtents[k]; });

					//float dot = glm::dot(-secondBoxAxes[j], centerDifference);
					//float minCoeff = dot - sum;
					//float maxCoeff = dot + sum;

					//if (secondBoxExtents[j] <= maxCoeff) {
					//	coefficient = secondBoxExtents[j];
					//}
					//else if(-secondBoxExtents[j] >= minCoeff) {
					//	coefficient = -secondBoxExtents[j];
					//}
					//else {
					//	coefficient = minCoeff;
					//}
					coefficient = 0.f;
				}
				contactPoint += coefficient * secondBoxAxes[j];
			}
			contactPoint += secondBox.GetCenter();

			break;
		case 1:
			axis = secondBoxAxes[jAxis];
			minOverlap /= glm::dot(axis, axis);
			sigma = glm::dot(axis, centerDifference) > 0 ? 1.f : -1.f;

			for (int i = 0; i < 3; ++i) {
				float coefficient = 0;
				if (glm::epsilonNotEqual(coefficients[i][jAxis], 0.f, EPSILON)) {
					coefficient = sigma * glm::sign(coefficients[i][jAxis]) * firstBoxExtents[i];
				}
				else {
					//auto indices = { 0, 1, 2 };
					//float sum = std::reduce(indices.begin(), indices.end(), 0.f, [&](int acc, int k) {
					//	return acc + absCoefficients[i][k] * secondBoxExtents[k]; });

					//float dot = glm::dot(-firstBoxAxes[i], centerDifference);
					//float minCoeff = dot - sum;
					//float maxCoeff = dot + sum;

					//if (firstBoxExtents[i] <= maxCoeff) {
					//	coefficient = firstBoxExtents[i];
					//}
					//else if (-firstBoxExtents[i] >= minCoeff) {
					//	coefficient = -firstBoxExtents[i];
					//}
					//else {
					//	coefficient = minCoeff;
					//}
					coefficient = 0.f;
				}
				contactPoint += coefficient * firstBoxAxes[i];
			}
			contactPoint += firstBox.GetCenter();

			break;
		case 2:

			axis = glm::cross(firstBoxAxes[iAxis], secondBoxAxes[jAxis]);
			minOverlap /= glm::dot(axis, axis);
			sigma = glm::dot(axis, centerDifference) > 0 ? 1.f : -1.f;

			glm::vec3 xCoefficients(0);
			glm::vec3 yCoefficients(0);

			int i0 = (iAxis + 1) % 3;
			int i1 = (iAxis + 2) % 3;

			int j0 = (jAxis + 1) % 3;
			int j1 = (jAxis + 2) % 3;

			xCoefficients[i0] = -1.f * sigma * glm::sign(coefficients[i1][jAxis]) * firstBoxExtents[i0];
			xCoefficients[i1] = sigma * glm::sign(coefficients[i0][jAxis]) * firstBoxExtents[i1];

			yCoefficients[j0] = -1.f * sigma * glm::sign(coefficients[iAxis][j1]) * secondBoxExtents[j0];
			yCoefficients[j1] = sigma * glm::sign(coefficients[iAxis][j0]) * secondBoxExtents[j1];

			float fraction = 1.f / (1.f - coefficients[iAxis][jAxis] * coefficients[iAxis][jAxis]);
			float innerParanthesis =
				-1.f * glm::dot(secondBoxAxes[jAxis], centerDifference) +
				coefficients[i0][jAxis] * xCoefficients[i0] +
				coefficients[i1][jAxis] * xCoefficients[i1];
			float outerParanthesis =
				glm::dot(firstBoxAxes[iAxis], centerDifference) +
				coefficients[iAxis][jAxis] * innerParanthesis +
				coefficients[iAxis][j0] * yCoefficients[j0] +
				coefficients[iAxis][j1] * yCoefficients[j1];

			xCoefficients[iAxis] = fraction * outerParanthesis;

			for (int i = 0; i < 3; ++i) {
				contactPoint += xCoefficients[i] * firstBoxAxes[i];
			}
			contactPoint += firstBox.GetCenter();
	}

	glm::vec3 normal = glm::normalize(axis) * minOverlap;
	if (glm::dot(normal, centerDifference) >= 0.f) {
		normal *= -1.f;
	}
	return Collision(p_firstCollider.entity, p_secondCollider.entity, contactPoint, contactPoint, normal);
}

std::optional<Collision> CollisionFunctions::IntersectSphere_Sphere(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider)
{
	const SphereColliderComponent& firstSphere = static_cast<const SphereColliderComponent&>(p_firstCollider);
	const SphereColliderComponent& secondSphere = static_cast<const SphereColliderComponent&>(p_secondCollider);

	glm::vec3 centerDifference = secondSphere.GetCenter() - firstSphere.GetCenter();
	float distance = glm::length(centerDifference);
	float radiusSum = firstSphere.GetRadius() + secondSphere.GetRadius();
	if (radiusSum > distance) {
		glm::vec3 point1 = glm::normalize(centerDifference) * firstSphere.GetRadius() + firstSphere.GetCenter();
		glm::vec3 point2 = glm::normalize(-centerDifference) * secondSphere.GetRadius() + secondSphere.GetCenter();

		return Collision(p_firstCollider.entity, p_secondCollider.entity, point1, point2, point2 - point1);
	}

	return std::nullopt;
}

std::optional<Collision> CollisionFunctions::IntersectSphere_Box(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider)
{
	const SphereColliderComponent* spherePointer;
	const BoxColliderComponent* boxPointer;

	if (p_firstCollider.type == Sphere) {
		spherePointer = static_cast<const SphereColliderComponent*>(&p_firstCollider);
		boxPointer = static_cast<const BoxColliderComponent*>(&p_secondCollider);
	}
	else {
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
		glm::vec3 boxPoint = box.GetAxes() * closestPointLocal + box.GetCenter();
		glm::vec3 spherePoint = glm::normalize(boxPoint - sphere.GetCenter()) * sphere.GetRadius() + sphere.GetCenter();
		return Collision(sphere.entity, box.entity, spherePoint, boxPoint, boxPoint - spherePoint);
	}

	return std::nullopt;
}
