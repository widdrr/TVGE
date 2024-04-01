module;

//TODO: see if this can be converted
#include <stb_image.h>
#include <gl/glew.h>

module Graphics.Resources.Textures:Cubemap;

import <string>;
import <vector>; 
import <iostream>;

using namespace TVGE::Graphics;

//TODO: improve this
Cubemap::Cubemap(const std::string& p_frontPath,
				 const std::string& p_rightPath,
				 const std::string& p_leftPath,
				 const std::string& p_topPath,
				 const std::string& p_bottomPath,
				 const std::string& p_backPath) :
	//TODO: improve this mess
	Texture(p_frontPath + p_rightPath + p_leftPath + p_topPath + p_bottomPath + p_backPath),
	_width(),
	_height(),
	_channels()
{
	stbi_set_flip_vertically_on_load(false);
	_textureData = stbi_load(p_frontPath.c_str(), &_width, &_height, &_channels, 0);

	GLenum format = _channels == 4 ? GL_RGBA : GL_RGB;

	if (_textureData) {
		glGenTextures(1, &_id);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

		//TODO customizations
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, _textureData);

		const std::vector<std::string> faces = { p_rightPath, p_leftPath, p_topPath, p_bottomPath, p_backPath };

		for (unsigned int i = 0; i < 5; ++i) {
			int width = 0;
			int height = 0;
			int channels = 0;

			unsigned char* textureData = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, textureData);

			stbi_image_free(textureData);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	}
	else {
		std::cerr << "Failed to load cubemap with front face " << p_frontPath << "\n";
	}

}

Cubemap::Cubemap(unsigned int p_width, unsigned int p_height, unsigned int p_format) :
	Texture(""),
	_width(p_width),
	_height(p_height)
{
	glGenTextures(1, &_id);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

	//TODO customizations
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);

	for (int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, p_format, _width, _height, 0, p_format, GL_FLOAT, nullptr);
	}
}

Cubemap::~Cubemap()
{
	if (_textureData) {
		stbi_image_free(_textureData);
	}
}

void Cubemap::Bind(unsigned int p_unit)
{
	Texture::Bind(p_unit, GL_TEXTURE_CUBE_MAP);
}
