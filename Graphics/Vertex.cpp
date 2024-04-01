module Graphics.Resources:Vertex;

using namespace TVGE::Graphics;

Vertex::Vertex(float p_posX, float p_posY, float p_posZ, 
			   float p_textS, float p_textT,
			   float p_normX, float p_normY, float p_normZ) :
	_position(p_posX, p_posY, p_posZ),
	_textureCoordinates(p_textS, p_textT),
	_normal(p_normX, p_normY, p_normZ)
{}
