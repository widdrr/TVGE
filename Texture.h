#pragma once
#include <string>
#include <gl/glew.h>

//TODO: support for multiple cubemaps, 3d textures, etc
class Texture {

private:
	friend class Renderer;
	
	GLuint _id;
	const int _width, _height, _channels;
	unsigned char* _textureData;

	Texture(const std::string& p_texturePath);

public:
	~Texture();
};