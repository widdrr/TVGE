module Physics.CollisionFunctions;

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

	//Separating Axes for first box
	for (int i = 0; i < 2; ++i) {
		float firstRadius = firstBoxExtents[i];
		float secondRadius = glm::dot(secondBoxExtents, absCoefficients[i]);

		float distance = glm::abs(glm::dot(firstBoxAxes[i], centerDifference));

		if (distance > firstRadius + secondRadius) {
			return std::nullopt;
		}
	}

	//Separating Axes for second box
	for (int i = 0; i < 2; ++i) {
		float firstRadius = glm::dot(firstBoxExtents, glm::vec3(absCoefficients[0][i], absCoefficients[1][i], absCoefficients[2][i]));
		float secondRadius = secondBoxExtents[i];

		float distance = glm::abs(glm::dot(secondBoxAxes[i], centerDifference));

		if (distance > firstRadius + secondRadius) {
			return std::nullopt;
		}
	}

	//Separating Axes for edge collisions
	for (int i = 0; i < 2; ++i) {
		int i0 = (i + 1) % 3;
		int i1 = (i + 2) % 3;

		for (int j = 0; j < 2; ++j) {

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
		}
	}

	return Collision(p_firstCollider.entity, p_secondCollider.entity, glm::vec3(0));
}

std::optional<Collision> CollisionFunctions::IntersectSphere_Sphere(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider)
{
	const SphereColliderComponent& firstSphere = static_cast<const SphereColliderComponent&>(p_firstCollider);
	const SphereColliderComponent& secondSphere = static_cast<const SphereColliderComponent&>(p_secondCollider);

	glm::vec3 centerDifference = secondSphere.GetCenter() - secondSphere.GetCenter();
	float distance = glm::length(centerDifference);
	float radiusSum = firstSphere.GetRadius() + secondSphere.GetRadius();
	if (radiusSum > distance) {
		float penetration = 2 * radiusSum - distance;
		return Collision(p_firstCollider.entity, p_secondCollider.entity, glm::normalize(centerDifference) * penetration);
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

	glm::vec3 boxLocalSphere = glm::transpose(box.GetAxes()) * (sphere.GetCenter() - box.GetCenter());

	glm::vec3 closestPoint = glm::clamp(boxLocalSphere, -box.GetExtents(), box.GetExtents());
	glm::vec3 vectorToPoint = closestPoint - boxLocalSphere;

	float distance = glm::length(vectorToPoint);
	if (sphere.GetRadius() > distance) {
		float penetration = sphere.GetRadius() - distance;
		glm::vec3 localPenVector = glm::normalize(vectorToPoint) * penetration;
		glm::vec3 worldPenVector = box.GetAxes() * localPenVector;
		return Collision(sphere.entity, box.entity, worldPenVector);
	}

	return std::nullopt;
}
