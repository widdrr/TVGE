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