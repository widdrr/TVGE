export module Graphics.Resources.Textures:Texture2D;

import <string>;

import :Texture;

export class Texture2D : public Texture {
	friend class Renderer;

public:
	 ~Texture2D() override;

	 void Bind(unsigned int p_unit);

protected:
	Texture2D(const std::string& p_texturePath);
	int _width, _height, _channels;
	unsigned char* _textureData;
};