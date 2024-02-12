module Physics.Components:ColliderComponent;

ColliderComponent::ColliderComponent(Entity& p_entity) :
	Component(p_entity),
	_collisionCallbacks()
{}

void ColliderComponent::AddCollisionEventHandler(Callback p_callback)
{
	_collisionCallbacks.push_back(p_callback);
}

void ColliderComponent::SendCollisionEvent(const Entity& p_other)
{
	for (auto&& callback : _collisionCallbacks) {
		callback(p_other);
	}
}