#version 460 core

in vec2 TextureCoordinates;
in vec3 Normal;
in vec3 FragmentPosition;

out vec4 FragmentColor;

uniform vec3 glCameraPosition;

uniform struct Material
{
    bool hasTexture;
    sampler2D ambientMap;
    sampler2D diffuseMap;
    sampler2D specularMap;

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

    vec3 color = glLight.ambientColor;
    if(glMaterial.hasTexture){
        color *= vec3(texture(glMaterial.ambientMap, TextureCoordinates));
    }
    else{
        color *= glMaterial.ambientColor;
    }
        
    return color;
}

vec3 ComputeDiffuseColor(vec3 normal, vec3 lightDirection){

    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    
    vec3 color = diffuseValue * glLight.diffuseColor;
    if(glMaterial.hasTexture){
        color *= vec3(texture(glMaterial.diffuseMap, TextureCoordinates));
    }
    else{
        color *= glMaterial.diffuseColor;
    }

    return color;
}

vec3 ComputeSpecularColor(vec3 normal, vec3 lightDirection){

    vec3 cameraDirection = normalize(glCameraPosition - FragmentPosition);
    vec3 reflectedLightDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(dot(cameraDirection, reflectedLightDirection), 0.0), glMaterial.shininess);

    vec3 color = specularValue * glLight.specularColor;
    if(glMaterial.hasTexture){
        color *= vec3(texture(glMaterial.specularMap, TextureCoordinates));
    }
    else{
        color *= glMaterial.specularColor;
    }

    return color;
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
    
    FragmentColor = shadedColor;
}