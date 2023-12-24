export module Graphics.Resources.Textures:Cubemap;

import <string>;

import :Texture;

export class Cubemap : public Texture {
	friend class Renderer;

public:
	~Cubemap() override;

	void Bind(unsigned int p_unit);

protected:
	Cubemap(const std::string& p_frontPath,
			const std::string& p_rightPath,
			const std::string& p_leftPath,
			const std::string& p_topPath,
			const std::string& p_bottomPath,
			const std::string& p_backPath);

	int _width, _height, _channels;
	unsigned char* _textureData;
};