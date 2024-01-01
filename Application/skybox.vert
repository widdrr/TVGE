#version 460 core

layout (location = 0) in vec3 inPosition;

out vec3 TextureCoordinates;

uniform mat4 glViewMatrix;
uniform mat4 glProjectionMatrix;

void main()
{
    TextureCoordinates = inPosition;
    gl_Position = (glProjectionMatrix * glViewMatrix * vec4(inPosition, 1.f)).xyww;
}  