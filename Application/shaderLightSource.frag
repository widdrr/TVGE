#version 460 core

out vec4 FragmentColor;

uniform struct Material
{
    sampler2D mainTexture;
    bool hasTexture;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 emissiveColor;
    float shininess;
} glMaterial;

void main()
{
   FragmentColor = vec4(glMaterial.emissiveColor, 1);
}