export module Physics.Resources:Collision;

import Common;

import <vector>;
import <glm/vec3.hpp>;

export struct Collision
{

public:
	Collision(Entity& p_entity1, Entity& p_entity2);

	Entity& _entity1;
	Entity& _entity2;
	std::vector<glm::vec3> _contactPoints;
};