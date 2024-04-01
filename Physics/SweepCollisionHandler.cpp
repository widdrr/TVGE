module Physics:SweepCollisionHandler;

import Physics.Resources;
import Physics.CollisionFunctions;

import <vector>;
import <ranges>;
import <algorithm>;
import <iostream>;

using namespace TVGE::Physics;

std::vector<CollisionEvent> SweepCollisionHandler::DetectCollisions()
{
	std::vector<CollisionEvent> _collisions;
	std::vector<BoundingBox> _preSort;
	std::vector<BoundingBox> _postSort;

	for (auto&& collider : _colliders) {
		if (collider.expired()) {
			continue;
		}
		collider.lock()->ApplyTransformations();

		_preSort.emplace_back(collider.lock()->GetBoundingBox());
	}



	std::ranges::sort(_colliders, [&](const std::weak_ptr<ColliderComponent>& collider1, const std::weak_ptr<ColliderComponent>& collider2) {

		if (collider1.expired()) {
			return false;
		}

		auto&& c1 = collider1.lock();

		if (collider2.expired()) {
			return true;
		}

		auto&& c2 = collider2.lock();

		auto&& box1 = c1->GetBoundingBox();
		auto&& box2 = c2->GetBoundingBox();

		return box1.min[_sortAxis] < box2.min[_sortAxis]; });

	for (auto&& collider : _colliders) {
		if (collider.expired()) {
			continue;
		}
		//collider.lock()->ApplyTransformations();

		_postSort.emplace_back(collider.lock()->GetBoundingBox());
	}

	unsigned int count = 0;
	glm::vec3 runningMean(0), runningSquareDiff(0);

	for (int i = 0; i < _colliders.size(); ++i) {
		if (_colliders[i].expired()) {
			break;
		}

		++count;
		auto&& collider1 = *_colliders[i].lock();
		auto&& box1 = collider1.GetBoundingBox();

		//Welford's singlepass variance algorithm
		glm::vec3 center = 0.5f * (box1.min + box1.max);

		glm::vec3 delta1 = center - runningMean;
		runningMean += delta1 / static_cast<float>(count);
		glm::vec3 delta2 = center - runningMean;

		runningSquareDiff += delta1 * delta2;

		for (int j = i + 1; j < _colliders.size(); ++j) {
			auto&& collider2 = *_colliders[j].lock();
			auto&& box2 = collider2.GetBoundingBox();

			if (box1.max[_sortAxis] < box2.min[_sortAxis]) {
				break;
			}

			if (CollisionFunctions::BoundingBoxOverlap(box1, box2)) {
				auto collision_opt = Intersect(collider1, collider2);
				if (collision_opt.has_value()) {
					auto&& collision = collision_opt.value();
					if (collider1.physical || collider2.physical) {
						_collisions.push_back(collision);
					}
					collider1.SendCollisionEvent(collision.entity1, collision);

					CollisionEvent collision2 = collision.GetOther();
					collider2.SendCollisionEvent(collision.entity2, collision2);
				}
			}
		}
	}

	glm::vec3 axisVariance = runningSquareDiff / static_cast<float>(count);

	_sortAxis = 0;
	if (axisVariance[0] < axisVariance[1]) {
		_sortAxis = 1;
	}
	if (axisVariance[_sortAxis] < axisVariance[2]) {
		_sortAxis = 2;
	}

	return _collisions;
}
