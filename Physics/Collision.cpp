module Physics.Resources:Collision;

Collision::Collision(Entity& p_entity1, Entity& p_entity2, glm::vec3 p_point1, glm::vec3 p_point2, glm::vec3 p_normal):
	entity1(p_entity1),
	entity2(p_entity2),
	contactPoint1(p_point1),
	contactPoint2(p_point2),
	collisionNormal(p_normal)
{}