import Common;
import Graphics;
import Graphics.Resources;
import Graphics.Components;
import MeshHelpers;

import <glm/vec3.hpp>;

import <iostream>;
import <thread>;
import <chrono>;
import <memory>;

void RotateAxis2D(Entity& obj, float theta, bool& flag) 
{
	while (flag) {
		obj.Rotate(0.f, 1.f, 0.f, theta);
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

		float zDist = glm::length(obj.position) * (cosf(glm::radians(totalTheta)) - cosf(glm::radians(previousTheta)));
		float xDist = glm::length(obj.position) * (sinf(glm::radians(totalTheta)) - sinf(glm::radians(previousTheta)));

		obj.Translate(xDist, 0.f, zDist);
		obj.Rotate(0.f, 1.f, 0.f, theta);

		std::this_thread::sleep_for(std::chrono::milliseconds(8));
	}
}


int main() 
{
	std::vector<Vertex> vertices = {

		//Front Face
		//B L
		{-0.5f, -0.5, 0.5,
		0.5f, 0.25f},
		//B R
		{0.5f, -0.5, 0.5,
		0.75f, 0.25f},
		//T R
		{0.5f, 0.5, 0.5,
		0.75f, 0.5f},
		//T L
		{-0.5f, 0.5, 0.5,
		0.5f, 0.5f},

		//Right Face
		//B L
		{0.5f, -0.5, 0.5,
		0.75f, 0.25f},
		//B R
		{0.5f, -0.5, -0.5,
		1.f, 0.25f},
		//T R
		{0.5f, 0.5, -0.5,
		1.f, 0.5f},
		//T L
		{0.5f, 0.5f, 0.5f,
		0.75f, 0.5f},

		//Back Face
		//B L
		{0.5f, -0.5, -0.5,
		0.f, 0.25f},
		//B R
		{-0.5f, -0.5, -0.5,
		0.25f, 0.25f},
		//T R
		{-0.5f, 0.5, -0.5,
		0.25f, 0.5f},
		//T L
		{0.5f, 0.5, -0.5,
		0.f, 0.5f},

		//Left Face
		//B L
		{-0.5f, -0.5, -0.5,
		0.25f, 0.25f},
		//B R
		{-0.5f, -0.5, 0.5,
		0.5f, 0.25f},
		//T R
		{-0.5f, 0.5, 0.5,
		0.5f, 0.5f},
		//T L
		{-0.5f, 0.5, -0.5,
		0.25f, 0.5f},

		//Top Face
		//B L
		{-0.5f, 0.5, 0.5,
		0.5f, 0.5f},
		//B R
		{0.5f, 0.5, 0.5,
		0.75f, 0.5f},
		//T R
		{0.5f, 0.5, -0.5,
		0.75f, 0.75f},
		//T L
		{-0.5f, 0.5, -0.5,
		0.5f, 0.75f},

		//Bottom Face
		//B L
		{-0.5f, -0.5, -0.5,
		0.5f, 0.f},
		//B R
		{0.5f, -0.5, -0.5,
		0.75f, 0.f},
		//T R
		{0.5f, -0.5, 0.5,
		0.75f, 0.25f},
		//T L
		{-0.5f, -0.5, 0.5,
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

	auto& window = Window::Initialize("TVGE v0.11A", 800, 600);

	auto& renderer = window.GetRenderer();

	auto defaultShader = renderer.GenerateShader("shader.vert", "shader.frag");

	auto basicMaterial = std::make_shared<Material>(*defaultShader);
	basicMaterial->_lightProperties.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	basicMaterial->_lightProperties.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	basicMaterial->_lightProperties.specular = glm::vec3(0.1f, 0.1f, 0.1f);
	basicMaterial->_lightProperties.shininess = 10.f;

	auto emerald = std::make_shared<Material>(*defaultShader);
	emerald->_lightProperties.ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
	emerald->_lightProperties.diffuse = glm::vec3(0.07568f, .61424f, 0.07568f);
	emerald->_lightProperties.specular = glm::vec3(0.633f, 0.727811f, 0.633f);
	emerald->_lightProperties.shininess = 0.6f * 128.f;


	auto lightMaterial = std::make_shared<Material>(*defaultShader);
	lightMaterial->_lightProperties.emissive = glm::vec3(1.f, 1.f, 1.f);

	Entity sphere;
	auto sphereComp = sphere.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*sphereComp, "monke.dae");
	sphere.Translate(0.f, 0.f, -5.f);
	sphere.Rotate(glm::vec3(-1.f, 0.f, 0.f), 90.f);

	Entity cube;
	auto diceComp = cube.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*diceComp, "cube.dae");

	cube.Translate(0.f, 0.f, 5.f);

	Entity backpack;
	auto bkpComp = backpack.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*bkpComp, "backpack.obj");

	backpack.Translate(10.f, 0.f, 0.f);
	backpack.Rotate(glm::vec3(0.f, 1.f, 0.f), -90.f);

	Entity light;
	auto lightComp = light.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*lightComp, "sphere.dae");
	lightComp->_meshes[0]->_material = lightMaterial;
	light.CreateComponentOfType<PointLightComponent>(glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.f, 1.f, 1.f));
	light.Scale(0.1f, 0.1f, 0.1f);
	light.Translate(0.f, 0.f, -1.f);

	Entity light2;
	auto lightComp2 = light2.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*lightComp2, "sphere.dae");
	lightComp2->_meshes[0]->_material = lightMaterial;
	light2.CreateComponentOfType<PointLightComponent>(glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.f, 1.f, 1.f));
	light2.Scale(0.1f, 0.1f, 0.1f);
	light2.Translate(14.f, 0.f, 14.f);

	Entity light3;
	auto lightComp3 = light3.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*lightComp3, "sphere.dae");
	lightComp3->_meshes[0]->_material = lightMaterial;
	light3.CreateComponentOfType<PointLightComponent>(glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.f, 1.f, 1.f));
	light3.Scale(0.1f, 0.1f, 0.1f);
	light3.Translate(-14.f, 0.f, -14.f);

	Entity floor;
	auto floorComp = floor.CreateComponentOfType<ModelComponent>();
	floorComp.lock()->_meshes.push_back(renderer.GenerateMesh(vertices, order, basicMaterial, true));
	floor.Scale(100.f, 0.1f, 100.f);
	floor.Translate(0.f, -5.f, 0.f);
	
	renderer.AddObject(cube);
	renderer.AddObject(light);
	renderer.AddObject(light2);
	renderer.AddObject(light3);
	renderer.AddObject(floor);
	renderer.AddObject(sphere);
	renderer.AddObject(backpack);
	renderer.AddLightSource(light);
	renderer.AddLightSource(light2);
	renderer.AddLightSource(light3);

	renderer.SetPerspective(90.f, 0.1f, 100.f);

	renderer.InitializeTime();
	while (window.IsOpen()) {
		renderer.ComputeTime();
		renderer.ProcessInput();
		renderer.RenderFrame();
	}
}