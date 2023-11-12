export module Graphics.Resources:Texture;

import <string>;

//TODO: support for multiple cubemaps, 3d textures, etc
export class Texture 
{
	friend class Renderer;

private:
	Texture(const std::string& p_texturePath);
public:
	~Texture();

	const unsigned int GetId() const;
	void Bind(unsigned int p_unit) const;

public:
	const std::string _texturePath;
private:
	unsigned int _id;
	const int _width, _height, _channels;
	unsigned char* _textureData;
};