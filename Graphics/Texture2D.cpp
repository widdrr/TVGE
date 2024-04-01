module;

//TODO: see if this can be converted
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <gl/glew.h>

module Graphics.Resources.Textures:Texture2D;

import <iostream>;

using namespace TVGE::Graphics;

Texture2D::Texture2D(const std::string& p_texturePath, bool p_repeat) :
	Texture(p_texturePath),
	_width(),
	_height(),
	_channels()
{
	stbi_set_flip_vertically_on_load(true);
	_textureData = stbi_load(p_texturePath.c_str(), &_width, &_height, &_channels, 0);

	GLenum format = _channels == 4 ? GL_RGBA : GL_RGB;
	//TODO texture units
	if (_textureData) {
		glGenTextures(1, &_id);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _id);

		//TODO customizations
		if (p_repeat) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, _textureData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {
		std::cerr << "Failed to load texture " << p_texturePath << "\n";
	}
}

Texture2D::Texture2D(unsigned int p_width, unsigned int p_height, unsigned int p_format) :
	Texture(""),
	_width(p_width),
	_height(p_height)
{
	glGenTextures(1, &_id);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);

	glTexImage2D(GL_TEXTURE_2D, 0, p_format, _width, _height, 0, p_format, GL_UNSIGNED_BYTE, nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{
	if (_textureData != nullptr) {
		stbi_image_free(_textureData);
	}
}

void Texture2D::Bind(unsigned int p_unit)
{
	Texture::Bind(p_unit, GL_TEXTURE_2D);
}
