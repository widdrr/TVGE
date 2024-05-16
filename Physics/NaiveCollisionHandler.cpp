module Physics:NaiveCollisionHandler;

import <vector>;

using namespace TVGE;
using namespace TVGE::Physics;

std::vector<CollisionEvent> NaiveCollisionHandler::DetectCollisions()
{
	std::vector<CollisionEvent> _collisions;

	for (auto&& collider : _colliders) {
		if (collider.expired()) {
			continue;
		}
		collider.lock()->ApplyTransformations();
	}

	for (int i = 0; i < static_cast<int>(_colliders.size()) - 1; ++i) {
		for (int j = i + 1; j < _colliders.size(); ++j) {
			if (_colliders[i].expired()) {
				break;
			}
			if (_colliders[j].expired()) {
				continue;
			}

			auto& collider1 = *_colliders[i].lock();
			auto& collider2 = *_colliders[j].lock();

			//std::thread collisionThread(Intersect, collider1, collider2);
			auto collision_opt = Intersect(collider1, collider2);
			if (collision_opt.has_value()) {
				auto&& collision = collision_opt.value();
				if (collider1.physical && collider2.physical) {
					_collisions.push_back(collision);
				}
				collider1.SendCollisionEvent(collision.entity2, collision);

				CollisionEvent collision2 = collision.GetOther();
				collider2.SendCollisionEvent(collision.entity1, collision2);
			}
		}
	}

	return _collisions;
}
