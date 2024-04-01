export module Physics:NaiveCollisionHandler;

import :CollisionHandler;
import Physics.Components;
import Physics.Resources;
import Physics.CollisionFunctions;

namespace TVGE::Physics
{
	export class NaiveCollisionHandler : public CollisionHandler
	{
	public:
		NaiveCollisionHandler() = default;
		std::vector<CollisionEvent> DetectCollisions() override;
	};
}