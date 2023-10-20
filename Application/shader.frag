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
uniform float glLightSpecularStrength;
uniform vec3 glCameraPosition;

//TODO: rename local variables to camelcase?
void main()
{
    //Ambient component of Phong shading
    vec3 AmbientColor = glLightAmbianceStrength * glLightColor;

    //Is a strength variable necessary here?
    //Diffuse component of Phong shading
    vec3 NormalizedNormal = normalize(Normal);
    vec3 LightDirection = normalize(glLightPosition - FragmentPosition);
    float DiffuseValue = max(dot(NormalizedNormal, LightDirection), 0.0);
    vec3 DiffuseColor = DiffuseValue * glLightColor;

    //Specular component of Phong shading
    vec3 CameraDirection = normalize(glCameraPosition - FragmentPosition);
    vec3 ReflectedLightDirection = reflect(-LightDirection, NormalizedNormal);
    float SpecularValue = pow(max(dot(CameraDirection, ReflectedLightDirection), 0.0), 32);
    vec3 SpecularColor = glLightSpecularStrength * SpecularValue * glLightColor;


    vec4 ShadedColor = vec4(((AmbientColor + DiffuseColor + SpecularColor) * Color.rgb),Color.a);
    
    if (glHasTexture) {
        FragmentColor = texture(Texture, TextureCoordinates) * ShadedColor;
    }
    else {
        FragmentColor = ShadedColor;
    }
}