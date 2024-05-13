import Graphics;
import MeshHelpers;
import Physics;

import <iostream>;
import <thread>;
import <chrono>;
import <memory>;
import <random>;
import <format>;

using namespace TVGE;
using namespace TVGE::Graphics;
using namespace TVGE::Physics;
using namespace TVGE::Input;


int main()
{
	auto [vertices, order] = Common3DMeshes::Cube();

	auto& window = Window::Initialize("TVGE v1.0A", 1024, 720);

	auto& renderer = window.GetRenderer();
	auto& input = window.GetInput();
	Simulator simulator;

	auto defaultShader = renderer.DefaultShader();
	auto normals = renderer.GenerateShaderFromFiles("normal.vert", "normal.frag", "normal.geom");
	auto axes = renderer.GenerateShaderFromFiles("axis.vert", "axis.frag", "axis.geom");

	auto basicMaterial = std::make_shared<Material>(*defaultShader);
	basicMaterial->lightProperties.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	basicMaterial->lightProperties.diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	basicMaterial->lightProperties.specular = glm::vec3(0.1f, 0.1f, 0.1f);
	basicMaterial->lightProperties.shininess = 10.f;

	Entity light;
	light.Rotate(1.f, 0.f, 0.f, 15.f);
	light.Translate(0.f, 5.f, 0.f);
	light.Scale(0.2f);
	light.CreateComponentOfType<DirectionalLightComponent>(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.f, 0.f, 0.f));

	Entity floor;
	auto floorComp = floor.CreateComponentOfType<ModelComponent>().lock();
	auto floorCollider = floor.CreateComponentOfType<BoxColliderComponent>().lock();

	floorComp->_meshes.push_back(renderer.GenerateMesh("Cube", vertices, order, basicMaterial, true));

	Entity sWall(floor);

	floor.Scale(100.f, 1.f, 100.f);
	floor.Translate(0.f, -8.f, 0.f);

	sWall.Scale(100.f, 4.f, 1.f);
	sWall.Translate(0.f, -6.f, 0.f);

	Entity nWall(sWall);
	Entity eWall(sWall);

	eWall.Rotate(glm::vec3(0.f, 1.f, 0.f), 90.f);
	Entity wWall(eWall);

	sWall.Translate(0.f, 0.f, -50.f);
	nWall.Translate(0.f, 0.f, +50.f);
	eWall.Translate(-50.f, 0.f, 0);
	wWall.Translate(50.f, 0.f, 0);

	auto& camera = renderer.GetMainCamera();
	camera.SetCameraPosition(0.f, 3.f, 0.f);
	auto& player = camera.entity;
	auto playerBody = player.CreateComponentOfType<BodyComponent>(100.f).lock();
	player.CreateComponentOfType<BoxColliderComponent>(glm::vec3(0.5f, 2.f, 0.5f));

	float playerSpeed = 10.f;
	input.AddGenericInputBehaviour([&]() {
		glm::vec3 _front = camera.GetCameraFront();
		glm::vec3 _up = camera.GetCameraUp();
		glm::vec3 _right = glm::normalize(glm::cross(_front, _up));
		glm::vec3 _movementFront = glm::normalize(glm::cross(_up, _right));
		
		auto delta = window.GetDeltaTime();
		float adjustedSpeed = playerSpeed * delta;

		if(input.IsKeyPressed(W)){
			camera.entity.position += _movementFront * adjustedSpeed;
		};
		if(input.IsKeyPressed(S)){
			camera.entity.position -= _movementFront * adjustedSpeed;
		}
		if(input.IsKeyPressed(A)){
			camera.entity.position -= _right * adjustedSpeed;
		};
		if(input.IsKeyPressed(D)){
			camera.entity.position += _right * adjustedSpeed;
		}
								   });

	

	renderer.AddObject(floor);
	renderer.AddLightSource(light);
	renderer.AddObject(sWall);
	renderer.AddObject(nWall);
	renderer.AddObject(eWall);
	renderer.AddObject(wWall);

	renderer.SetPerspective(90.f, 0.1f, 100.f);
	renderer.SetSkybox("StarSkybox041.png", "StarSkybox042.png", "StarSkybox043.png", "StarSkybox044.png", "StarSkybox045.png", "StarSkybox046.png");


	simulator.AddObject(floor);
	simulator.AddObject(player);
	simulator.AddObject(sWall);
	simulator.AddObject(nWall);
	simulator.AddObject(eWall);
	simulator.AddObject(wWall);

	bool initialFocus = true;
	double prevX = 0, prevY = 0;
	input.AddKeyPressEventHandler(Keys::ESCAPE, [&]() {window.Unfocus(); });

	input.AddMouseButtonPressEventHandler(MouseButtons::LEFT_CLICK, [&]() {window.Focus(); initialFocus = true; });

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

	bool showNormals = false;
	input.AddKeyPressEventHandler(Keys::N, [&]() {showNormals = !showNormals; });
	bool renderWireframe = false;
	input.AddKeyPressEventHandler(Keys::M, [&]() {renderWireframe = !renderWireframe; });
	bool showAxes = false;
	input.AddKeyPressEventHandler(Keys::Q, [&]() {showAxes = !showAxes; });

	window.InitializeTime();
	int frameCounter = 0;
	while (window.IsOpen()) {
		++frameCounter;
		auto deltaTime = window.ComputeDeltaTime();
		window.ComputeFPS();
		input.ProcessInput();

		if (!renderWireframe) {
			renderer.RenderScene();
		}
		else {
			renderer.RenderWireframe();
		}
		if (showNormals) {
			renderer.RenderScene(*normals);
		}
		if (showAxes) {
			renderer.RenderScene(*axes);
		}
		simulator.SimulateStep(deltaTime * 1.f);

		renderer.DisplayScene();

		if (frameCounter >= 3000) {
			std::cout << "cleaned\n";
			renderer.CleanDanglingPointers();
			simulator.CleanDanglingPointers();
			frameCounter = 0;
		}
	}
}