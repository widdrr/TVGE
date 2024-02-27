module Graphics.Components:ShaderDataComponent;



ShaderDataComponent::ShaderDataComponent(Entity& p_entity) : 
	Component(p_entity)
{}

void ShaderDataComponent::SetUniformsInShader(ShaderProgram & p_shader)
{
	for (auto&& [key, value] : _uniforms) {
		
		if (typeid(value) == typeid(glm::vec3)) {
			auto realValue = std::any_cast<glm::vec3>(value);
			p_shader.SetVariable(key, realValue);
		}
		else if (typeid(value) == typeid(glm::vec4))
		{
			auto realValue = std::any_cast<glm::vec4>(value);
			p_shader.SetVariable(key, realValue);
		}
		else if (typeid(value) == typeid(glm::vec4)) {
			auto realValue = std::any_cast<glm::vec4>(value);
			p_shader.SetVariable(key, realValue);
		}
	}
}

std::shared_ptr<Component> ShaderDataComponent::Clone(Entity& p_entity) const
{
	auto component = p_entity.CreateComponentOfType<ShaderDataComponent>().lock();
	component->_uniforms = _uniforms;
	return component;
}

template<Hashable T>
void ShaderDataComponent::SetUniformVariable(const std::string_view key, const T value)
{
	_uniforms[key] = value;
}

template<Hashable T>
std::optional<T> ShaderDataComponent::GetUniformVariable(const std::string_view key)
{
	auto value = _uniforms.find(key);
	if (value == _uniforms.end()) {
		return std::nullopt;
	}

	if (typeid(*value) != typeid(T)) {
		return std::nullopt;
	}

	return std::any_cast<T>(*value);
}

