#include "Renderer.h"
#include <thread>
#include <chrono>
void rotateObj(Entity& obj, float theta, bool& flag)
{
	while (flag) {
		
		obj.Rotate(1.f, 1.f, 1.f, theta);
		std::this_thread::sleep_for(std::chrono::microseconds(4));
	}
}

int main() {

	//TODO: make a builder for this mess
	std::vector<Vertex> vertices = {

		{0.5f, 0.5f, 0.5f,
		 1.f, 1.f, 1.f, 1.f,
		 0.75f, 0.2f},
		
		{-0.5f, 0.5f, -0.5f,
		 0.f, 1.f, 0.f, 1.f,
		 0.f, 0.f},
		
		{-0.5f, 0.5f, 0.5f,
		 0.f, 1.f, 1.f, 1.f,
	     0.f, 1.f},
		
		{0.5f, -0.5f, -0.5f,
		 1.f, 0.f, 0.f, 1.f,
		 0.f, 0.f},
		
		{-0.5f, -0.5f, -0.5f,
		 0.f, 0.f, 0.f, 1.f,
	     1.f, 0.f},
		
		{0.5f, 0.5f, -0.5f,
		 1.f, 1.f, 0.f, 1.f,
	     1.f, 0.f},
		
		{0.5f, -0.5f, 0.5f,
		 1.f, 0.f, 1.f, 1.f,
	     0.f, 1.f},
		
		{-0.5f, -0.5f, 0.5f,
		 0.f, 0.f, 1.f, 1.f,
	     1.f, 1.f} };
	
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

	std::vector<Vertex> diceVertices = {
		//0
	{-0.5f, -0.5f, 0.5f,
	 1.f, 1.f, 1.f, 1.f,
	 0.5f, 0.25f},

		//1
	{ 0.5f, -0.5f, 0.5f,
	 1.f, 1.f, 1.f, 1.f,
	 0.75f, 0.25f },

		//2
	{ 0.5f, 0.5f, 0.5f,
	 1.f, 1.f, 1.f, 1.f,
	 0.75f, .5f },

		//3
	{ -0.5f, 0.5f, 0.5f,
	 1.f, 1.f, 1.f, 1.f,
	 0.5f, 0.5f },

		//4
	{ 0.5f, -0.5f, -0.5f,
	 1.f, 1.f, 1.f, 1.f,
	 1.f, 0.25f },

		//5
	{ 0.5f, 0.5f, -0.5f,
	 1.f, 1.f, 1.f, 1.f,
	 1.f, 0.5f },

		//6
	{ -0.5f, -0.5f, -0.5f,
	 1.f, 1.f, 1.f, 1.f,
	 0.5f, 0.f },

		//7
	{ 0.5f, -0.5f, -0.5f,
	 1.f, 1.f, 1.f, 1.f,
	 0.75f, 0.f },

		//8
	{ 0.5f, 0.5f, -0.5f,
	 1.f, 1.f, 1.f, 1.f,
	 0.75f, 0.75f },

		//9
	{ -0.5f, 0.5f, -0.5f,
	 1.f, 1.f, 1.f, 1.f,
	 0.5f, 0.75f },

		//10
	{ -0.5f, -0.5f, -0.5f,
	 1.f, 1.f, 1.f, 1.f,
	 0.25f, 0.25f },

		//11
	{ -0.5f, 0.5f, -0.5f,
	 1.f, 1.f, 1.f, 1.f,
	 0.25f, 0.5f },

		//12
	{ 0.5f, -0.5f, -0.5f,
	 1.f, 1.f, 1.f, 1.f,
	 0.f, 0.25f },

		//13
	{ 0.5f, 0.5f, -0.5f,
	 1.f, 1.f, 1.f, 1.f,
	 0.f, 0.5f }
};

	std::vector<unsigned int> diceOrder = {
		0, 1, 2,
		2, 3, 0,
		1, 4, 5,
		5, 2, 1,
		3, 2, 8,
		6, 7, 1,
		1, 0, 6,
		3, 2, 8,
		8, 9, 3,
		10, 0, 3,
		3, 11, 10,
		12, 10, 11,
		11, 13, 12
	};

	Mesh cubeMesh(vertices, order);
	Mesh diceMesh(diceVertices, diceOrder);
	Entity cube;
	Entity cube2;
	auto comp = cube.CreateComponent<GraphicsComponent>();
	comp->mesh = std::make_shared<Mesh>(diceMesh);
	auto comp2 = cube2.CreateComponent<GraphicsComponent>();
	comp2->mesh = std::make_shared<Mesh>(cubeMesh);
	
	cube.Translate(-0.5f, 0.f, -3.f);
	cube2.Translate(0.5f, 0.2f, -5.f);

	auto renderer = Renderer::GetInstance();
	comp->texture = renderer->TextureFactory("dice.png");
	renderer->SetPerspective(90, 0.1f, 100.f);
	renderer->AddObject(cube);
	renderer->AddObject(cube2);
	bool flag = true;
	std::thread objThread(rotateObj, std::ref(cube), 2.f, std::ref(flag));
	std::thread objThread2(rotateObj, std::ref(cube2), 3.f, std::ref(flag));
	renderer->Run();
	flag = false;
	objThread.join();
	objThread2.join();

}