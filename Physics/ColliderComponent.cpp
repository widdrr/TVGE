module Physics.Components:ColliderComponent;

ColliderComponent::ColliderComponent(Entity& p_entity, ColliderTypes p_type, const bool p_physical) :
	Component(p_entity),
	type(p_type),
	physical(p_physical),
	_collisionCallbacks()
{}

void ColliderComponent::AddCollisionEventHandler(Callback p_callback)
{
	_collisionCallbacks.push_back(p_callback);
}

void ColliderComponent::SendCollisionEvent(Entity& p_other, const Collision& p_info)
{
	for (auto&& callback : _collisionCallbacks) {
		callback(p_other, p_info);
	}
}