module Physics:CollisionHandler;

import <iostream>;

CollisionHandler::CollisionHandler():
	_colliders()
{}

std::vector<Collision> CollisionHandler::DetectCollisions()
{

	std::vector<Collision> _collisions;
	//if (_colliders.empty()) {
	//	return _collisions;
	//}

	for(auto && collider : _colliders)
	{
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

			auto collision_opt = collider1.Intersects(collider2);
			if (collision_opt.has_value()) {
				auto&& collision = collision_opt.value();
				_collisions.push_back(collision);
				collider1.SendCollisionEvent(collision._entity2, collision);
				collider2.SendCollisionEvent(collision._entity1, collision);
			}
		}
	}

	return _collisions;
}

void CollisionHandler::AddCollider(std::weak_ptr<ColliderComponent> p_collider)
{
	if (p_collider.expired()) {
		std::cerr << "Collider expired\n";
		return;
	}

	_colliders.push_back(p_collider);
}
