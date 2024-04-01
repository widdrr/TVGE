export module Graphics.Resources:Utilities;

import <string_view>;
import <format>;


namespace TVGE::Graphics
{
	export enum VertexAttributes
	{
		Position,
		TextureCoordinates,
		Normal
	};

	export enum TextureUnits
	{
		Skybox,
		PointShadows,
		DirectionalShadows,
		Ambient,
		Diffuse,
		Specular
	};
}

export namespace TVGE::Graphics::UniformVariables
{
	constexpr std::string_view projectionMatrix = "glProjectionMatrix";
	constexpr std::string_view modelMatrix = "glModelMatrix";
	constexpr std::string_view viewMatrix = "glViewMatrix";
	constexpr std::string_view lightMatrix = "glLightMatrix";
	constexpr std::string_view color = "glColor";

	constexpr std::string_view cameraPosition = "glCameraPosition";
	namespace Materials
	{
		constexpr std::string_view materialHasTexture = "glMaterial.hasTexture";
		constexpr std::string_view materialAmbientMap = "glMaterial.ambientMap";
		constexpr std::string_view materialDiffuseMap = "glMaterial.diffuseMap";
		constexpr std::string_view materialSpecularMap = "glMaterial.specularMap";

		constexpr std::string_view materialAmbientColor = "glMaterial.ambientColor";
		constexpr std::string_view materialDiffuseColor = "glMaterial.diffuseColor";
		constexpr std::string_view materialSpecularColor = "glMaterial.specularColor";
		constexpr std::string_view materialEmissiveColor = "glMaterial.emissiveColor";

		constexpr std::string_view materialShininess = "glMaterial.shininess";
	}

	constexpr std::string_view skyBoxMap = "glSkyboxMap";

	namespace Lights
	{
		constexpr std::string_view lightPositionArray = "glLights[{}].position";

		constexpr std::string_view lightAmbientColorArray = "glLights[{}].ambientColor";
		constexpr std::string_view lightDiffuseColorArray = "glLights[{}].diffuseColor";
		constexpr std::string_view lightSpecularColorArray = "glLights[{}].specularColor";

		constexpr std::string_view lightAttenuationLinearArray = "glLights[{}].linearAttenuation";
		constexpr std::string_view lightAttenuationQuadraticArray = "glLights[{}].quadraticAttenuation";
		constexpr std::string_view lightAttenuationConstantArray = "glLights[{}].constantAttenuation";

		constexpr std::string_view lightCount = "glLightCount";
	}

	namespace Shadows
	{
		constexpr std::string_view hasShadows = "glShadowCasterPresent";
		constexpr std::string_view shadowCasterPosition = "glShadowCasterPosition";
		constexpr std::string_view shadowFarPlane = "glShadowFarPlane";
		constexpr std::string_view pointShadowMap = "glPointShadowMap";
		constexpr std::string_view directionalShadowMap = "glDirectionalShadowMap";
		constexpr std::string_view shadowMatricesArray = "glShadowMatrices[{}]";
	}

	const std::string InsertArrayIndex(std::string_view variable, int index)
	{
		return std::vformat(variable, std::make_format_args(index));
	}
}

