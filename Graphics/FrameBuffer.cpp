module;

#include <GL/glew.h>

module Graphics.Resources:FrameBuffer;

FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, &_id);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteRenderbuffers(1, &_colorBufferId);
	glDeleteRenderbuffers(1, &_depthBufferId);
	glDeleteRenderbuffers(1, &_stencilBufferId);
	glDeleteFramebuffers(1, &_id);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
}

void FrameBuffer::SetDepthTexture2D(std::shared_ptr<Texture2D> p_texture)
{
	Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, p_texture->GetId(), 0);
	_depthTexture = p_texture;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::SetDepthCubemap(std::shared_ptr<Cubemap> p_cubemap)
{
	Bind();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, p_cubemap->GetId(), 0);
	_depthTexture = p_cubemap;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
