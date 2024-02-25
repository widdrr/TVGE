import Common;
import Graphics;
import Graphics.Resources;
import Graphics.Components;
import MeshHelpers;
import Physics;
import Physics.Components;

import <iostream>;
import <thread>;
import <chrono>;
import <memory>;
import <random>;

//for some ungodly reason, main does not compile without this
static void DontDeleteThis(Entity& obj)
{
	obj.Translate(0.f, 0.f, 0.f);
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

	auto& window = Window::Initialize("TVGE v0.2A", 800, 600);

	auto& renderer = window.GetRenderer();
	auto& input = window.GetInput();

	auto defaultShader = renderer.GenerateShader("shader.vert", "shader.frag");
	auto normals = renderer.GenerateShader("normal.vert", "normal.frag", "normal.geom");

	auto basicMaterial = std::make_shared<Material>(*defaultShader);
	basicMaterial->_lightProperties.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	basicMaterial->_lightProperties.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	basicMaterial->_lightProperties.specular = glm::vec3(0.1f, 0.1f, 0.1f);
	basicMaterial->_lightProperties.shininess = 10.f;

	Entity moon;
	moon.CreateComponentOfType<DirectionalLightComponent>(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.f, 0.f, 0.f));

	Entity floor;
	auto floorComp = floor.CreateComponentOfType<ModelComponent>().lock();
	auto floorCollider = floor.CreateComponentOfType<BoxColliderComponent>().lock();

	floorComp->_meshes.push_back(renderer.GenerateMesh("Cube", vertices, order, basicMaterial, true));
	
	Entity testFloor(floor);
	testFloor.Translate(0.f, -7.f, -10.f);
	testFloor.Rotate(0.f, 1.f, 0.f, 90.f);
	testFloor.Rotate(1.f, 0.f, 0.f, 45.f);

	floor.Scale(100.f, 0.1f, 100.f);
	floor.Translate(0.f, -8.f, 0.f);

	Entity cube;
	auto cubeComp = cube.CreateComponentOfType<ModelComponent>().lock();
	cubeComp->_meshes.push_back(renderer.GenerateMesh("Cube"));
	cube.CreateComponentOfType<BoxColliderComponent>();

	Entity testCube(cube);
	testCube.CreateComponentOfType<BodyComponent>(0.1f);
	cube.Scale(5.f, 0.1f, 5.f);
	cube.Translate(0.f, 2.f, -10.f);

	Entity cube2(cube);

	cube.Translate(-5.f, 0.f, 0.f);
	cube.Rotate(0.f, 0.f, 1.f, -45.f);

	cube2.Translate(5.f, 0.f, 0.f);
	cube2.Rotate(0.f, 0.f, 1.f, 45.f);

	testCube.Translate(0.f, 2.f, -10.f);
	testCube.Rotate(1.f, 0.f, 0.f, 45.f);
	testCube.Rotate(0.f, 0.f, 1.f, 45.f);

	/*Entity sphere;
	sphere.Translate(0.f, 10.f, -10.f);
	auto sphereModel = sphere.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*sphereModel, "sphere.dae");
	auto sphereCollider = sphere.CreateComponentOfType<SphereColliderComponent>(1.f).lock();
	auto sphereBody = sphere.CreateComponentOfType<BodyComponent>(10.f).lock();

	Entity sphere2(sphere);

	sphere.Translate(-5.f, 0.f, 0.f);
	sphere2.Translate(5.f, 0.f, 0.f);*/

	renderer.AddObject(cube);
	renderer.AddObject(cube2);
	renderer.AddObject(testCube);
	renderer.AddObject(floor);
	renderer.AddObject(testFloor);
	//renderer.AddObject(sphere);
	//renderer.AddObject(sphere2);
	renderer.AddLightSource(moon);

	renderer.SetPerspective(90.f, 0.1f, 100.f);

	renderer.SetSkybox("StarSkybox041.png", "StarSkybox042.png", "StarSkybox043.png", "StarSkybox044.png", "StarSkybox045.png", "StarSkybox046.png");

	auto& camera = renderer.GetMainCamera();

	Simulator simulator;

	simulator.AddObject(floor);
	simulator.AddObject(cube);
	simulator.AddObject(cube2);
	simulator.AddObject(testFloor);
	simulator.AddObject(testCube);
	//simulator.AddObject(sphere);
	//simulator.AddObject(sphere2);

	bool initialFocus = true;
	double prevX = 0, prevY = 0;
	input.AddKeyPressEventHandler(Keys::ESCAPE, [&]() {window.Unfocus(); });

	input.AddMouseButtonPressEventHandler(MouseButtons::LEFT_CLICK, [&]() {window.Focus(); initialFocus = true; });
	Camera::Movement movement{};

	input.AddCursorPositionEventHandler([&](double p_crtX, double p_crtY) {
		if (window.IsFocused()) {

			if (initialFocus) {
				prevX = p_crtX;
				prevY = p_crtY;
				initialFocus = false;
			}
			else {
				float offsetX = static_cast<float>(p_crtX - prevX);
				float offsetY = static_cast<float>(prevY - p_crtY);
				prevX = p_crtX;
				prevY = p_crtY;

				camera.RotateCamera(offsetX, offsetY);
			}
		}});

	input.AddGenericInputBehaviour([&]() {
		movement.moveForward = input.IsKeyPressed(W);
		movement.moveBackward = input.IsKeyPressed(S);
		movement.moveLeft = input.IsKeyPressed(A);
		movement.moveRight = input.IsKeyPressed(D);
		movement.moveUp = input.IsKeyPressed(SPACE);
		movement.moveDown = input.IsKeyPressed(LEFT_SHIFT);

		auto delta = window.GetDeltaTime();
		camera.MoveCamera(movement, delta);
								   });
	bool showNormals = false;
	input.AddKeyPressEventHandler(Keys::N, [&]() {showNormals = !showNormals; });
	bool renderWireframe = false;
	input.AddKeyPressEventHandler(Keys::M, [&]() {renderWireframe = !renderWireframe; });

	window.InitializeTime();
	while (window.IsOpen()) {
		auto deltaTime = window.ComputeDeltaTime();
		window.ComputeFPS();
		input.ProcessInput();
		simulator.SimulateStep(deltaTime);
		if (!renderWireframe) {
			renderer.RenderFrame();
		}
		else {
			renderer.RenderWireframe();
		}
		if (showNormals) {
			renderer.RenderFrame(*normals);
		}
		renderer.DisplayScene();
	}
}