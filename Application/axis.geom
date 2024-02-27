#version 460 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

uniform mat4 glProjectionMatrix;
uniform mat4 glViewMatrix;
uniform mat4 glModelMatrix;

out vec3 Color;

const float length = 1.f;

void GenerateAxis(vec3 axis)
{
    gl_Position = glProjectionMatrix * glViewMatrix * glModelMatrix * vec4(0.f, 0.f, 0.f, 1.f);
    Color = axis;
    EmitVertex();
    vec4 transformedAxis = length * normalize(glModelMatrix * vec4(axis, 0.f));
    vec4 translation = glModelMatrix[3];
    gl_Position = glProjectionMatrix * glViewMatrix * (glModelMatrix * vec4(axis, 1.f) + transformedAxis + translation);
    Color = axis;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateAxis(vec3(1.f, 0.f, 0.f));
    GenerateAxis(vec3(0.f, 1.f, 0.f));
    GenerateAxis(vec3(0.f, 0.f, 1.f));
}  