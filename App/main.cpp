#include "Renderer.h"

//TODO: transition to modules :)
import <thread>;
import <chrono>;

void rotateObj(Entity& obj, float theta, bool& flag)
{
	while (flag) {
		
		obj.Rotate(0.f, 1.f, 0.f, theta,
			0.f, 0.f, -5.f);
		std::this_thread::sleep_for(std::chrono::milliseconds(4));
	}
}

int main() {

	//TODO: make a builder for this mess
	//Just make the model loader instead
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

	Mesh cubeMesh(vertices, order);
	Entity cube;
	Entity light;
	Entity cube3;
	auto comp = cube.CreateComponent<GraphicsComponent>();
	comp->mesh = std::make_shared<Mesh>(cubeMesh);
	
	
	auto comp2 = light.CreateComponent<GraphicsComponent>();
	comp2->mesh = std::make_shared<Mesh>(cubeMesh);
	light.CreateComponent<LightSourceComponent>(1.f,1.f,1.f, 0.3f, 0.3f);
	
	auto comp3 = cube3.CreateComponent<GraphicsComponent>();
	comp3->mesh = std::make_shared<Mesh>(cubeMesh);
	
	cube.Translate(0.f, -5.f, 0.f);
	cube.Scale(100.f, .1f, 100.f);
	light.Scale(0.1f, 0.1f, 0.1f);
	
	light.Translate(0.f, 0.f, -5.f);
	cube3.Translate(0.f, 0.f, 0);

	auto renderer = Renderer::GetInstance();
	
	comp3->texture = renderer->TextureFactory("dice.png");
	comp2->shaderProgram = renderer->ShaderFactory("shader.vert", "shaderLightSource.frag");
	renderer->SetPerspective(90, 0.1f, 100.f);
	renderer->SetLightSource(light);
	renderer->AddObject(cube);
	renderer->AddObject(light);
	renderer->AddObject(cube3);
	
	bool flag = true;
	std::thread objThread(rotateObj, std::ref(cube3), 1.f, std::ref(flag));
	renderer->Run();
	flag = false;
	objThread.join();

}