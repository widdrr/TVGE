#version 460 core

uniform vec3 glColor;
out vec4 FragmentColor;

void main()
{
    FragmentColor = vec4(glColor, 1.f);
}  