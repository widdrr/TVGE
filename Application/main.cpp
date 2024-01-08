import Common;
import Graphics;
import Graphics.Resources;
import Graphics.Components;
import MeshHelpers;
import Physics;
import Physics.Components;

import <glm/vec3.hpp>;

import <iostream>;
import <thread>;
import <chrono>;
import <memory>;
import <random>;

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

	auto& window = Window::Initialize("TVGE v0.13A", 800, 600);

	auto& renderer = window.GetRenderer();

	auto defaultShader = renderer.GenerateShader("shader.vert", "shader.frag");

	auto basicMaterial = std::make_shared<Material>(*defaultShader);
	basicMaterial->_lightProperties.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	basicMaterial->_lightProperties.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	basicMaterial->_lightProperties.specular = glm::vec3(0.1f, 0.1f, 0.1f);
	basicMaterial->_lightProperties.shininess = 10.f;

	auto grassMaterial = std::make_shared<Material>(*defaultShader);
	grassMaterial->_diffuseMap = renderer.GenerateTexture2D("grass-texture-background.jpg", true);
	grassMaterial->_lightProperties.shininess = 0.f;

	Entity moon;
	moon.CreateComponentOfType<DirectionalLightComponent>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.01f, 1.01f, 1.01f), glm::vec3(0.f, 0.f, 0.f));

	Entity floor;
	auto floorComp = floor.CreateComponentOfType<ModelComponent>();
	for (auto&& vertex : vertices) {
		vertex._textureCoordinates *= 100;
	}

	floorComp.lock()->_meshes.push_back(renderer.GenerateMesh("Cube", vertices, order, grassMaterial, true));
	floor.Scale(100.f, 0.1f, 100.f);
	floor.Translate(0.f, -8.f, 0.f);

	std::random_device rd;  // Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd());
	std::uniform_real_distribution xDistribution(-4.f, 5.f);
	std::uniform_real_distribution zDistribution(-5.5f, 4.5f);
	std::uniform_real_distribution scaleDistribution(0.03, 0.05);

	std::vector<Entity> trees(20);
	for (auto&& tree : trees) {
		auto treeComp = tree.CreateComponentOfType<ModelComponent>().lock();

		float scale = scaleDistribution(gen);

		tree.Scale(scale, scale, scale);
		tree.Rotate(glm::vec3(-1.f, 0.f, 0.f), 90);

		float x, z;
		do {
			x = xDistribution(gen) * 10;
			z = zDistribution(gen) * 10;
		} while (std::sqrtf((x - 10) * (x - 10) + (z - 10) * (z - 10)) <= 20.f);

		tree.Translate(x, -9.f, z);
		renderer.LoadModel(*treeComp, "tree.obj", false);
		renderer.AddObject(tree);
	}

	Entity fire;
	auto fireModelComp = fire.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*fireModelComp, "fireme.obj");
	auto flameLightComp = fire.CreateComponentOfType<PointLightComponent>(glm::vec3(0.f, 0.f, 0.f),
																		  glm::vec3(1.000000, 0.372789, 0.021186),
																		  glm::vec3(1.000000, 0.372789, 0.021186),
																		  glm::vec3(0.f, 4.f, 0.f),
																		  0.017f, 0.017f, 1).lock();

	fire.Translate(0.f, -8.f, 0.5);

	Entity tent;
	auto tentModelComp = tent.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*tentModelComp, "tent_low_uvfix.fbx");
	tent.Scale(0.04f, 0.04f, 0.04f);
	tent.Rotate(glm::vec3(0.f, 1.f, 0.f), -126.f);
	tent.Translate(8.f, -6.45f, 6.f);

	Entity log1;
	auto log1ModelComp = log1.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*log1ModelComp, "log.dae", false);
	log1.Scale(0.03f, 0.03f, 0.03f);
	log1.Translate(-7.f, -8.f, 7.f);
	log1.Rotate(glm::vec3(0.f, 1.f, 0.f), -45.f);

	Entity log2;
	auto log2ModelComp = log2.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*log2ModelComp, "log.dae", false);
	log2.Scale(0.03f, 0.03f, 0.03f);
	log2.Translate(-1.f, -8.f, -6.f);
	log2.Rotate(glm::vec3(0.f, 1.f, 0.f), 0.f);

	renderer.AddObject(floor);
	renderer.AddObject(fire);
	renderer.AddObject(tent);
	renderer.AddObject(log1);
	renderer.AddObject(log2);
	renderer.AddLightSource(moon);
	renderer.AddLightSource(fire);
	renderer.SetShadowCaster(fire);

	renderer.SetPerspective(90.f, 0.1f, 100.f);


	auto normals = renderer.GenerateShader("normal.vert", "normal.frag", "normal.geom");

	renderer.SetSkybox("StarSkybox041.png", "StarSkybox042.png", "StarSkybox043.png", "StarSkybox044.png", "StarSkybox045.png", "StarSkybox046.png");

	renderer.InitializeTime();
	while (window.IsOpen()) {
		renderer.ComputeTime();
		renderer.ProcessInput();
		renderer.RenderFrame();
		//renderer.RenderFrame(*normals);
		renderer.DisplayScene();
	}
}