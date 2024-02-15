module Physics:Simulator;

import <glm/vec3.hpp>;

import <iostream>;

Simulator::Simulator() :
	_collisionHandler()
{}

void Simulator::SimulateStep(float p_delta)
{
	UpdateBodies(p_delta);
	auto collisions = _collisionHandler.DetectCollisions();
	for (auto&& collision : collisions) {
	}
}

void Simulator::UpdateBodies(float p_delta)
{
	for (auto&& wbody : _bodies) {
		if (wbody.expired()) {
			continue;
		}
		auto body = wbody.lock();

		body->Update(p_delta);
	}
}

void Simulator::AddObject(const Entity& p_object)
{
	//obtaining the GraphicsComponent of the Entity
	auto component = p_object.TryGetComponentOfType<BodyComponent>();
	if (component.expired()) {
		std::cerr << "Object does not have a Body Component\n";
	}
	else {
		_bodies.push_back(component);
	}

	auto collider = p_object.TryGetComponentOfType<ColliderComponent>();
	if (!collider.expired()) {
		_collisionHandler.AddCollider(collider);
	}
}