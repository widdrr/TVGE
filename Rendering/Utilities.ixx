export module Graphics.Resources:Utilities;

import <string>;

export enum VertexAttributes {
	Position,
	Color,
	TextureCoordinates,
	Normal
};

export namespace UniformVariables {

	const std::string viewMatrix = "glViewMatrix";
	const std::string projectionMatrix = "glProjectionMatrix";
	const std::string modelMatrix = "glModelMatrix";
	const std::string hasTexture = "glHasTexture";
	const std::string lightColor = "glLightColor";
	const std::string lightPosition = "glLightPosition";
	const std::string lightAmbianceStrength = "glLightAmbianceStrength";
	const std::string lightDiffuseStrength = "glLightDiffuseStrength";
	const std::string lightSpecularStrength = "glLightSpecularStrength";
	const std::string cameraPosition = "glCameraPosition";
}