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

using namespace TVGE;
using namespace TVGE::Graphics;
using namespace TVGE::Physics;
using namespace TVGE::Input;

//for some ungodly reason, main does not compile without this
static void DontDeleteThis(Entity& obj)
{
	obj.Translate(0.f, 0.f, 0.f);
}

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

	floor.Scale(100.f, 1.f, 100.f);
	floor.Translate(0.f, -8.f, 0.f);

	renderer.AddObject(floor);
	renderer.AddLightSource(light);

	renderer.SetPerspective(90.f, 0.1f, 100.f);

	renderer.SetSkybox("StarSkybox041.png", "StarSkybox042.png", "StarSkybox043.png", "StarSkybox044.png", "StarSkybox045.png", "StarSkybox046.png");

	auto& camera = renderer.GetMainCamera();

	simulator.AddObject(floor);

	bool initialFocus = true;
	double prevX = 0, prevY = 0;
	input.AddKeyPressEventHandler(Keys::ESCAPE, [&]() {window.Unfocus(); });

	input.AddMouseButtonPressEventHandler(MouseButtons::LEFT_CLICK, [&]() {window.Focus(); initialFocus = true; });
	CameraComponent::Movement movement{};

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