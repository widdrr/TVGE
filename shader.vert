#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

out vec4 Color;


void main()
{
	gl_Position = vec4(inPosition, 1.f);
	Color = vec4(inColor, 1.f);
}