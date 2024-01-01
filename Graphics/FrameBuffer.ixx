export module Graphics.Resources:FrameBuffer;

import Graphics.Resources.Textures;
import :ShaderProgram;

import <memory>;

export class FrameBuffer
{
	friend class FrameBufferBuilder;
	friend class Renderer;
public:
	~FrameBuffer();

	void Bind();

private:
	FrameBuffer();
	unsigned int _id;
	unsigned int _colorBufferId, _depthBufferId, _stencilBufferId;
	std::shared_ptr<Texture> _colorTexture;
	std::shared_ptr<Texture> _depthTexture;
	std::shared_ptr<Texture> _stencilTexture;
};

export class FrameBufferBuilder
{
public:
	static FrameBufferBuilder Init();
	FrameBufferBuilder& AttachColorTexture2D(std::shared_ptr<Texture2D> p_texture);
	FrameBufferBuilder& AttachDepthTexture2D(std::shared_ptr<Texture2D> p_texture);
	FrameBufferBuilder& AttachStencilTexture2D(std::shared_ptr<Texture2D> p_texture);
	FrameBufferBuilder& AttachColorCubemap(std::shared_ptr<Cubemap> p_texture);
	FrameBufferBuilder& AttachDepthCubemap(std::shared_ptr<Cubemap> p_texture);
	FrameBufferBuilder& AttachStencilCubemap(std::shared_ptr<Cubemap> p_texture);
	FrameBufferBuilder& AttachColorRenderbuffer(unsigned int p_width, unsigned int p_height);
	FrameBufferBuilder& AttachDepthRenderbuffer(unsigned int p_width, unsigned int p_height);
	FrameBufferBuilder& AttachStencilRenderbuffer(unsigned int p_width, unsigned int p_height);
	FrameBufferBuilder& NoColorBuffer();
	std::unique_ptr<FrameBuffer>&& Build();
private:
	FrameBufferBuilder();
	std::unique_ptr<FrameBuffer> _object;
	bool _hasColor, _hasDepth, _hasStencil;
};