export module Graphics.Components:ModelComponent;

import Common;
import Graphics.Resources;

import <glm/mat4x4.hpp>;

import <vector>;
import <memory>;

export class ModelComponent : public Component 
{
public:
	ModelComponent(Entity& p_entity);

	glm::mat4 GetModelTransformation() const;
	
	std::vector<std::shared_ptr<Mesh>> _meshes;
};