module Graphics.Resources.Textures:Texture;

#include <gl/glew.h>

Texture::Texture(const std::string& p_texturePath) :
	_texturePath(p_texturePath),
	_id()
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &_id);
}

const unsigned int Texture::GetId() const
{
	return _id;
}

void Texture::Bind(unsigned int p_unit, unsigned int type) const
{
	glActiveTexture(GL_TEXTURE0 + p_unit);
	glBindTexture(type, _id);
}