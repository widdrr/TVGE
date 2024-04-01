export module Graphics.Resources.Textures:Texture2D;

import <string>;

import :Texture;

namespace TVGE::Graphics
{
	export class Texture2D : public Texture
	{
		friend class Renderer;

	public:
		~Texture2D() override;

		void Bind(unsigned int p_unit);

	protected:
		Texture2D(const std::string& p_texturePath, bool p_repeat = false);
		Texture2D(unsigned int p_width, unsigned int p_height, unsigned int p_format);
		int _width, _height, _channels;
		unsigned char* _textureData;
	};
}