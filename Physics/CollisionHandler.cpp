module Physics:CollisionHandler;

import <iostream>;

CollisionHandler::CollisionHandler() :
	_colliders(),
	_collisionFunctions()
{
	_collisionFunctions[{Box, Box}] = CollisionFunctions::IntersectBox_Box;
	_collisionFunctions[{Sphere, Sphere}] = CollisionFunctions::IntersectSphere_Sphere;
	_collisionFunctions[{Sphere, Box}] = CollisionFunctions::IntersectSphere_Box;
	//TODO: investigate why these don't hash the same
	_collisionFunctions[{Box, Sphere}] = CollisionFunctions::IntersectSphere_Box;
}

std::vector<Collision> CollisionHandler::DetectCollisions()
{
	std::vector<Collision> _collisions;

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

			auto collision_opt = Intersect(collider1, collider2);
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

std::optional<Collision> CollisionHandler::Intersect(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider)
{
	auto types = std::make_pair(p_firstCollider.type, p_secondCollider.type);
	try {
		auto& collisionFunction = _collisionFunctions.at(types);
		return collisionFunction(p_firstCollider, p_secondCollider);
	}
	catch (std::out_of_range) {
		std::cerr << "There is no defined collision function between provided types";
		return std::nullopt;
	}

}