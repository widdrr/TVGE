#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec2 inTextureCoordinates;

out vec4 Color;
out vec2 TextureCoordinates;

uniform mat4 glModelMatrix;
uniform mat4 glProjectionMatrix;

void main()
{
	gl_Position = glProjectionMatrix * glModelMatrix * vec4(inPosition, 1.f);
	Color = inColor;
	TextureCoordinates = inTextureCoordinates;
}