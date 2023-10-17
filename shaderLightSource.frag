#version 460 core

out vec4 FragmentColor;

uniform vec3 glLightColor;

void main()
{
   FragmentColor = vec4(glLightColor, 1);
}