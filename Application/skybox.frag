#version 460 core

out vec4 FragmentColor;

in vec3 TextureCoordinates;

uniform samplerCube glSkyboxTexture;

void main()
{
    FragmentColor = texture(glSkyboxTexture, TextureCoordinates);
}