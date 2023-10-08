#include "Renderer.h"

int main() {

	std::vector<Vertex> vertices = {
		{0.5f, 0.5f, 0.5f},
		{-0.5f, 0.5f, -0.5f},
		{-0.5f, 0.5f, 0.5f},
		{0.5f, -0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f},
		{0.5f, 0.5f, -0.5f},
		{0.5f, -0.5f, 0.5f},
		{-0.5f, -0.5f, 0.5f} };
	
	std::vector<unsigned int> order = {
		0, 1, 2,
		1, 3, 4,
		5, 6, 3,
		7, 3, 6,
		2, 4, 7,
		0, 7, 6,
		0, 5, 1,
		1, 5, 3,
		5, 0, 6,
		7, 4, 3,
		2, 1, 4,
		0, 2, 7 };

	Mesh cubeMesh(vertices, order);
	Object cube(cubeMesh);
	
	auto renderer = Renderer::GetInstance();
		
	renderer->AddObject(cube);
	renderer->Run();
}