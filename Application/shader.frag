#version 460 core

in vec4 Color;
in vec2 TextureCoordinates;
in vec3 Normal;
in vec3 FragmentPosition;

out vec4 FragmentColor;

uniform sampler2D Texture;
uniform bool glHasTexture;
uniform vec3 glLightPosition;
uniform vec3 glLightColor;
uniform float glLightAmbianceStrength;
uniform float glLightDiffuseStrength;
uniform float glLightSpecularStrength;
uniform vec3 glCameraPosition;

//TODO: rename local variables to camelcase?
void main()
{
    //Ambient component of Phong shading
    vec3 ambientColor = glLightAmbianceStrength * glLightColor;

    //Is a strength variable necessary here?
    //Diffuse component of Phong shading
    vec3 normalizedNormal = normalize(Normal);
    vec3 lightDirection = normalize(glLightPosition - FragmentPosition);
    float diffuseValue = max(dot(normalizedNormal, lightDirection), 0.0);
    vec3 diffuseColor = glLightDiffuseStrength * diffuseValue * glLightColor;

    //Specular component of Phong shading
    vec3 cameraDirection = normalize(glCameraPosition - FragmentPosition);
    vec3 reflectedLightDirection = reflect(-lightDirection, normalizedNormal);
    float specularValue = pow(max(dot(cameraDirection, reflectedLightDirection), 0.0), 32);
    vec3 specularColor = glLightSpecularStrength * specularValue * glLightColor;


    vec4 shadedColor = vec4(((ambientColor + diffuseColor + specularColor) * Color.rgb),Color.a);
    
    if (glHasTexture) {
        FragmentColor = texture(Texture, TextureCoordinates) * shadedColor;
    }
    else {
        FragmentColor = shadedColor;
    }
}