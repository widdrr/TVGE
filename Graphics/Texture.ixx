export module Graphics.Resources:Texture;

import <string>;

//TODO: support for multiple cubemaps, 3d textures, etc
export class Texture 
{
	friend class Renderer;

public:
	~Texture();

	const unsigned int GetId() const;
	void Bind(unsigned int p_unit) const;

	const std::string _texturePath;

private:
	Texture(const std::string& p_texturePath);

	unsigned int _id;
	const int _width, _height, _channels;
	unsigned char* _textureData;
};