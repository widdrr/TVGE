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
	ResolveCollisions(collisions);
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
	auto body = p_object.TryGetComponentOfType<BodyComponent>();
	if (body.expired()) {
		std::cerr << "Object does not have a Body Component\n";
	}
	else {
		_bodies.push_back(body);
	}

	auto collider = p_object.TryGetComponentOfType<ColliderComponent>();
	if (!collider.expired()) {
		_collisionHandler.AddCollider(collider);
	}
}

void Simulator::ResolveCollisions(std::vector<Collision> p_collisions)
{
	for (auto&& collision : p_collisions) {
		auto wbody1 = collision.entity1.TryGetComponentOfType<BodyComponent>();
		auto wbody2 = collision.entity2.TryGetComponentOfType<BodyComponent>();

		if (!wbody1.expired() && !wbody2.expired()) {
			auto body1 = wbody1.lock();
			auto body2 = wbody2.lock();

			body1->entity.Translate(-0.5f * collision.penetration);
			body2->entity.Translate(0.5f * collision.penetration);
		}
		else if (!wbody1.expired()) {
			auto body1 = wbody1.lock();
			body1->entity.Translate(-1.f * collision.penetration);
		}
		else if (!wbody2.expired()) {
			auto body2 = wbody2.lock();
			body2->entity.Translate(1.f * collision.penetration);
		}
	}
}