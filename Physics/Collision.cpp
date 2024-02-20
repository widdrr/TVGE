module Physics.Resources:Collision;

Collision::Collision(Entity& p_entity1, Entity& p_entity2, glm::vec3 p_point1, glm::vec3 p_point2):
	entity1(p_entity1),
	entity2(p_entity2),
	penetrationPoint1(p_point1),
	penetrationPoint2(p_point2)
{
	penetration = penetrationPoint1 - penetrationPoint2;
	penetrationMidpoint = (penetrationPoint1 + penetrationPoint2) * 0.5f;
}