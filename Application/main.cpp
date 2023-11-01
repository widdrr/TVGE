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
		0.5f, 0.33f},
		//B R
		{0.5f, -0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75f, 0.33f},
		//T R
		{0.5f, 0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75f, 0.66f},
		//T L
		{-0.5f, 0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.66f},

		//Right Face
		//B L
		{0.5f, -0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75f, 0.33f},
		//B R
		{0.5f, -0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		1.f, 0.33f},
		//T R
		{0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		1.f, 0.66f},
		//T L
		{0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75f, 0.33f},

		//Back Face
		//B L
		{0.5f, -0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.f, 0.33f},
		//B R
		{-0.5f, -0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.25f, 0.33f},
		//T R
		{-0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.25f, 0.66f},
		//T L
		{0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.f, 0.66f},

		//Left Face
		//B L
		{-0.5f, -0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.25f, 0.33f},
		//B R
		{-0.5f, -0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.33f},
		//T R
		{-0.5f, 0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.66f},
		//T L
		{-0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.25f, 0.66f},

		//Top Face
		//B L
		{-0.5f, 0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.66f},
		//B R
		{0.5f, 0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75f, 0.66f},
		//T R
		{0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.75f, 0.1f},
		//T L
		{-0.5f, 0.5, -0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.1f},

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
		0.75f, 0.33f},
		//T L
		{-0.5f, -0.5, 0.5,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.3f}
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

	Entity sun;
	auto sunData = CommonMeshes::Circle(50, 36);
	auto sunComp = sun.CreateComponent<RenderComponent>().lock();
	sunComp->mesh = renderer.MeshFactory(sunData.first, sunData.second, GL_TRIANGLE_FAN);
	sunComp->texture = renderer.TextureFactory("sun.png");

	Entity earth;
	earth.SetParent(sun);
	auto earthData = CommonMeshes::Circle(30, 36);
	auto earthComp = earth.CreateComponent<RenderComponent>().lock();
	earthComp->mesh = renderer.MeshFactory(earthData.first, earthData.second, GL_TRIANGLE_FAN);
	earthComp->texture = renderer.TextureFactory("earth.png");
	earth.Translate(200.f, 0.f, 0.f);

	Entity moon;
	moon.SetParent(earth);
	auto moonData = CommonMeshes::Circle(20, 36);
	auto moonComp = moon.CreateComponent<RenderComponent>().lock();
	moonComp->mesh = renderer.MeshFactory(moonData.first, moonData.second, GL_TRIANGLE_FAN);
	moonComp->texture = renderer.TextureFactory("moon.png");
	moon.Translate(75.f, 0.f, 0.f);

	renderer.Set2DMode(800, 600);

	renderer.AddObject(sun);
	renderer.AddObject(earth);
	renderer.AddObject(moon);

	bool flag = true;
	std::thread earthOrbit(OrbitParentEntity2D, std::ref(earth), 1.f, std::ref(flag));
	std::thread earthRotation(RotateAxis2D, std::ref(earth), 2.f, std::ref(flag));
	std::thread moonOrbit(OrbitParentEntity2D, std::ref(moon), -1.f, std::ref(flag));
	
	renderer.Run();
	flag = false;
	earthOrbit.join();
	earthRotation.join();
	moonOrbit.join();
}