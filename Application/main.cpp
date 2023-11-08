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
import <random>;


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

void Flash(Entity& obj, bool& flag)
{
	auto component = obj.TryGetComponentOfType<RenderComponent>().lock();
	std::random_device seed;

	std::default_random_engine gen(seed());
	std::uniform_int_distribution<> dist(500, 3000);

	while (flag) {

		component->disabled = false;

		std::this_thread::sleep_for(std::chrono::milliseconds(dist(gen)));

		component->disabled = true;

		std::this_thread::sleep_for(std::chrono::milliseconds(dist(gen)));
	}
}

int main()
{
	auto& renderer = Renderer::GetInstance();

	Entity sun;
	auto sunData = Common2DMeshes::Circle(50, 36);
	auto sunComp = sun.CreateComponentOfType<RenderComponent>().lock();
	sunComp->mesh = renderer.MeshFactory(sunData.first, sunData.second, GL_TRIANGLE_FAN);
	sunComp->texture = renderer.TextureFactory("sun.png");

	Entity earth;
	earth.SetParent(sun);
	auto earthData = Common2DMeshes::Circle(30, 36);
	auto earthComp = earth.CreateComponentOfType<RenderComponent>().lock();
	earthComp->mesh = renderer.MeshFactory(earthData.first, earthData.second, GL_TRIANGLE_FAN);
	earthComp->texture = renderer.TextureFactory("earth.png");
	earth.Translate(200.f, 0.f, 0.f);

	Entity moon;
	moon.SetParent(earth);
	auto moonData = Common2DMeshes::Circle(20, 36);
	auto moonComp = moon.CreateComponentOfType<RenderComponent>().lock();
	moonComp->mesh = renderer.MeshFactory(moonData.first, moonData.second, GL_TRIANGLE_FAN);
	moonComp->texture = renderer.TextureFactory("moon.png");
	moon.Translate(75.f, 0.f, 0.f);

	auto starData0 = Common2DMeshes::Rectangle(30, 30);
	auto& v = starData0.first;
	v[0]._textureCoordinates.s = 0.f;
	v[0]._textureCoordinates.t = 0.f;
	v[1]._textureCoordinates.s = 0.2f;
	v[1]._textureCoordinates.t = 0.f;
	v[2]._textureCoordinates.s = 0.2f;
	v[2]._textureCoordinates.t = 1.f;
	v[3]._textureCoordinates.s = 0.f;
	v[3]._textureCoordinates.t = 1.f;

	auto starData1 = Common2DMeshes::Rectangle(30, 30);
	auto& v1 = starData1.first;
	v1[0]._textureCoordinates.s = 0.2f;
	v1[0]._textureCoordinates.t = 0.f;
	v1[1]._textureCoordinates.s = 0.4f;
	v1[1]._textureCoordinates.t = 0.f;
	v1[2]._textureCoordinates.s = 0.4f;
	v1[2]._textureCoordinates.t = 1.f;
	v1[3]._textureCoordinates.s = 0.2f;
	v1[3]._textureCoordinates.t = 1.f;

	auto starData2 = Common2DMeshes::Rectangle(30, 30);
	auto& v2 = starData2.first;
	v2[0]._textureCoordinates.s = 0.4f;
	v2[0]._textureCoordinates.t = 0.f;
	v2[1]._textureCoordinates.s = 0.6f;
	v2[1]._textureCoordinates.t = 0.f;
	v2[2]._textureCoordinates.s = 0.6f;
	v2[2]._textureCoordinates.t = 1.f;
	v2[3]._textureCoordinates.s = 0.4f;
	v2[3]._textureCoordinates.t = 1.f;

	auto starData3 = Common2DMeshes::Rectangle(30, 30);
	auto& v3 = starData3.first;
	v3[0]._textureCoordinates.s = 0.6f;
	v3[0]._textureCoordinates.t = 0.f;
	v3[1]._textureCoordinates.s = 0.8f;
	v3[1]._textureCoordinates.t = 0.f;
	v3[2]._textureCoordinates.s = 0.8f;
	v3[2]._textureCoordinates.t = 1.f;
	v3[3]._textureCoordinates.s = 0.6f;
	v3[3]._textureCoordinates.t = 1.f;

	auto starData4 = Common2DMeshes::Rectangle(30, 30);
	auto &v4 = starData4.first;
	v4[0]._textureCoordinates.s = 0.8f;
	v4[0]._textureCoordinates.t = 0.f;
	v4[1]._textureCoordinates.s = 1.f;
	v4[1]._textureCoordinates.t = 0.f;
	v4[2]._textureCoordinates.s = 1.f;
	v4[2]._textureCoordinates.t = 1.f;
	v4[3]._textureCoordinates.s = 0.8f;
	v4[3]._textureCoordinates.t = 1.f;

	auto star0 = renderer.MeshFactory(starData0.first, starData0.second);
	auto star1 = renderer.MeshFactory(starData1.first, starData1.second);
	auto star2 = renderer.MeshFactory(starData2.first, starData2.second);
	auto star3 = renderer.MeshFactory(starData3.first, starData3.second);
	auto star4 = renderer.MeshFactory(starData4.first, starData4.second);

	std::vector<std::shared_ptr<Mesh>> starMeshes({ star0, star1, star2, star3, star4 });

	auto starsTexture = renderer.TextureFactory("star_textures.png");
	
	std::random_device seed;
	std::default_random_engine gen(seed());

	std::uniform_real_distribution<float> xDist(-400.f, 400.f);
	std::uniform_real_distribution<float> yDist(-300.f, 300.f);
	std::uniform_real_distribution<float> scaleDist(0.5f, 2.f);

	constexpr int numStars = 50;
	std::vector<Entity> stars;
	stars.reserve(numStars);
	std::vector<std::thread> starThreads;
	bool flag = true;

	for (auto i = 0; i < numStars; ++i) {
		stars.emplace_back();
		stars[i].Translate(xDist(gen), yDist(gen), -1.f);
		float scale = scaleDist(gen);
		stars[i].Scale(scale, scale, 1.f);
		auto comp = stars[i].CreateComponentOfType<RenderComponent>().lock();
		comp->texture = starsTexture;
		comp->mesh = starMeshes[i % 5];
		comp->disabled = true;
		renderer.AddObject(stars[i]);
	}
	for (auto& star : stars) {
		starThreads.emplace_back(Flash, std::ref(star), std::ref(flag));
	}

	renderer.Set2DMode(800, 600);

	renderer.AddObject(sun);
	renderer.AddObject(earth);
	renderer.AddObject(moon);

	std::thread sunRotation(RotateAxis2D, std::ref(sun), 0.1f, std::ref(flag));
	std::thread earthOrbit(OrbitParentEntity2D, std::ref(earth), 0.1f, std::ref(flag));
	std::thread earthRotation(RotateAxis2D, std::ref(earth), 1.f, std::ref(flag));
	std::thread moonOrbit(OrbitParentEntity2D, std::ref(moon), -0.1f, std::ref(flag));

	renderer.Run();
	
	flag = false;
	sunRotation.join();
	earthOrbit.join();
	earthRotation.join();
	moonOrbit.join();
	for (auto& thread : starThreads) {
		thread.join();
	}
}