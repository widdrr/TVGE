#version 460 core

layout (location = 0) in vec3 inPosition;

uniform mat4 glModelMatrix;
uniform mat4 glViewMatrix;
uniform mat4 glProjectionMatrix;

void main()
{
	gl_Position = glProjectionMatrix * glViewMatrix * glModelMatrix * vec4(inPosition, 1.f);
}