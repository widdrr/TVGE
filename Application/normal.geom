#version 460 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in vec3 Normal[];

uniform mat4 glProjectionMatrix;

const float length = 0.4f;

void GenerateLine(int index)
{
    gl_Position = glProjectionMatrix * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = glProjectionMatrix * (gl_in[index].gl_Position + normalize(vec4(Normal[index], 0.0)) * length);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}  