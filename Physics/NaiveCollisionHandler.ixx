export module Physics:NaiveCollisionHandler;

import :CollisionHandler;
import Physics.Components;
import Physics.Resources;
import Physics.CollisionFunctions;


export class NaiveCollisionHandler : public CollisionHandler
{
public:
	NaiveCollisionHandler() = default;
	std::vector<CollisionEvent> DetectCollisions() override;
};