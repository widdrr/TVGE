module Physics:Simulator;

import <glm/vec3.hpp>;

import <iostream>;

void Simulator::SimulateStep(float p_delta)
{
	for (auto&& wbody : _bodies) 
	{
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
		std::cerr << "Object does not have a Body Component";
		return;
	}

	_bodies.push_back(component);
}
