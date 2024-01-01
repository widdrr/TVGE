module;

#include <GL/glew.h>

module Graphics.Resources:FrameBuffer;

import <iostream>;

FrameBufferBuilder FrameBufferBuilder::Init()
{
	return FrameBufferBuilder();
}

FrameBufferBuilder& FrameBufferBuilder::AttachColorTexture2D(std::shared_ptr<Texture2D> p_texture)
{
	_object->Bind();
	if (!_hasColor) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, p_texture->GetId(), 0);
		_object->_colorTexture = p_texture;
		_hasColor = true;
	}
	else {
		std::cerr << "Framebuffer already has color attachment, skippping\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return *this;
}

FrameBufferBuilder& FrameBufferBuilder::AttachDepthTexture2D(std::shared_ptr<Texture2D> p_texture)
{
	_object->Bind();
	if (!_hasDepth) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, p_texture->GetId(), 0);
		_object->_depthTexture = p_texture;
		_hasDepth = true;
	}
	else {
		std::cerr << "Framebuffer already has depth attachment, skippping\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return *this;
}

FrameBufferBuilder& FrameBufferBuilder::AttachStencilTexture2D(std::shared_ptr<Texture2D> p_texture)
{
	_object->Bind();
	if (!_hasStencil) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, p_texture->GetId(), 0);
		_object->_stencilTexture = p_texture;
		_hasStencil = true;
	}
	else {
		std::cerr << "Framebuffer already has stencil attachment, skippping\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return *this;
}

FrameBufferBuilder& FrameBufferBuilder::AttachColorCubemap(std::shared_ptr<Cubemap> p_texture)
{
	_object->Bind();
	if (!_hasColor) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, p_texture->GetId(), 0);
		_object->_colorTexture = p_texture;
		_hasColor = true;
	}
	else {
		std::cerr << "Framebuffer already has color attachment, skippping\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return *this;
}

FrameBufferBuilder& FrameBufferBuilder::AttachDepthCubemap(std::shared_ptr<Cubemap> p_texture)
{
	_object->Bind();
	if (!_hasDepth) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, p_texture->GetId(), 0);
		_object->_depthTexture = p_texture;
		_hasDepth = true;
	}
	else {
		std::cerr << "Framebuffer already has depth attachment, skippping\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return *this;
}

FrameBufferBuilder& FrameBufferBuilder::AttachStencilCubemap(std::shared_ptr<Cubemap> p_texture)
{
	_object->Bind();
	if (!_hasStencil) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, p_texture->GetId(), 0);
		_object->_stencilTexture = p_texture;
		_hasStencil = true;
	}
	else {
		std::cerr << "Framebuffer already has stencil attachment, skippping\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return *this;
}

FrameBufferBuilder& FrameBufferBuilder::AttachColorRenderbuffer(unsigned int p_width, unsigned int p_height)
{
	_object->Bind();
	if (!_hasColor) {
		glGenRenderbuffers(1, &_object->_colorBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, _object->_colorBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F, p_width, p_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _object->_colorBufferId);

		_hasColor = true;
	}
	else {
		std::cerr << "Framebuffer already has color attachment, skippping\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return *this;
}

FrameBufferBuilder& FrameBufferBuilder::AttachDepthRenderbuffer(unsigned int p_width, unsigned int p_height)
{
	_object->Bind();
	if (!_hasDepth) {
		glGenRenderbuffers(1, &_object->_depthBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, _object->_depthBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, p_width, p_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _object->_depthBufferId);

		_hasDepth = true;
	}
	else {
		std::cerr << "Framebuffer already has depth attachment, skippping\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return *this;
}

FrameBufferBuilder& FrameBufferBuilder::AttachStencilRenderbuffer(unsigned int p_width, unsigned int p_height)
{
	_object->Bind();
	if (!_hasStencil) {
		glGenRenderbuffers(1, &_object->_stencilBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, _object->_stencilBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, p_width, p_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _object->_stencilBufferId);

		_hasStencil = true;
	}
	else {
		std::cerr << "Framebuffer already has stencil attachment, skippping\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return *this;
}

FrameBufferBuilder& FrameBufferBuilder::NoColorBuffer()
{
	_object->Bind();
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return *this;
}

std::unique_ptr<FrameBuffer>&& FrameBufferBuilder::Build()
{
	_object->Bind();
	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return std::move(_object);
	}
	else {
		std::cerr << "Framebuffer is not complete, returning null\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return nullptr;
}

FrameBufferBuilder::FrameBufferBuilder() :
	_object(new FrameBuffer()),
	_hasColor(),
	_hasDepth(),
	_hasStencil()
{
}