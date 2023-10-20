export module Graphics.Resources:Mesh;

import Vertex;
import <GL/glew.h>;
import <vector>;

export class Mesh
{
private:
	std::vector<Vertex> _vertices;
	const std::vector<unsigned int> _indices;
	const GLenum _drawMode;

public:
	Mesh(const std::vector<Vertex>& p_vertices, const std::vector<unsigned int>& p_indices, const GLenum p_mode = GL_TRIANGLES);
	const std::vector<Vertex>& GetVertices() const;
	const std::vector<unsigned int>& GetElements() const;
	const GLsizei GetElementCount() const;
	const GLenum GetDrawMode() const;
};

