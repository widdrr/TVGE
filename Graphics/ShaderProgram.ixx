export module Graphics.Resources:ShaderProgram;

import <glm/mat4x4.hpp>;
import <string>;

namespace TVGE::Graphics
{
	export class ShaderProgram
	{
		//Shaders should be instantiated by the Renderer
		friend class Renderer;

	public:
		~ShaderProgram();

		void SetVariable(std::string_view p_variableName, glm::mat3 p_value, bool p_debug = false);
		void SetVariable(std::string_view p_variableName, glm::mat4 p_value, bool p_debug = false);
		void SetVariable(std::string_view p_variableName, float p_value, bool p_debug = false);
		void SetVariable(std::string_view p_variableName, int p_value, bool p_debug = false);
		void SetVariable(std::string_view p_variableName, glm::vec3 p_value, bool p_debug = false);
		void SetVariable(std::string_view p_variableName, glm::vec4 p_value, bool p_debug = false);

		const unsigned int GetProgramSourceId() const;
		static unsigned int GenerateProgramSourceId(const std::string& p_vertexShader,
													const std::string& p_fragmentShader,
													const std::string& p_geometryShader);

	private:
		static constexpr unsigned int _dumpSize = 1024;

		ShaderProgram(const std::string& p_vertexShader,
					  const std::string& p_fragmentShader,
					  const std::string& p_geometryShader = "",
					  bool p_files = true);
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&) = delete;

		void AddShader(std::string p_shaderText, unsigned int p_shaderType);
		unsigned int GetUniformLocation(std::string_view p_variableName, bool p_debug);

		static std::string ReadShaderFromFile(std::string p_fileName);

		unsigned int _id;
		unsigned int _sourceId;
		bool _failed;

		template <typename T, typename... Rest>
		static void HashCombine(unsigned int& seed, const T& v, Rest... rest)
		{
			seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
			(HashCombine(seed, rest), ...);
		}
	};
}