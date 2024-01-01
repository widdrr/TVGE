#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec3 inNormal;

out vec3 Normal;

uniform mat4 glModelMatrix;
uniform mat4 glViewMatrix;

void main()
{
	gl_Position = glViewMatrix* glModelMatrix * vec4(inPosition, 1.f);
	Normal = mat3(transpose(inverse(glViewMatrix*glModelMatrix))) * inNormal;
}