export module Graphics.Components:LightSourceComponent;

import Common;
import :CameraComponent;
import Graphics.Resources;

import <glm/vec3.hpp>;
import <glm/vec4.hpp>;

import <memory>;

export namespace TVGE::Graphics
{
	export class LightSourceComponent : public Component
	{
	public:
		virtual void SetLightVariables(ShaderProgram& p_shaderProgram, int p_index) = 0;
		virtual glm::vec4 GetPosition() const = 0;

	protected:
		LightSourceComponent(Entity& p_entity,
							 const glm::vec3& p_ambient,
							 const glm::vec3& p_diffuse,
							 const glm::vec3& p_specular);

		const glm::vec3 _ambientColor;
		const glm::vec3 _diffuseColor;
		const glm::vec3 _specularColor;
	};

	export class PointLightComponent : public LightSourceComponent
	{
	public:
		PointLightComponent(Entity& p_entity,
							const glm::vec3& p_ambient,
							const glm::vec3& p_diffuse,
							const glm::vec3& p_specular,
							const glm::vec3& p_positionOffset = glm::vec3(0.f, 0.f, 0.f),
							const float p_quadraticAttenuation = 0.0019f,
							const float p_linearAttenuation = 0.022f,
							const float p_constantAttenuation = 1.f);

		glm::vec4 GetPosition() const override;
		void SetLightVariables(ShaderProgram& p_shaderProgram, int p_index = 0) override;

	protected:
		std::shared_ptr<Component> Clone(Entity& p_entity) const override;

	private:
		float _constantAttenuation;
		float _linearAttenuation;
		float _quadraticAttenuation;
		glm::vec3 _offset;
	};

	export class DirectionalLightComponent : public LightSourceComponent
	{
	public:
		DirectionalLightComponent(Entity& p_entity,
								  const glm::vec3& p_ambient,
								  const glm::vec3& p_diffuse,
								  const glm::vec3& p_specular);

		glm::vec4 GetPosition() const override;
		void SetLightVariables(ShaderProgram& p_shaderProgram, int p_index = 0) override;

	protected:
		std::shared_ptr<Component> Clone(Entity& p_entity) const override;

	private:
		static const glm::vec3 _defaultDirection;
	};
}