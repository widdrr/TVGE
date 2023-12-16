#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec3 inNormal;

out vec3 Normal;

uniform mat4 glModelViewMatrix;

uniform mat3 glModelViewInverseTranspose;

void main()
{
	gl_Position = glModelViewMatrix * vec4(inPosition, 1.f);
	Normal = glModelViewInverseTranspose * inNormal;
}