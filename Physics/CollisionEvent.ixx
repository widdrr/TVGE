export module Physics.Resources:CollisionEvent;

import Common;

import <vector>;
import <glm/vec3.hpp>;

export struct CollisionEvent
{
public:
	CollisionEvent(Entity& p_entity1, Entity& p_entity2, glm::vec3 p_point1, glm::vec3 p_point2, glm::vec3 p_normal);

	Entity& entity1;
	Entity& entity2;
	glm::vec3 contactPoint1;
	glm::vec3 contactPoint2;
	glm::vec3 collisionNormal;

	CollisionEvent GetOther() const;
};