export module Physics.CollisionFunctions:ContactPoints;

import Physics.Components;

import <glm/vec3.hpp>;

import <vector>;
import <functional>;

export namespace TVGE::Physics::ContactPoints
{
	using ContactFunction = std::function<std::pair<glm::vec3, glm::vec3>(std::vector<glm::vec3>, std::vector<glm::vec3>)>;

	struct PairHash final
	{
	public:
		size_t operator()(const std::pair<unsigned int, unsigned int> p) const
		{
			return std::hash<unsigned int>()(p.first) + std::hash<unsigned int>()(p.second);
		}
	};

	std::pair<glm::vec3, glm::vec3> GetBox_BoxContacts(const BoxColliderComponent& p_firstCollider, const BoxColliderComponent& p_secondCollider, glm::vec3 p_normal);
	std::vector<glm::vec3> GetBoxSupports(const BoxColliderComponent& p_collider, glm::vec3 p_normal);


	std::pair<glm::vec3, glm::vec3> GetFace_FaceContacts(std::vector<glm::vec3> p_firstSupport, std::vector<glm::vec3> p_secondSupport);
	std::pair<glm::vec3, glm::vec3> GetFace_EdgeContacts(std::vector<glm::vec3> p_firstSupport, std::vector<glm::vec3> p_secondSupport);
	std::pair<glm::vec3, glm::vec3> GetFace_PointContacts(std::vector<glm::vec3> p_firstSupport, std::vector<glm::vec3> p_secondSupport);
	std::pair<glm::vec3, glm::vec3> GetEdge_EdgeContacts(std::vector<glm::vec3> p_firstSupport, std::vector<glm::vec3> p_secondSupport);
	std::pair<glm::vec3, glm::vec3> GetEdge_PointContacts(std::vector<glm::vec3> p_firstSupport, std::vector<glm::vec3> p_secondSupport);
	std::pair<glm::vec3, glm::vec3> GetPoint_PointContacts(std::vector<glm::vec3> p_firstSupport, std::vector<glm::vec3> p_secondSupport);

	std::unordered_map<std::pair<unsigned int, unsigned int>, ContactFunction, PairHash> contactHandlers = {
		{ {4,4}, &GetFace_FaceContacts},
		{ {4,2}, &GetFace_EdgeContacts },
		{ {4,1}, &GetFace_PointContacts },
		{ {2,2}, &GetEdge_EdgeContacts },
		{ {2,1}, &GetEdge_PointContacts },
		{ {1,1}, &GetPoint_PointContacts },
	};
}