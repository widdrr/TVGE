export module Graphics.Resources:Utilities;

import <string>;

export enum VertexAttributes 
{
	Position,
	TextureCoordinates,
	Normal
};

export namespace UniformVariables 
{
	const std::string viewMatrix = "glViewMatrix";
	const std::string projectionMatrix = "glProjectionMatrix";
	const std::string modelMatrix = "glModelMatrix";
	const std::string cameraPosition = "glCameraPosition";
	
	namespace Material 
	{
		const std::string materialMainTexture = "glMaterial.mainTexture";
		const std::string materialHasTexture = "glMaterial.hasTexture";
		const std::string materialAmbientColor = "glMaterial.ambientColor";
		const std::string materialDiffuseColor = "glMaterial.diffuseColor";
		const std::string materialSpecularColor = "glMaterial.specularColor";
		const std::string materialEmissiveColor = "glMaterial.emissiveColor";
		const std::string materialShininess = "glMaterial.shininess";
	}
	
	namespace Light 
	{
		const std::string lightPosition = "glLight.position";
		const std::string lightAmbientColor = "glLight.ambientColor";
		const std::string lightDiffuseColor = "glLight.diffuseColor";
		const std::string lightSpecularColor = "glLight.specularColor";
	}
}