#pragma once
#include <string>
#include <gl/glew.h>

class Texture {

private:
	friend class Renderer;
	
	GLuint _id;
	const int _width, _height, _channels;
	unsigned char* _textureData;

	//TODO, fix object slicingadkjadkadhwad
	Texture(const std::string& p_texturePath);

public:
	~Texture();
};