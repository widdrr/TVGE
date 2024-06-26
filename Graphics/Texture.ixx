export module Graphics.Resources.Textures:Texture;

import <string>;

namespace TVGE::Graphics
{
	export class Texture
	{
		friend class Renderer;

	public:
		virtual ~Texture();

		const unsigned int GetId() const;


		const std::string& _texturePath;

	protected:
		Texture(const std::string& p_texturePath);
		Texture(const Texture&) = delete;
		Texture(Texture&) = delete;
		void Bind(unsigned int p_unit, unsigned int type) const;

		unsigned int _id;
	};
}