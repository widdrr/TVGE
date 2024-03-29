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
import <format>;


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

	auto& window = Window::Initialize("TVGE v0.7A", 1024, 720);

	auto& renderer = window.GetRenderer();
	auto& input = window.GetInput();

	auto defaultShader = renderer.DefaultShader();
	auto normals = renderer.GenerateShaderFromFiles("normal.vert", "normal.frag", "normal.geom");
	auto axes = renderer.GenerateShaderFromFiles("axis.vert", "axis.frag", "axis.geom");

	auto basicMaterial = std::make_shared<Material>(*defaultShader);
	basicMaterial->_lightProperties.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	basicMaterial->_lightProperties.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	basicMaterial->_lightProperties.specular = glm::vec3(0.1f, 0.1f, 0.1f);
	basicMaterial->_lightProperties.shininess = 10.f;

	Entity moon;
	//moon.Rotate(1.f, 0.f, 0.f, 15.f);
	moon.Translate(0.f, 5.f, 0.f);
	moon.Scale(0.2f);
	moon.CreateComponentOfType<DirectionalLightComponent>(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.f, 0.f, 0.f));
	auto test = moon.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*test, "sphere.dae");

	Entity floor;
	auto floorComp = floor.CreateComponentOfType<ModelComponent>().lock();
	auto floorCollider = floor.CreateComponentOfType<BoxColliderComponent>().lock();

	floorComp->_meshes.push_back(renderer.GenerateMesh("Cube", vertices, order, basicMaterial, true));

	floor.Scale(100.f, 1.f, 100.f);
	floor.Translate(0.f, -8.f, 0.f);

	Entity cube;
	auto cubeComp = cube.CreateComponentOfType<ModelComponent>().lock();
	cubeComp->_meshes.push_back(renderer.GenerateMesh("Cube"));
	cube.CreateComponentOfType<BoxColliderComponent>();

	Entity ramp1(cube);
	ramp1.Translate(0.f, 2.f, -10.f);
	ramp1.Scale(5.f, 0.1f, 5.f);
	

	Entity testCube(cube);
	auto cubeBody = testCube.CreateComponentOfType<BodyComponent>(1.f).lock();
	testCube.Translate(-5.f, 10.f, -10.f);
	testCube.Scale(2.f);
	testCube.Rotate(1.f, 0.f, 0.f, 45.f);
	
	
	Entity testCube2(testCube);
	testCube2.Translate(10.f, 0.f, 0.f);

	Entity ramp2(ramp1);

	ramp1.Translate(-5.f, 0.f, 0.f);
	ramp1.Rotate(0.f, 0.f, 1.f, -45.f);

	ramp2.Translate(5.f, 0.f, 0.f);
	ramp2.Rotate(0.f, 0.f, 1.f, 45.f);

	Entity sphere;
	sphere.Translate(0.f, 15.f, -10.f);
	auto sphereModel = sphere.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*sphereModel, "sphere.dae");
	Entity collisionSphere(sphere);
	collisionSphere.Scale(0.05f);
	auto sphereCollider = sphere.CreateComponentOfType<SphereColliderComponent>(1.f).lock();
	auto sphereBody = sphere.CreateComponentOfType<BodyComponent>(1.f).lock();

	std::vector<Entity> cubeTower;

	Simulator simulator;
	cubeTower.reserve(3000);
	for (int i = 0; i < 3000; ++i) {
		cubeTower.push_back(cube);
		auto&& newCube = cubeTower.back();
		auto&& tempComp = newCube.CreateComponentOfType<BodyComponent>(1.f).lock();
		tempComp->gravity = false;
		newCube.Scale(2.f);
		newCube.Translate(0.f, i*4, -10.f);
		//renderer.AddObject(newCube);
		simulator.AddObject(newCube);
	}

	sphere.Translate(-5.f, 0.f, 0.f);

	renderer.AddObject(ramp1);
	renderer.AddObject(ramp2);
	renderer.AddObject(testCube);
	renderer.AddObject(testCube2);
	renderer.AddObject(floor);
	renderer.AddObject(sphere);
	renderer.AddObject(moon);
	renderer.AddLightSource(moon);
	renderer.SetShadowCaster(moon);

	renderer.SetPerspective(90.f, 0.1f, 100.f);

	renderer.SetSkybox("StarSkybox041.png", "StarSkybox042.png", "StarSkybox043.png", "StarSkybox044.png", "StarSkybox045.png", "StarSkybox046.png");

	auto& camera = renderer.GetMainCamera();

	simulator.AddObject(floor);
	simulator.AddObject(ramp1);
	simulator.AddObject(ramp2);
	simulator.AddObject(testCube);
	simulator.AddObject(testCube2);
	simulator.AddObject(sphere);

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
	
	std::vector<Entity> cols;
	bool stopSimulation = true;
	glm::vec3 contact(0), normal(0), angularVec(0);
	input.AddKeyPressEventHandler(Keys::C, [&]() {stopSimulation = !stopSimulation; });
	bool enableAutostop = false;
	input.AddKeyPressEventHandler(Keys::V, [&]() {enableAutostop = !enableAutostop; });
	//testCube.TryGetComponentOfType<ColliderComponent>().lock()->AddCollisionEventHandler([&](Entity& e, const CollisionEvent c) {
	//	if (enableAutostop) {
	//		stopSimulation = true;
	//	}
	//	cols.push_back(collisionSphere);
	//	auto&& col = cols.back();
	//	col.position = c.contactPoint1;
	//	renderer.AddObject(col);
	//	contact = c.contactPoint1;
	//	normal = glm::normalize(c.collisionNormal);
	//	angularVec = glm::cross(contact - e.position, normal);
	//	//std::cout << std::format("Position: {}, {}, {}\n", e.position.x, e.position.y, e.position.z);
	//	//std::cout << std::format("Normal: {}, {}, {}\n", c.collisionNormal.x, c.collisionNormal.y, c.collisionNormal.z); 
	//																					 });

	bool showAxes = false;
	input.AddKeyPressEventHandler(Keys::Q, [&]() {showAxes = !showAxes; });

	auto test1 = ramp2.TryGetComponentOfType<ColliderComponent>().lock();

	window.InitializeTime();
	int frameCounter = 0;
	while (window.IsOpen()) {
		++frameCounter;
		auto deltaTime = window.ComputeDeltaTime();
		window.ComputeFPS();
		input.ProcessInput();

		if (!renderWireframe) {
			renderer.RenderFrame();
		}
		else {
			renderer.RenderWireframe();
		}
		if (showNormals) {
			renderer.RenderFrame(*normals);
		}
		if (showAxes) {
			renderer.RenderFrame(*axes);
		}

		renderer.DrawRayAtPosition(testCube.position, cubeBody->angularVelocity, glm::vec3(1.f, 1.f, 0.f));
		if (!stopSimulation) {
			simulator.SimulateStep(deltaTime * 1.f);
		}
		else {
			renderer.DrawRayAtPosition(contact, normal, glm::vec3(0.f, 1.f, 0.f));
			renderer.DrawRayBetweenPoints(contact, testCube.position, glm::vec3(0.f, 0.f, 1.f));
			renderer.DrawRayAtPosition(testCube.position, angularVec, glm::vec3(0.f, 1.f, 1.f));
		}

		//auto&& box = test1->GetBoundingBox();
		//auto&& sphereBox = sphereCollider->GetBoundingBox();
		//renderer.DrawBoxFromExtents(glm::vec3(1.f, 0.f, 0.f), box.min, box.max);
		//renderer.DrawBoxFromExtents(glm::vec3(1.f, 0.f, 0.f), sphereBox.min, sphereBox.max);

		renderer.DisplayScene();

		if (frameCounter >= 3000) {
			std::cout << "cleaned\n";
			renderer.CleanDanglingPointers();
			simulator.CleanDanglingPointers();
			frameCounter = 0;
		}
	}
}