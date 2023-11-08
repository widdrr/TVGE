#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTextureCoordinates;
layout (location = 2) in vec3 inNormal;

out vec2 TextureCoordinates;
out vec3 Normal;
out vec3 FragmentPosition;

uniform mat4 glModelMatrix;
uniform mat4 glProjectionMatrix;
uniform mat4 glViewMatrix;

void main()
{
	gl_Position = glProjectionMatrix * glViewMatrix * glModelMatrix * vec4(inPosition, 1.f);
	TextureCoordinates = inTextureCoordinates;
	//TODO: investigate performance of inverse in shader (perhaps precompute per model?)
	Normal = mat3(transpose(inverse(glModelMatrix))) * inNormal;
	FragmentPosition = vec3(glModelMatrix * vec4(inPosition, 1.f));
}