export module Physics:Simulator;

import Common;
import Physics.Components;

import <vector>;
import <memory>;

export class Simulator 
{
public:
	Simulator() = default;
	void SimulateStep(float p_delta);

	void AddObject(const Entity& p_object);

private:

	std::vector<std::weak_ptr<BodyComponent>> _bodies;
};