export module Physics:Simulator;

import Common;
import Physics.Components;
import :CollisionHandler;

import <vector>;
import <memory>;

export class Simulator 
{
public:
	Simulator();
	void SimulateStep(float p_delta);
	void UpdateBodies(float p_delta);

	void AddObject(const Entity& p_object);

private:
	void ResolveCollisions(std::vector<Collision> p_collisions);

	std::vector<std::weak_ptr<BodyComponent>> _bodies;
	CollisionHandler _collisionHandler;
};