export module Graphics.Components:ShaderDataComponent;

import Common;
import Graphics.Resources;

import <glm/vec3.hpp>;
import <glm/mat3x3.hpp>;

import <unordered_map>;
import <string_view>;
import <any>;
import <concepts>;
import <optional>;

template <typename T>
concept Hashable = requires(T a)
{
	{ std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

export class ShaderDataComponent : public Component
{
public:
	ShaderDataComponent(Entity& p_entity);
	
	template <Hashable T>
	void SetUniformVariable(const std::string_view key,  const T value);
	
	template <Hashable T>
	std::optional<T> GetUniformVariable(const std::string_view key);

	void SetUniformsInShader(ShaderProgram& p_shader);

protected:
	std::shared_ptr<Component> Clone(Entity& p_entity) const override;

private:
	std::unordered_map<std::string_view, std::any> _uniforms;
};
