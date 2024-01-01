#version 460 core

layout (location = 0) in vec3 inPosition;

uniform mat4 glModelMatrix;

void main()
{
	gl_Position = glModelMatrix * vec4(inPosition, 1.f);
}