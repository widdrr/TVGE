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

	Entity sun;
	sun.CreateComponentOfType<DirectionalLightComponent>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.f, 0.f, 0.f));

	Entity floor;
	auto floorComp = floor.CreateComponentOfType<ModelComponent>();
	floorComp.lock()->_meshes.push_back(renderer.GenerateMesh("Cube", vertices, order, basicMaterial, true));
	floor.Scale(100.f, 0.1f, 100.f);
	floor.Translate(0.f, -8.f, 0.f);

	Entity tree;
	auto treeComp = tree.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*treeComp, "tree.obj", false);
	tree.Scale(0.03f, 0.03f, 0.03f);
	tree.Rotate(glm::vec3(-1.f, 0.f, 0.f), 90);
	tree.Translate(0.f, -8.f, 0.f);

	Entity tree2;
	auto treeComp2 = tree2.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*treeComp2, "tree.obj", false);
	tree2.Scale(0.05f, 0.05f, 0.05f);
	tree2.Rotate(glm::vec3(-1.f, 0.f, 0.f), 90);
	tree2.Translate(10.f, -8.f, 20.f);

	Entity tree3;
	auto treeComp3 = tree3.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*treeComp3, "tree.obj", false);
	tree3.Scale(0.04f, 0.04f, 0.04f);
	tree3.Rotate(glm::vec3(-1.f, 0.f, 0.f), 90);
	tree3.Translate(-10.f, -8.f, 10.f);

	Entity fire;
	auto fireModelComp = fire.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*fireModelComp, "fireme.obj");
	auto flameLightComp = fire.CreateComponentOfType<PointLightComponent>(glm::vec3(0.f, 0.f, 0.f),
																		  glm::vec3(1.000000, 0.372789, 0.021186),
																		  glm::vec3(1.000000, 0.372789, 0.021186),
																		  glm::vec3(0.f, 3.f, 0.f),
																		  0, 0, 1).lock();

	fire.Translate(0.f, -8.f, 0.5);

	renderer.AddObject(floor);
	renderer.AddObject(tree);
	renderer.AddObject(tree2);
	renderer.AddObject(tree3);
	renderer.AddObject(fire);
	renderer.AddLightSource(sun);
	renderer.AddLightSource(fire);
	renderer.SetShadowCaster(fire);

	renderer.SetPerspective(90.f, 0.1f, 100.f);


	auto normals = renderer.GenerateShader("normal.vert", "normal.frag", "normal.geom");

	renderer.SetSkybox("back.jpg", "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg");

	renderer.InitializeTime();
	while (window.IsOpen()) {
		renderer.ComputeTime();
		renderer.ProcessInput();
		renderer.RenderFrame();
		//renderer.RenderFrame(*normals);
		renderer.DisplayScene();
	}
}