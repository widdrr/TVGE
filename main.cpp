#include "Renderer.h"
#include <thread>
#include <chrono>
void rotateObj(Object& obj, float theta, bool& flag)
{
	while (flag) {
		
		obj.Rotate(1.f, 1.f, 0.f, theta);
		std::this_thread::sleep_for(std::chrono::milliseconds(4));
	}
}

int main() {

	//TODO: make a builder for this mess
	std::vector<Vertex> vertices = {
		{0.5f, 0.5f, 0.5f,
		 1.f, 1.f, 1.f},
		
		{-0.5f, 0.5f, -0.5f,
		 0.f, 1.f, 0.f},
		
		{-0.5f, 0.5f, 0.5f,
		 0.f, 1.f, 1.f},
		
		{0.5f, -0.5f, -0.5f,
		 1.f, 0.f, 0.f},
		
		{-0.5f, -0.5f, -0.5f,
		 0.f, 0.f, 0.f},
		
		{0.5f, 0.5f, -0.5f,
		 1.f, 1.f, 0.f},
		
		{0.5f, -0.5f, 0.5f,
		 1.f, 0.f, 1.f},
		
		{-0.5f, -0.5f, 0.5f,
		 0.f, 0.f, 1.f} };
	
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
	Object cube2(cubeMesh);
	cube.Translate(0.5f, 0.f, -2.5f);
	cube2.Scale(0.5f, 0.5f, 0.5f);
	cube2.Translate(-0.5f, 0.5f, -2.f);
	auto renderer = Renderer::GetInstance();
	renderer->SetPerspective(90, 0.1f, 10.f);
	renderer->AddObject(cube);
	renderer->AddObject(cube2);
	bool flag = true;
	std::thread objThread(rotateObj, std::ref(cube), 1.f, std::ref(flag));
	std::thread objThread2(rotateObj, std::ref(cube2), 1.5f, std::ref(flag));
	renderer->Run();
	flag = false;
	objThread.join();
	objThread2.join();

}