export module Physics.Resources:Collision;

import Common;

import <vector>;
import <glm/vec3.hpp>;

export struct Collision
{
public:
	Collision(Entity& p_entity1, Entity& p_entity2, glm::vec3 p_penetration);

	Entity& entity1;
	Entity& entity2;
	glm::vec3 penetration;
};