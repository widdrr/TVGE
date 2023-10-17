#include "Renderer.h"
#include <thread>
#include <chrono>
void rotateObj(Entity& obj, float theta, bool& flag)
{
	while (flag) {
		
		obj.Rotate(1.f, 1.f, 1.f, theta);
		std::this_thread::sleep_for(std::chrono::milliseconds(4));
	}
}

int main() {

	//TODO: make a builder for this mess
	std::vector<Vertex> vertices = {

		//Front Face
		//B L
		{-0.5f, -0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.25f},
		//B R
		{0.5f, -0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75f, 0.25f},
		//T R
		{0.5f, 0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75f, 0.5f},
		//T L
		{-0.5f, 0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.5f},

		//Right Face
		//B L
		{0.5f, -0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75f, 0.25f},
		//B R
		{0.5f, -0.5, -0.5, 
		0.5f, 0.5f, 0.5f, 1.f,
		1.f, 0.25f},
		//T R
		{0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		1.f, 0.5f},
		//T L
		{0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75, 0.5},

		//Back Face
		//B L
		{0.5f, -0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.f, 0.25},
		//B R
		{-0.5f, -0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.25, 0.25},
		//T R
		{-0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.25, 0.5},
		//T L
		{0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.f, 0.5},

		//Left Face
		//B L
		{-0.5f, -0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.25f, 0.25},
		//B R
		{-0.5f, -0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5, 0.25},
		//T R
		{-0.5f, 0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5, 0.5},
		//T L
		{-0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.25f, 0.5},

		//Top Face
		//B L
		{-0.5f, 0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5, 0.5},
		//B R
		{0.5f, 0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75, 0.5},
		//T R
		{0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75, 0.75},
		//T L
		{-0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5, 0.75},

		//Bottom Face
		//B L
		{-0.5f, -0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5, 0.f},
		//B R
		{0.5f, -0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75, 0.f},
		//T R
		{0.5f, -0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75, 0.25},
		//T L
		{-0.5f, -0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5, 0.25}
	};

	std::vector<unsigned int> order = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20,
	};

//	std::vector<Vertex> diceVertices = {
//		//0
//	{-0.5f, -0.5f, 0.5f,
//	 1.f, 1.f, 1.f, 1.f,
//	 0.5f, 0.25f},
//
//		//1
//	{ 0.5f, -0.5f, 0.5f,
//	 1.f, 1.f, 1.f, 1.f,
//	 0.75f, 0.25f },
//
//		//2
//	{ 0.5f, 0.5f, 0.5f,
//	 1.f, 1.f, 1.f, 1.f,
//	 0.75f, .5f },
//
//		//3
//	{ -0.5f, 0.5f, 0.5f,
//	 1.f, 1.f, 1.f, 1.f,
//	 0.5f, 0.5f },
//
//		//4
//	{ 0.5f, -0.5f, -0.5f,
//	 1.f, 1.f, 1.f, 1.f,
//	 1.f, 0.25f },
//
//		//5
//	{ 0.5f, 0.5f, -0.5f,
//	 1.f, 1.f, 1.f, 1.f,
//	 1.f, 0.5f },
//
//		//6
//	{ -0.5f, -0.5f, -0.5f,
//	 1.f, 1.f, 1.f, 1.f,
//	 0.5f, 0.f },
//
//		//7
//	{ 0.5f, -0.5f, -0.5f,
//	 1.f, 1.f, 1.f, 1.f,
//	 0.75f, 0.f },
//
//		//8
//	{ 0.5f, 0.5f, -0.5f,
//	 1.f, 1.f, 1.f, 1.f,
//	 0.75f, 0.75f },
//
//		//9
//	{ -0.5f, 0.5f, -0.5f,
//	 1.f, 1.f, 1.f, 1.f,
//	 0.5f, 0.75f },
//
//		//10
//	{ -0.5f, -0.5f, -0.5f,
//	 1.f, 1.f, 1.f, 1.f,
//	 0.25f, 0.25f },
//
//		//11
//	{ -0.5f, 0.5f, -0.5f,
//	 1.f, 1.f, 1.f, 1.f,
//	 0.25f, 0.5f },
//
//		//12
//	{ 0.5f, -0.5f, -0.5f,
//	 1.f, 1.f, 1.f, 1.f,
//	 0.f, 0.25f },
//
//		//13
//	{ 0.5f, 0.5f, -0.5f,
//	 1.f, 1.f, 1.f, 1.f,
//	 0.f, 0.5f }
//};
//
//	std::vector<unsigned int> diceOrder = {
//		0, 1, 2,
//		2, 3, 0,
//		1, 4, 5,
//		5, 2, 1,
//		3, 2, 8,
//		6, 7, 1,
//		1, 0, 6,
//		3, 2, 8,
//		8, 9, 3,
//		10, 0, 3,
//		3, 11, 10,
//		12, 10, 11,
//		11, 13, 12
//	};

	Mesh cubeMesh(vertices, order);
	//Mesh diceMesh(diceVertices, diceOrder);
	Entity cube;
	Entity light;
	Entity cube3;
	//auto comp = cube.CreateComponent<GraphicsComponent>();
	//comp->mesh = std::make_shared<Mesh>(diceMesh);
	
	auto comp2 = light.CreateComponent<GraphicsComponent>();
	comp2->mesh = std::make_shared<Mesh>(cubeMesh);
	light.CreateComponent<LightSourceComponent>(1.f,1.f,1.f, 0.3f, 0.9f);
	
	auto comp3 = cube3.CreateComponent<GraphicsComponent>();
	comp3->mesh = std::make_shared<Mesh>(cubeMesh);
	
	//cube.Translate(-0.5f, 0.f, -3.f);
	light.Translate(0.5f, 0.2f, -5.f);
	light.Scale(0.1f, 0.1f, 0.1f);
	
	cube3.Translate(-0.5, 0.5, 3);
	//cube3.Rotate(1.f, 1.f, 1.f, 45.f);

	auto renderer = Renderer::GetInstance();
	
	comp3->texture = renderer->TextureFactory("dice.png");
	comp2->shaderProgram = renderer->ShaderFactory("shader.vert", "shaderLightSource.frag");
	renderer->SetPerspective(90, 0.1f, 100.f);
	renderer->SetLightSource(light);
	//renderer->AddObject(cube);
	renderer->AddObject(light);
	renderer->AddObject(cube3);
	
	bool flag = true;
	std::thread objThread(rotateObj, std::ref(cube3), 1.f, std::ref(flag));
	renderer->Run();
	flag = false;
	objThread.join();

}