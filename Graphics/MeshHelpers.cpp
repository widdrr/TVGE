module MeshHelpers;

std::pair<std::vector<Vertex>, std::vector<unsigned int>> Common2DMeshes::Circle(
	unsigned int p_radius,
	unsigned int p_vertexCount)
{

	auto vertices = std::vector<Vertex>();
	vertices.reserve(p_vertexCount + 1);
	auto indices = std::vector<unsigned int>();
	indices.reserve(3 * p_vertexCount + 2);

	vertices.emplace_back(0.f, 0.f, 0.f,
		0.5f, 0.5f);
	indices.push_back(0);

	for (unsigned int i = 1; i <= p_vertexCount; ++i) {

		float theta = 2 * i * PI / p_vertexCount;
		vertices.emplace_back(p_radius * cosf(theta), p_radius * sinf(theta), 0.f,
			0.5f + cosf(theta) / 2.f, 0.5f + sinf(theta) / 2.f);
		indices.push_back(i);
	}
	indices.push_back(1);

	return std::make_pair(vertices, indices);

}


std::pair<std::vector<Vertex>, std::vector<unsigned int>> CommonMeshes::Cube()
{
	std::vector<Vertex> vertices = {
		Vertex(-1,-1,1),
		Vertex(1,-1,1),
		Vertex(1,1,1),
		Vertex(-1,1,1),
		Vertex(1,-1,-1),
		Vertex(-1,-1,-1),
		Vertex(-1,1,-1),
		Vertex(1,1,-1),
	};

	std::vector<unsigned int> indices = {
		0, 1, 2,  2, 3, 0, // Front face
		1, 4, 7,  7, 2, 1, // Right face
		5, 0, 3,  3, 6, 5, // Left face
		3, 2, 7,  7, 6, 3, // Top face
		5, 4, 1,  1, 0, 5, // Bottom face
		4, 5, 6,  6, 7, 4  // Back face
	};

	return std::make_pair(vertices, indices);
}