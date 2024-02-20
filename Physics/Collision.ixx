export module Physics.Resources:Collision;

import Common;

import <vector>;
import <glm/vec3.hpp>;

export struct Collision
{
public:
	Collision(Entity& p_entity1, Entity& p_entity2, glm::vec3 p_point1, glm::vec3 p_point2);

	Entity& entity1;
	Entity& entity2;
	glm::vec3 penetrationPoint1;
	glm::vec3 penetrationPoint2;
	glm::vec3 penetration;
	glm::vec3 penetrationMidpoint;
};