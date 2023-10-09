#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

out vec4 Color;

uniform mat4 glModelMatrix;
uniform mat4 glProjectionMatrix;

void main()
{
	gl_Position = glProjectionMatrix * glModelMatrix * vec4(inPosition, 1.f);
	Color = vec4(inColor, 1.f);
}