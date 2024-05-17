export module Graphics:Shaders;

import <string_view>;

export namespace TVGE::Graphics::ShaderSources
{
    constexpr std::string_view defaultVertex = { R"(
	
		#version 460 core

		layout(location = 0) in vec3 inPosition;
		layout(location = 1) in vec2 inTextureCoordinates;
		layout(location = 2) in vec3 inNormal;

		out vec2 TextureCoordinates;
		out vec3 Normal;
		out vec3 FragmentPosition;
		out vec4 LightFragmentPosition;

		uniform mat4 glModelMatrix;
		uniform mat4 glViewMatrix;
		uniform mat4 glProjectionMatrix;
		uniform mat4 glLightMatrix;

		void main()
		{
			TextureCoordinates = inTextureCoordinates;
			Normal = mat3(transpose(inverse(glModelMatrix))) * inNormal;
			FragmentPosition = vec3(glModelMatrix * vec4(inPosition, 1.f));
			LightFragmentPosition = glLightMatrix * vec4(FragmentPosition, 1.f);

			gl_Position = glProjectionMatrix * glViewMatrix * glModelMatrix * vec4(inPosition, 1.f);
		}
	)" };

    constexpr std::string_view defaultFragment = { R"(
       #version 460 core

        in vec2 TextureCoordinates;
        in vec3 Normal;
        in vec3 FragmentPosition;
        in vec4 LightFragmentPosition;

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
        uniform vec4 glShadowCasterPosition;
        uniform float glShadowFarPlane;
        uniform samplerCubeShadow glPointShadowMap;
        uniform sampler2DShadow glDirectionalShadowMap;

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

            if(glShadowCasterPosition.w == 1)
            {
                vec3 relativePosition = fragmentPosition - glShadowCasterPosition.xyz;
                vec3 casterDirection = normalize(glShadowCasterPosition.xyz - FragmentPosition);
                float bias = max(0.5 * (1.0 - dot(normal, casterDirection)), 0.01); 

                //the reference value for depth comparison is the distance from the shadow caster to the fragmentcolor
                //with a shadow bias added to prevent shadow acne, normalized by dividing by the far plane
                float testValue = (length(relativePosition) - bias) / glShadowFarPlane;
    
                return texture(glPointShadowMap, vec4(relativePosition, testValue));
            }

            else {
                vec3 coords = LightFragmentPosition.xyz / LightFragmentPosition.w;
                coords = coords * 0.5f + 0.5f;
        
                vec3 casterDirection = normalize(glShadowCasterPosition.xyz);
                float bias = max(0.5 * (1.0 - dot(normal, casterDirection)), 0.01); 

                float testValue = (coords.z - bias);
                return texture(glDirectionalShadowMap, vec3(coords.xy, testValue));
            }
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
	)" };

    constexpr std::string_view pointShadowVertex = { R"(
		#version 460 core

		layout (location = 0) in vec3 inPosition;

		uniform mat4 glModelMatrix;

		void main()
		{
			gl_Position = glModelMatrix * vec4(inPosition, 1.f);
		}
	)" };

    constexpr std::string_view pointShadowGeometry = { R"(
        #version 460 core

        layout (triangles) in;
        layout (triangle_strip, max_vertices=18) out;

        uniform mat4 glShadowMatrices[6];

        out vec4 FragmentPosition;

        void main()
        {
            for(int face = 0; face < 6; ++face)
            {
                gl_Layer = face;
                for(int i = 0; i < 3; ++i) 
                {
                    FragmentPosition = gl_in[i].gl_Position;
                    gl_Position = glShadowMatrices[face] * FragmentPosition;
                    EmitVertex();
                }    
                EndPrimitive();
            }
        }  
    )" };

    constexpr std::string_view pointShadowFragment = { R"(
        #version 460 core
        in vec4 FragmentPosition;

        uniform vec3 glShadowCasterPosition;
        uniform float glShadowFarPlane;

        void main()
        {
            float lightDistance = length(FragmentPosition.xyz - glShadowCasterPosition);
    
            // map to [0;1] range by dividing by far_plane
            lightDistance = lightDistance / glShadowFarPlane;
    
            gl_FragDepth = lightDistance;
        }  
    )" };

    constexpr std::string_view directionalShadowVertex = { R"(
		#version 460 core

		layout (location = 0) in vec3 inPosition;

		uniform mat4 glModelMatrix;
        uniform mat4 glShadowMatrices[1];


		void main()
		{
			gl_Position = glShadowMatrices[0] * glModelMatrix * vec4(inPosition, 1.f);
		}
	)" };

    constexpr std::string_view directionalShadowFragment = { R"(
        #version 460 core
        void main()
        {
        }  
    )" };

    constexpr std::string_view wireframeVertex = { R"(
		#version 460 core

		layout (location = 0) in vec3 inPosition;

		uniform mat4 glModelMatrix;
		uniform mat4 glViewMatrix;
		uniform mat4 glProjectionMatrix;

		void main()
		{
			gl_Position = glProjectionMatrix * glViewMatrix * glModelMatrix * vec4(inPosition, 1.f);
		}
	)" };

    constexpr std::string_view wireframeFragment = { R"(
        #version 460 core

        uniform vec3 glColor;
        out vec4 FragmentColor;

        void main()
        {
            FragmentColor = vec4(glColor, 1.f);
        }  
    )" };
}