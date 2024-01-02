#version 460 core

in vec2 TextureCoordinates;
in vec3 Normal;
in vec3 FragmentPosition;

out vec4 FragmentColor;

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

//TODO: figure out a way to not allocate useless data for different light types.
struct LightSource
{
    vec4 position;
    
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;

    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

//TODO: Shader Buffers to support more lights
const int MaxLightSources = 128;

uniform LightSource glLights[MaxLightSources];
uniform int glLightCount;

uniform vec3 glCameraPosition;

uniform bool glShadowCasterPresent;
uniform vec3 glShadowCasterPosition;
uniform float glShadowFarPlane;
uniform samplerCubeShadow glShadowMap;

vec3 ComputeAmbientColor(LightSource light)
{
    vec3 color = light.ambientColor;
    if(glMaterial.hasTexture) {
        color *= vec3(texture(glMaterial.ambientMap, TextureCoordinates));
    }
    else {
        color *= glMaterial.ambientColor;
    }
        
    return color;
}

vec3 ComputeDiffuseColor(LightSource light, vec3 normal, vec3 lightDirection)
{
    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    
    vec3 color = diffuseValue * light.diffuseColor;
    if(glMaterial.hasTexture) {
        color *= vec3(texture(glMaterial.diffuseMap, TextureCoordinates));
    }
    else {
        color *= glMaterial.diffuseColor;
    }

    return color;
}

vec3 ComputeSpecularColor(LightSource light, vec3 normal, vec3 lightDirection, vec3 cameraDirection)
{
    vec3 halfwayDirection = normalize(lightDirection + cameraDirection);
    float specularValue = pow(max(dot(normal, halfwayDirection), 0.0), glMaterial.shininess);

    vec3 color = specularValue * light.specularColor;
    if(glMaterial.hasTexture) {
        color *= vec3(texture(glMaterial.specularMap, TextureCoordinates));
    }
    else {
        color *= glMaterial.specularColor;
    }

    return color;
}

float ComputeShadow(vec3 fragmentPosition, vec3 normal)
{
    if(!glShadowCasterPresent){
        return 0;
    }

    vec3 relativePosition = fragmentPosition - glShadowCasterPosition;
    vec3 casterDirection = normalize(glShadowCasterPosition - FragmentPosition);
    float bias = max(0.05 * (1.0 - dot(normal, casterDirection)), 0.005); 

    //the reference value for depth comparison is the distance from the shadow caster to the fragmentcolor
    //with a shadow bias added to prevent shadow acne, normalized by dividing by the far plane
    float testValue = (length(relativePosition) - bias) / glShadowFarPlane;
    
    return texture(glShadowMap, vec4(relativePosition, testValue));
}

void main()
{
    vec3 normalizedNormal = normalize(Normal);
    vec3 cameraDirection = normalize(-FragmentPosition);

    vec3 ambientColor, diffuseColor, specularColor;
    float shadow;

    for (int i = 0; i < glLightCount; ++i) {
        //Determining if light source is point or directional
        vec3 lightDirection;
        float attenuation;
        
        if(glLights[i].position.w == 1) {
            float lightDistance = length(glLights[i].position.xyz - FragmentPosition);

            lightDirection = normalize(glLights[i].position.xyz - FragmentPosition);

            attenuation = 1.f / (glLights[i].constantAttenuation + 
                                 glLights[i].linearAttenuation * lightDistance + 
                                 glLights[i].quadraticAttenuation * (lightDistance * lightDistance)); 
        }
        else {
            lightDirection = normalize(glLights[i].position.xyz);
            //Directional Lights do not have attenuation since there is no concept of distance
            attenuation = 1.f;
        }
        
        ambientColor += ComputeAmbientColor(glLights[i]) * attenuation;

        diffuseColor += ComputeDiffuseColor(glLights[i], normalizedNormal, lightDirection) * attenuation;

        specularColor += ComputeSpecularColor(glLights[i], normalizedNormal, lightDirection, cameraDirection) * attenuation;

        shadow = ComputeShadow(FragmentPosition, normalizedNormal);
    }

    vec4 shadedColor = vec4(((glMaterial.emissiveColor + ambientColor + (1.f - shadow) * (diffuseColor + specularColor))), 1.f);
    
    FragmentColor = shadedColor;
}