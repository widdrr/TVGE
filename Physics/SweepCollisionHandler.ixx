export module Physics:SweepCollisionHandler;

import Physics.Resources;
import :CollisionHandler;


export class SweepCollisionHandler : public CollisionHandler
{
public:
	SweepCollisionHandler() = default;
	std::vector<CollisionEvent> DetectCollisions() override;

private:
	unsigned int _sortAxis = 1;
};