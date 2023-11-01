import Common;
import Graphics.Components;
import Graphics.Resources;
import Rendering;
import MeshHelpers;

#include <GL/glew.h>

import <glm/vec3.hpp>;

import <iostream>;
import <thread>;
import <chrono>;
import <memory>;


void RotateAxis2D(Entity& obj, float theta, bool& flag) 
{
	while (flag) {

		obj.Rotate(0.f, 0.f, 1.f, theta);
		std::this_thread::sleep_for(std::chrono::milliseconds(8));
	}
}

void OrbitParentEntity2D(Entity& obj, float theta, bool& flag) 
{
	float totalTheta = 0.f;

	while (flag) {

		auto previousTheta = totalTheta;
		totalTheta += theta;
		totalTheta = fmodf(totalTheta + 360, 360.f);

		float xDist = glm::length(obj.position) * (cosf(glm::radians(totalTheta)) - cosf(glm::radians(previousTheta)));
		float yDist = glm::length(obj.position) * (sinf(glm::radians(totalTheta)) - sinf(glm::radians(previousTheta)));

		obj.Translate(xDist, yDist, 0);
		obj.Rotate(0.f, 0.f, 1.f, theta);

		std::this_thread::sleep_for(std::chrono::milliseconds(8));
	}
}

int main() 
{
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
		0.75f, 0.5f},

		//Back Face
		//B L
		{0.5f, -0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.f, 0.25f},
		//B R
		{-0.5f, -0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.25f, 0.25f},
		//T R
		{-0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.25f, 0.5f},
		//T L
		{0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.f, 0.5f},

		//Left Face
		//B L
		{-0.5f, -0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.25f, 0.25f},
		//B R
		{-0.5f, -0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.25f},
		//T R
		{-0.5f, 0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.5f},
		//T L
		{-0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.25f, 0.5f},

		//Top Face
		//B L
		{-0.5f, 0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.5f},
		//B R
		{0.5f, 0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75f, 0.5f},
		//T R
		{0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75f, 0.75f},
		//T L
		{-0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.75f},

		//Bottom Face
		//B L
		{-0.5f, -0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.f},
		//B R
		{0.5f, -0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75f, 0.f},
		//T R
		{0.5f, -0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75f, 0.25f},
		//T L
		{-0.5f, -0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.25f}
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

	auto& renderer = Renderer::GetInstance();

	Entity dice;
	auto diceComp = dice.CreateComponentOfType<RenderComponent>();
	diceComp.lock()->mesh = renderer.MeshFactory(vertices, order);
	diceComp.lock()->texture = renderer.TextureFactory("dice.png");

	dice.Translate(0.f, 0.f, 5.f);

	Entity light;
	auto lightComp = light.CreateComponentOfType<RenderComponent>();
	lightComp.lock()->mesh = renderer.MeshFactory(vertices, order);
	auto lightSource = light.CreateComponentOfType<LightSourceComponent>();
	lightComp.lock()->shaderProgram = renderer.ShaderFactory("shader.vert", "shaderLightSource.frag");
	light.Scale(0.1f, 0.1f, 0.1f);
	light.Translate(0.f, 0.f, -1.f);

	Entity floor;
	auto floorComp = floor .CreateComponentOfType<RenderComponent>();
	floorComp.lock()->mesh = renderer.MeshFactory(vertices, order);
	floor.Scale(100.f, 0.1f, 100.f);
	floor.Translate(0.f, -5.f, 0.f);
	
	renderer.AddObject(dice);
	renderer.AddObject(light);
	renderer.AddObject(floor);
	renderer.SetLightSource(light);

	renderer.SetPerspective(90.f, 0.1f, 100.f);

	renderer.Run();
}