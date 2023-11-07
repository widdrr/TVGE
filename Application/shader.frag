#version 460 core

in vec2 TextureCoordinates;
in vec3 Normal;
in vec3 FragmentPosition;

out vec4 FragmentColor;

uniform vec3 glCameraPosition;

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

uniform struct LightSource
{
    vec3 position;
    vec3 direction;
    float radius;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
} glLight;

vec3 ComputeAmbientColor(){

    return glMaterial.ambientColor * glLight.ambientColor;
}

vec3 ComputeDiffuseColor(vec3 normal, vec3 lightDirection){

    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    return glMaterial.diffuseColor * diffuseValue * glLight.diffuseColor;
}

vec3 ComputeSpecularColor(vec3 normal, vec3 lightDirection){

    vec3 cameraDirection = normalize(glCameraPosition - FragmentPosition);
    vec3 reflectedLightDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(dot(cameraDirection, reflectedLightDirection), 0.0), glMaterial.shininess);
    return glMaterial.specularColor * specularValue * glLight.specularColor;
}

void main()
{
    vec3 lightDirection = normalize(glLight.position - FragmentPosition);
    vec3 normalizedNormal = normalize(Normal);
    //Ambient component of Phong shading
    vec3 ambientColor = ComputeAmbientColor();

    //Diffuse component of Phong shading
    vec3 diffuseColor = ComputeDiffuseColor(normalizedNormal, lightDirection);

    //Specular component of Phong shading
    vec3 specularColor = ComputeSpecularColor(normalizedNormal, lightDirection);

    vec4 shadedColor = vec4(((ambientColor + diffuseColor + specularColor)), 1.f);
    
    if (glMaterial.hasTexture) {
        FragmentColor = texture(glMaterial.mainTexture, TextureCoordinates) * shadedColor;
    }
    else {
        FragmentColor = shadedColor;
    }
}