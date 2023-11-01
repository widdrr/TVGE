module MeshHelpers;

std::pair<std::vector<Vertex>, std::vector<unsigned int>> Common2DMeshes::Circle(
	unsigned int p_radius, 
	unsigned int p_vertexCount,
	glm::vec4 p_color) {
	
	auto vertices = std::vector<Vertex>();
	vertices.reserve(p_vertexCount + 1);
	auto indices = std::vector<unsigned int>();
	indices.reserve(3 * p_vertexCount + 2);

	vertices.emplace_back(0.f, 0.f, 0.f, 
					      p_color.r, p_color.g, p_color.b, p_color.a,
						  0.5f, 0.5f);
	indices.push_back(0);

	for (unsigned int i = 1; i <= p_vertexCount; ++i) {
		
		float theta = 2 * i * PI / p_vertexCount;
		vertices.emplace_back(p_radius * cosf(theta), p_radius * sinf(theta), 0.f,
							 p_color.r, p_color.g, p_color.b, p_color.a,
							 0.5f + cosf(theta) / 2.f, 0.5f + sinf(theta) / 2.f );
		indices.push_back(i);
	}
	indices.push_back(1);

	return std::make_pair(vertices, indices);


}