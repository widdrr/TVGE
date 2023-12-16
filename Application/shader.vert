#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTextureCoordinates;
layout (location = 2) in vec3 inNormal;

out vec2 TextureCoordinates;
out vec3 Normal;
out vec3 FragmentPosition;

uniform mat4 glModelViewMatrix;
uniform mat4 glProjectionMatrix;

uniform mat3 glModelViewInverseTranspose;

void main()
{
	gl_Position = glProjectionMatrix * glModelViewMatrix * vec4(inPosition, 1.f);
	TextureCoordinates = inTextureCoordinates;
	Normal = glModelViewInverseTranspose * inNormal;
	FragmentPosition = vec3(glModelViewMatrix * vec4(inPosition, 1.f));
}