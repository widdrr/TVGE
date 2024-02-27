module Physics:Simulator;

import <glm/vec3.hpp>;
import <glm/geometric.hpp>;

import <iostream>;

float Simulator::gravityStrength = 9.8f;
float Simulator::airDynamicFriction = 0.5f;

Simulator::Simulator() :
	_collisionHandler()
{}

void Simulator::SimulateStep(float p_delta)
{
	auto collisions = _collisionHandler.DetectCollisions();
	ResolveCollisions(collisions);
	UpdateBodies(p_delta);
}

void Simulator::UpdateBodies(float p_delta)
{
	for (auto&& wbody : _bodies) {
		if (wbody.expired()) {
			continue;
		}
		auto body = wbody.lock();
		if (body->gravity) {
			body->AddForce(glm::vec3(0.f, -1.f, 0.f) * body->_mass * gravityStrength);
			body->AddForce(-body->velocity * airDynamicFriction);
		}
		body->UpdateInertiaMatrix();
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

			ApplyCollisionDynamic(*body1, *body2, collision.contactPoint1, collision.contactPoint2, collision.collisionNormal);
		}
		else if (!wbody1.expired()) {
			auto body1 = wbody1.lock();

			ApplyCollisionStatic(*body1, collision.contactPoint1, collision.collisionNormal);
		}
		else if (!wbody2.expired()) {
			auto body2 = wbody2.lock();

			ApplyCollisionStatic(*body2, collision.contactPoint2, -1.f * collision.collisionNormal);
		}
	}
}

void Simulator::ApplyCollisionStatic(BodyComponent& p_body, glm::vec3 p_point, glm::vec3 p_normal)
{
	glm::vec3 collisionNormal = glm::normalize(p_normal);
	p_body.entity.Translate(p_normal);

	glm::vec3 support = p_point - p_body.entity.position;

	glm::vec3 scaledAngularVel = glm::cross(p_body._inverseInertiaMatrix * glm::cross(support, collisionNormal), support);

	float impulse = glm::dot(-2.f * p_body.velocity, collisionNormal) /
		(p_body._inverseMass + glm::dot(scaledAngularVel, collisionNormal));

	p_body.velocity += collisionNormal * impulse * p_body._inverseMass;
	p_body.angularVelocity += p_body._inverseInertiaMatrix * glm::cross(support, impulse * collisionNormal);
}

void Simulator::ApplyCollisionDynamic(BodyComponent& p_body, BodyComponent& p_other, glm::vec3 p_point, glm::vec3 p_otherPoint, glm::vec3 p_normal)
{
	glm::vec3 collisionNormal = glm::normalize(p_normal);

	p_body.entity.Translate(p_normal * 0.5f);
	p_other.entity.Translate(p_normal * -0.5f);

	glm::vec3 relativeVelocity = p_body.velocity - p_other.velocity;
	glm::vec3 support = p_point - p_body.entity.position;
	glm::vec3 otherSupport = p_otherPoint - p_other.entity.position;

	float numerator = glm::dot(-2.f * relativeVelocity, collisionNormal);
	glm::vec3 scaledAngularVel = glm::cross(p_body._inverseInertiaMatrix * glm::cross(support, collisionNormal), support);
	glm::vec3 otherScaledAngularVel = glm::cross(p_other._inverseInertiaMatrix *  glm::cross(otherSupport, collisionNormal), otherSupport);
	float denominator = p_body._inverseMass + p_other._inverseMass + glm::dot(scaledAngularVel + otherScaledAngularVel, collisionNormal);
						
	float impulse = numerator / denominator;

	p_body.velocity += collisionNormal * impulse * p_body._inverseMass;
	p_other.velocity -= collisionNormal * impulse * p_other._inverseMass;

	p_body.angularVelocity += p_body._inverseInertiaMatrix * glm::cross(support, impulse * collisionNormal);
	p_other.angularVelocity -= p_other._inverseInertiaMatrix * glm::cross(otherSupport, impulse * collisionNormal);
}
