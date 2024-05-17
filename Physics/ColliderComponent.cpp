module Physics.Components:ColliderComponent;

import <iostream>;
import :BodyComponent;

using namespace TVGE::Physics;

ColliderComponent::ColliderComponent(Entity& p_entity, ColliderTypes p_type, const bool p_physical) :
	Component(p_entity),
	type(p_type),
	physical(p_physical),
	_collisionCallbacks()
{
	auto body = p_entity.TryGetComponentOfType<BodyComponent>();
	if (!body.expired()) {
		body.lock()->RegisterCollider();
	}
}

const BoundingBox ColliderComponent::GetBoundingBox() const
{
	return _boundingBox;
}

void ColliderComponent::AddCollisionEventHandler(Callback p_callback)
{
	_collisionCallbacks.push_back(p_callback);
}

void ColliderComponent::SendCollisionEvent(Entity& p_other, const CollisionEvent p_info)
{
	for (auto&& callback : _collisionCallbacks) {
		callback(p_other, p_info);
	}
}