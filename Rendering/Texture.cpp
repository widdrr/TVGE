module;

#define STB_IMAGE_IMPLEMENTATION

module Graphics.Resources:Texture;

import <gl/glew.h>;
import <iostream>;
import <stb_image.h>;

Texture::Texture(const std::string& p_texturePath):
	_id(),
	_width(),
	_height(),
	_channels()
{
	stbi_set_flip_vertically_on_load(true);
	_textureData = stbi_load(p_texturePath.c_str(), const_cast<int*>(&_width), const_cast<int*>(&_height), const_cast<int*>(&_channels), 0);

	GLenum format = _channels == 4 ? GL_RGBA : GL_RGB;
	//TODO texture units
	if (_textureData) {
		glGenTextures(1, &_id);

		glBindTexture(GL_TEXTURE_2D, _id);

		//TODO customizations
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

Texture::~Texture() {

	stbi_image_free(_textureData);
}
