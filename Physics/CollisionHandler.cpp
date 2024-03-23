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

void CollisionHandler::AddCollider(std::weak_ptr<ColliderComponent> p_collider)
{
	if (p_collider.expired()) {
		std::cerr << "Collider expired\n";
		return;
	}

	_colliders.push_back(p_collider);
}

void CollisionHandler::CleanDanglingPointers()
{
	std::vector<std::weak_ptr<ColliderComponent>> validColliders;
	for (auto&& comp : _colliders) {
		if (!comp.expired()) {
			validColliders.push_back(comp);
		}
	}

	_colliders = validColliders;
}

std::optional<CollisionEvent> CollisionHandler::Intersect(const ColliderComponent& p_firstCollider, const ColliderComponent& p_secondCollider)
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