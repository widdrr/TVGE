module Physics.Resources:Collision;

Collision::Collision(Entity& p_entity1, Entity& p_entity2, glm::vec3 p_penetration):
	entity1(p_entity1),
	entity2(p_entity2),
	penetration(p_penetration)
{}