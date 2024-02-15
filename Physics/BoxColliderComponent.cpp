module Physics.Components:BoxColliderComponent;


BoxColliderComponent::BoxColliderComponent(Entity& p_entity, glm::vec3 p_extents, glm::vec3 p_center, glm::mat3 p_axes) :
	ColliderComponent(p_entity, Box),
	_localExtents(p_extents),
	_localCenter(p_center),
	_localAxes(p_axes)
{
	_localExtents[0] = glm::abs(_localExtents[0]);
	_localExtents[1] = glm::abs(_localExtents[1]);
	_localExtents[2] = glm::abs(_localExtents[2]);
}

void BoxColliderComponent::ApplyTransformations()
{
	_center = _localCenter + _entity.position;
	
	_extents = _localExtents * _entity.scaling;
	_extents[0] = glm::abs(_extents[0]);
	_extents[1] = glm::abs(_extents[1]);
	_extents[2] = glm::abs(_extents[2]);

	_axes = glm::mat3_cast(_entity.rotation) * _localAxes;
}

std::optional<Collision> BoxColliderComponent::Intersects(const BoxColliderComponent& p_other) const
{
	auto centerDifference = p_other._center - _center;
	auto coefficients = glm::transpose(_axes) * p_other._axes;
	auto absCoefficients = coefficients;
	absCoefficients[0] = glm::abs(absCoefficients[0]);
	absCoefficients[1] = glm::abs(absCoefficients[1]);
	absCoefficients[2] = glm::abs(absCoefficients[2]);

	//Separating Axes for first box
	for (int i = 0; i < 2; ++i) {
		auto firstRadius = _extents[i];
		auto secondRadius = glm::dot(p_other._extents, absCoefficients[i]);

		auto distance = glm::abs(glm::dot(_axes[i], centerDifference));

		if (distance > firstRadius + secondRadius) {
			return std::nullopt;
		}
	}

	//Separating Axes for second box
	for (int i = 0; i < 2; ++i) {
		auto firstRadius = glm::dot(_extents, glm::vec3(absCoefficients[0][i], absCoefficients[1][i], absCoefficients[2][i]));
		auto secondRadius = p_other._extents[i];

		auto distance = glm::abs(glm::dot(p_other._axes[i], centerDifference));

		if (distance > firstRadius + secondRadius) {
			return std::nullopt;
		}
	}

	//Separating Axes for edge collisions
	for (int i = 0; i < 2; ++i) {
		auto i0 = (i + 1) % 3;
		auto i1 = (i + 2) % 3;
		
		for (int j = 0; j < 2; ++j) {

			auto j0 = (j + 1) % 3;
			auto j1 = (j + 2) % 3;

			auto firstRadius =
				_extents[i0] * absCoefficients[i1][j] +
				_extents[i1] * absCoefficients[i0][j];

			auto secondRadius =
				p_other._extents[j0] * absCoefficients[i][j1] +
				p_other._extents[j1] * absCoefficients[i][j0];

			auto distance = glm::abs(
				glm::dot(coefficients[i0][j] * _axes[i1],
						 centerDifference)
				-
				glm::dot(coefficients[i1][j] * _axes[i0],
						 centerDifference)
			);

			if (distance > firstRadius + secondRadius) {
				return std::nullopt;
			}
		}
	}

	return Collision(_entity, p_other._entity);
}

std::optional<Collision> BoxColliderComponent::Intersects(const ColliderComponent& p_other) const
{
	if (p_other.type == Box) {
		return Intersects(static_cast<const BoxColliderComponent&>(p_other));
	}

	return std::nullopt;
};