#version 460 core

in vec4 Color;
in vec2 TextureCoordinates;

out vec4 FragmentColor;

uniform sampler2D Texture;
uniform bool glHasTexture; 

void main()
{
    if (glHasTexture) {
        FragmentColor = texture(Texture, TextureCoordinates) * Color;
    }
    else {
        FragmentColor = Color;
    }
}