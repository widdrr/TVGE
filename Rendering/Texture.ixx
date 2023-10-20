export module Graphics.Resources:Texture;

import <string>;

//TODO: support for multiple cubemaps, 3d textures, etc
export class Texture {

private:
	friend class Renderer;

	unsigned int _id;
	const int _width, _height, _channels;
	unsigned char* _textureData;

	Texture(const std::string& p_texturePath);

public:
	~Texture();
};