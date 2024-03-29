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
			gl_Position = glProjectionMatrix * glViewMatrix * glModelMatrix * vec4(inPosition, 1.f);
			TextureCoordinates = inTextureCoordinates;
			Normal = mat3(transpose(inverse(glModelMatrix))) * inNormal;
			FragmentPosition = vec3(glModelMatrix * vec4(inPosition, 1.f));
			LightFragmentPosition = glLightMatrix * vec4(FragmentPosition, 1.f);
		}