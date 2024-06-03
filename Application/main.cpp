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

static void PrintVector(const glm::vec3 vec){
	std::cout<<"X= "<< vec.x<<" Y= "<<vec.y<<" Z= "<<vec.z<<"\n";
}

static void PrintVector(const glm::quat vec){
	std::cout<<"X= "<< vec.x<<" Y= "<<vec.y<<" Z= "<<vec.z<<" W= "<<vec.w<<"\n";
}

int main()
{
	auto [vertices, order] = Common3DMeshes::Cube();

	auto& window = Window::Initialize("TVGE v1.0A", 1920, 1080);

	auto& renderer = window.GetRenderer();
	auto& input = window.GetInput();
	Simulator simulator;

	auto defaultShader = renderer.DefaultShader();
	auto normals = renderer.GenerateShaderFromFiles("normal.vert", "normal.frag", "normal.geom");
	auto axes = renderer.GenerateShaderFromFiles("axis.vert", "axis.frag", "axis.geom");

	auto basicMaterial = std::make_shared<Material>(*defaultShader);
	basicMaterial->lightProperties.ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	basicMaterial->lightProperties.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	basicMaterial->lightProperties.specular = glm::vec3(0.1f, 0.1f, 0.1f);
	basicMaterial->lightProperties.shininess = 10.f;

	auto carpetMaterial = std::make_shared<Material>(*defaultShader);
	carpetMaterial->ambientMap = renderer.GenerateTexture2D("carpet.png", true);
	carpetMaterial->diffuseMap = renderer.GenerateTexture2D("carpet.png", true);

	auto debugMaterial = std::make_shared<Material>(*defaultShader);
	debugMaterial->lightProperties.ambient = glm::vec3(0.f, 0.3f, 0.f);
	debugMaterial->lightProperties.diffuse = glm::vec3(0.f, 0.5f, 0.f);
	debugMaterial->lightProperties.specular = glm::vec3(0.f, 0.1f, 0.f);
	debugMaterial->lightProperties.shininess = 10.f;

	auto lightMaterial = std::make_shared<Material>(*defaultShader);
	lightMaterial->lightProperties.ambient = glm::vec3(0.f, 0.f, 0.f);
	lightMaterial->lightProperties.diffuse = glm::vec3(0.f, 0.f, 0.f);
	lightMaterial->lightProperties.specular = glm::vec3(0.f, 0.f, 0.f);
	lightMaterial->lightProperties.emissive = glm::vec3(1.f, 1.f, 1.f);
	
	Entity light;
	light.CreateComponentOfType<DirectionalLightComponent>(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	Entity floor;
	auto floorComp = floor.CreateComponentOfType<ModelComponent>().lock();
	auto floorCollider = floor.CreateComponentOfType<BoxColliderComponent>().lock();

	floorComp->meshes.push_back(renderer.GenerateMesh("Cube", vertices, order, carpetMaterial, true));

	Entity sWall(floor);
	Entity trackWallR(floor);
	Entity platform(floor);
	Entity eWall(floor);
	Entity frWall(floor);

	eWall.Scale(1.f, 6.f, 20.f);
	eWall.Translate(0.f, -1.f, 40.f);

	frWall.Scale(32.f, 6.f, 1.f);
	frWall.Translate(23.5f, -1.f, 29.5f);

	Entity flWall(frWall);

	flWall.Translate(-47.f, 0.f, 0.f);

	Entity wWall(eWall);

	eWall.Translate(40.f, 0.f, 0.f);
	wWall.Translate(-40.f, 3.f, 0.f);
	wWall.Scale(1.f, 3.f, 1.f);

	sWall.Scale(80.f, 6.f, 1.f);
	sWall.Translate(0.f, -1.f, 0.f);

	trackWallR.Scale(1.f, 6.f, 70.f);
	trackWallR.Translate(0.f, -1.f, -5.f);
	Entity trackWallL(trackWallR);

	sWall.Translate(0.f, 0.f, 50.f);

	trackWallL.Translate(-7.f, 0.f, 0.f);
	trackWallR.Translate(7.f, 0.f, 0.f);

	floor.Scale(7.f, 1.f, 70.f);
	floor.Translate(0.f, -2.f, -5.f);

	platform.Scale(80, 1.f, 20.f);
	platform.Translate(0.f, -2.f, 40.f);
	
	Entity underFloor(floor);

	underFloor.Scale(2.f, 1.f, 1.f);
	underFloor.Translate(0.f, -2.f, 0.f);

	Entity ball;
	ball.Translate(0.f, 0.f, 35.f);
	auto ballModel = ball.CreateComponentOfType<ModelComponent>().lock();
	ball.CreateComponentOfType<PointLightComponent>(glm::vec3(0.3f, 0.3f, 0.3f),
													glm::vec3(0.8f, 0.8f, 0.8f),
													glm::vec3(0.5f, 0.5f, 0.5f));
	renderer.LoadModel(*ballModel, "sphere.dae");
	ballModel->meshes[0].lock()->material = lightMaterial;
	auto ballBody = ball.CreateComponentOfType<BodyComponent>(200.f).lock();
	ballBody->SetElasticity(0.1f);
	ball.CreateComponentOfType<SphereColliderComponent>();
	renderer.AddObject(ball);
	simulator.AddObject(ball);

	float throwPower = 5000.f;

	auto& camera = renderer.GetMainCamera();
	camera.SetCameraPosition(0.f, 3.f, 0.f);
	auto& player = camera.entity;
	player.Translate(0.f, 0.f, 40.f);
	auto playerBody = player.CreateComponentOfType<BodyComponent>(1.f).lock();
	playerBody->SetElasticity(0.f);
	player.CreateComponentOfType<BoxColliderComponent>(true, glm::vec3(0.5f, 2.f, 0.5f));


	Entity grabHitbox;
	grabHitbox.SetParent(player);
	grabHitbox.Translate(0.f, 1.f, -4.f);
	grabHitbox.Scale(2.f, 3.f, 3.f);
	auto debugModel = grabHitbox.CreateComponentOfType<ModelComponent>().lock();
	debugModel->meshes.push_back(renderer.GenerateMesh("Cube2", vertices, order, debugMaterial, true));
	
	bool ballInRange = false;
	auto grabCollider = grabHitbox.CreateComponentOfType<BoxColliderComponent>(false).lock();
	grabCollider->AddCollisionEventHandler([&](Entity& other, const CollisionEvent){
	
		if(other.GetId() == ball.GetId()){
			ballInRange = true;
		}
										   });


	Entity pinParent;
	pinParent.Translate(0, 0.f, -30.f);
	std::vector<Entity> pins;
	pins.reserve(10);

	pins.emplace_back();
	pins[0].SetParent(pinParent);
	pins[0].Scale(0.2f, 0.2f, 0.2f);
	auto pinModel = pins[0].CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*pinModel, "pin.glb");
	for(auto mesh : pinModel->meshes){
	
		auto material = mesh.lock()->material;
		material->lightProperties.ambient = 0.5f * material->lightProperties.diffuse;
	}
	auto pinBody = pins[0].CreateComponentOfType<BodyComponent>(1.f, glm::vec3(0.f, -6.f, 0.f)).lock();
	pinBody->SetElasticity(0.f);
	pins[0].CreateComponentOfType<BoxColliderComponent>(true, 5.f * glm::vec3(0.45f, 1.55f, 0.45f), glm::vec3(0.f, -1.5f, 0.f));

	renderer.AddObject(pins[0]);
	simulator.AddObject(pins[0]);
	for(int i = 1; i < 10; ++i){
		pins.emplace_back(pins[0]);
		renderer.AddObject(pins[i]);
		simulator.AddObject(pins[i]);
	}

	for(int i=0; i<10; ++i){
		pins[i].relativeRotation = glm::identity<glm::quat>();
		auto pinBody = pins[i].TryGetComponentOfType<BodyComponent>().lock();
		pinBody->velocity = glm::vec3(0.f,0.f,0.f);
		pinBody->angularVelocity = glm::vec3(0.f,0.f,0.f);
	}

	pins[0].relativePosition = glm::vec3(0.f, 0.f, 0.f);
	pins[1].relativePosition = glm::vec3(-1.f, 0.f, -2.f);
	pins[2].relativePosition = glm::vec3(1.f, 0.f, -2.f);
	pins[3].relativePosition = glm::vec3(-2.f, 0.f, -4.f);
	pins[4].relativePosition = glm::vec3(0.f, 0.f, -4.f);
	pins[5].relativePosition = glm::vec3(2.f, 0.f, -4.f);
	pins[6].relativePosition = glm::vec3(-3.f, 0.f, -6.f);
	pins[7].relativePosition = glm::vec3(-1.f, 0.f, -6.f);
	pins[8].relativePosition = glm::vec3(1.f, 0.f, -6.f);
	pins[9].relativePosition = glm::vec3(3.f, 0.f, -6.f);

	Entity sculpture;
	sculpture.Scale(0.01f, 0.01f, 0.01f);
	sculpture.Rotate(0.f, 1.f, 0.f, 90.f);
	sculpture.Translate(-30.f, 1.f, 40.f);

	input.AddKeyPressEventHandler(Keys::K, [&](){
								  sculpture.Translate(0.f, -1.f, 0.f);
								  });
	input.AddKeyPressEventHandler(Keys::I, [&](){
								  sculpture.Translate(0.f, 1.f, 0.f);
								  });
	//input.AddKeyPressEventHandler(Keys::U, [&](){
	//							platform.Translate(0.f, 1.f, 0.f);
	//							});
	//input.AddKeyPressEventHandler(Keys::J, [&](){
	//							  sculpture.Translate(1.f, 0.f, 0.f);
	//							  });
	//input.AddKeyPressEventHandler(Keys::L, [&](){
	//							  sculpture.Translate(-1.f, 0.f, 0.f);
	//							  });

	auto sculptureModel = sculpture.CreateComponentOfType<ModelComponent>().lock();
	renderer.LoadModel(*sculptureModel, "bust.obj", false);

	renderer.AddObject(floor);
	renderer.AddObject(platform);
	renderer.AddObject(underFloor);
	renderer.AddObject(light);
	renderer.AddObject(sWall);
	renderer.AddObject(eWall);
	renderer.AddObject(wWall);
	renderer.AddObject(frWall);
	renderer.AddObject(flWall);
	renderer.AddObject(trackWallL);
	renderer.AddObject(trackWallR);
	renderer.AddObject(sculpture);
	renderer.SetShadowSource(light);


	renderer.SetPerspective(90.f, 0.1f, 150.f);
	renderer.SetSkybox("StarSkybox041.png", "StarSkybox042.png", "StarSkybox043.png", "StarSkybox044.png", "StarSkybox045.png", "StarSkybox046.png");


	simulator.AddObject(floor);
	simulator.AddObject(platform);
	simulator.AddObject(underFloor);
	simulator.AddObject(player);
	simulator.AddObject(sWall);
	simulator.AddObject(eWall);
	simulator.AddObject(wWall);
	simulator.AddObject(frWall);
	simulator.AddObject(flWall);
	simulator.AddObject(trackWallR);
	simulator.AddObject(trackWallL);
	simulator.AddObject(grabHitbox);
	

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

				glm::vec3 front = camera.GetCameraFront();
				glm::vec3 up = camera.GetCameraUp();
				glm::vec3 right = glm::normalize(glm::cross(front, up));
				glm::vec3 movementFront = glm::normalize(glm::cross(up, right));

				glm::quat rotation;
				glm::vec3 cross = glm::cross(glm::vec3(0.f, 0.f, -1.f), movementFront);
				rotation.x = cross.x;
				rotation.y = cross.y;
				rotation.z = cross.z;
				rotation.w = 1 + glm::dot(glm::vec3(0.f, 0.f, -1.f), movementFront);

				player.relativeRotation = glm::normalize(rotation);
			}
		}});

	float playerSpeed = 10.f;
	
	input.AddGenericInputBehaviour([&]() {
		glm::vec3 front = camera.GetCameraFront();
		glm::vec3 up = camera.GetCameraUp();
		glm::vec3 right = glm::normalize(glm::cross(front, up));
		glm::vec3 movementFront = glm::normalize(glm::cross(up, right));
		
		auto delta = window.GetDeltaTime();
		float adjustedSpeed = playerSpeed * delta;

		if(input.IsKeyPressed(W)){
			camera.entity.Translate(movementFront * adjustedSpeed);
		};
		if(input.IsKeyPressed(S)){
			camera.entity.Translate(-movementFront * adjustedSpeed);
		}
		if(input.IsKeyPressed(A)){
			camera.entity.Translate(-right * adjustedSpeed);
		};
		if(input.IsKeyPressed(D)){
			camera.entity.Translate(right * adjustedSpeed);
		}
								   });

	bool grabbedBall = false;
	input.AddMouseButtonPressEventHandler(LEFT_CLICK, [&]() {
		if(grabbedBall){
			grabbedBall = false;
			auto newPos = ball.GetAbsolutePosition();
			ball.RemoveParent();
			ball.relativePosition = newPos;
			ballBody->AddForceInstant(camera.GetCameraFront() * throwPower);
			ballBody->gravity = true;
			return;
		}
		if(ballInRange){
			grabbedBall = true;
			ballBody->velocity = glm::vec3(0.f,0.f,0.f);
			ballBody->angularVelocity = glm::vec3(0.f,0.f,0.f);
			ballBody->gravity = false;
			ball.SetParent(player);
			ball.relativePosition = glm::vec3(0.f, 0.f, -5.f);
		}
										  });
	input.AddMouseButtonPressEventHandler(RIGHT_CLICK, [&]() {
		if(grabbedBall){
			grabbedBall = false;
			auto newPos = ball.GetAbsolutePosition();
			ball.RemoveParent();
			ball.relativePosition = newPos;
			ballBody->gravity = true;
		}
										  });

	bool showNormals = false;
	input.AddKeyPressEventHandler(Keys::N, [&]() {showNormals = !showNormals; });
	bool renderWireframe = false;
	input.AddKeyPressEventHandler(Keys::M, [&]() {renderWireframe = !renderWireframe; });
	bool showAxes = false;
	input.AddKeyPressEventHandler(Keys::Q, [&]() {showAxes = !showAxes; });
	bool showColliders = false;
	input.AddKeyPressEventHandler(Keys::H, [&]() {showColliders = !showColliders; });
	bool stopTime = false;
	input.AddKeyPressEventHandler(Keys::T, [&]() {stopTime = !stopTime; });

	input.AddKeyPressEventHandler(Keys::R, [&](){

		player.relativePosition = glm::vec3(0.f, 0.f, 40.f);
		ball.relativePosition = glm::vec3(0.f, 0.f, 35.f);
		ballBody->velocity = glm::vec3(0.f,0.f,0.f);
		ballBody->angularVelocity = glm::vec3(0.f,0.f,0.f);

		for(int i=0; i<10; ++i){
			pins[i].relativeRotation = glm::identity<glm::quat>();
			auto pinBody = pins[i].TryGetComponentOfType<BodyComponent>().lock();
			pinBody->velocity = glm::vec3(0.f,0.f,0.f);
			pinBody->angularVelocity = glm::vec3(0.f,0.f,0.f);
	}
		pins[0].relativePosition = glm::vec3(0.f, 0.f, 0.f);
		pins[1].relativePosition = glm::vec3(-1.f, 0.f, -2.f);
		pins[2].relativePosition = glm::vec3(1.f, 0.f, -2.f);
		pins[3].relativePosition = glm::vec3(-2.f, 0.f, -4.f);
		pins[4].relativePosition = glm::vec3(0.f, 0.f, -4.f);
		pins[5].relativePosition = glm::vec3(2.f, 0.f, -4.f);
		pins[6].relativePosition = glm::vec3(-3.f, 0.f, -6.f);
		pins[7].relativePosition = glm::vec3(-1.f, 0.f, -6.f);
		pins[8].relativePosition = glm::vec3(1.f, 0.f, -6.f);
		pins[9].relativePosition = glm::vec3(3.f, 0.f, -6.f);
								  });

	input.AddKeyPressEventHandler(Keys::SPACE, [&](){
		player.Translate(0.f, 0.1f, 0.f);
		playerBody->AddForceInstant(0.f, 10.f, 0.f);
	});

	window.InitializeTime();
	int frameCounter = 0;
	while (window.IsOpen()) {
		++frameCounter;
		auto deltaTime = window.ComputeDeltaTime();
		window.ComputeFPS();
		input.ProcessInput();

		auto ballPosition = ball.GetAbsolutePosition();
		if(ballPosition.z > -20.f && ballPosition.y > -1.f){
			renderer.SetShadowSource(ball);
		}
		else{
			renderer.SetShadowSource(light);
		}
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
		ballInRange = false;
		playerBody->angularVelocity = glm::vec3(0.f,0.f,0.f);
		playerBody->velocity = glm::vec3(0.f, playerBody->velocity.y, 0.f);
		if(!stopTime){
		simulator.SimulateStep(deltaTime * 1.f);
		}

		if(showColliders){
			for(int i=0; i< 10; ++i){
				auto pinCollider = pins[i].TryGetComponentOfType<BoxColliderComponent>().lock();
				renderer.DrawBox(glm::vec3(0.f, 1.f, 0.f), pinCollider->GetCenter(), 2.f*pinCollider->GetExtents(), pins[i].GetAbsoluteRotation());
			}
		}

		renderer.DisplayScene();

		if (frameCounter >= 3000) {
			std::cout << "cleaned\n";
			renderer.CleanDanglingPointers();
			simulator.CleanDanglingPointers();
			frameCounter = 0;
		}
	}
}