#include "GameState.h"

using namespace NFGE::Physics;

const float cameraMoveSpeed = 20.0f;
const float cameraTurnSpeed = 20.0f;

void GameState::Initialize()
{
	auto& camera = NFGE::sApp.GetMainCamera();
	camera.SetPosition({ -50.0f,200.0f, -200.0f });
	camera.SetDirection({ 0.6f,-0.3f, 0.7f });
	camera.SetFarPlane(10000.0f);

	GraphicsSystem::Get()->SetClearColor(NFGE::Graphics::Colors::Black);

	mScene.Load();

	mPhysicsWorld.AddOBB(OBB{ {0.0f,10.0f,0.0f}, {10.0f,1.0f,10.0f},{ Math::QuaternionRotationAxis(Math::Vector3::XAxis, -0.5f) } });
}

void GameState::Terminate()
{
	mScene.Unload();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		NFGEApp::ShutDown();
		return;
	}

	mdeltaTime = deltaTime;

	mScene.Update(deltaTime);

	CameraControl(NFGE::sApp.GetMainCamera());

	mPhysicsWorld.Update(deltaTime);
}

void GameState::Render()
{
	mScene.Render(NFGE::sApp.GetMainCamera());

	mPhysicsWorld.DebugDraw();
}

void GameState::DebugUI()
{
	mScene.DebugUI();

	ShowUI();

	for (float i = -50.0f; i <= 50.0f; i += 1.0f)
	{
		SimpleDraw::AddLine({ i,0.0f,-50.0f }, { i,0.0f,50.0f }, Colors::DarkGray);
		SimpleDraw::AddLine({ -50.0f,0.0f,i }, { 50.0f,0.0f,i }, Colors::DarkGray);
	}
	Vector3 origin{ 0.0f,0.0f,0.0f };
	NFGE::sApp.DrawLine(origin, origin + Vector3::ZAxis * 200.0f, NFGE::Graphics::Colors::Red);
	NFGE::sApp.DrawSphere(origin + Vector3::ZAxis * 200.0f, 2.0f, NFGE::Graphics::Colors::Red);
	NFGE::sApp.DrawLine(origin, origin + Vector3::YAxis * 200.0f, NFGE::Graphics::Colors::Green);
	NFGE::sApp.DrawSphere(origin + Vector3::YAxis* 200.0f, 2.0f, NFGE::Graphics::Colors::Green);
	NFGE::sApp.DrawLine(origin, origin + Vector3::XAxis * 200.0f, NFGE::Graphics::Colors::Cyan);
	NFGE::sApp.DrawSphere(origin + Vector3::XAxis  * 200.0f, 2.0f, NFGE::Graphics::Colors::Cyan);
	
}

void GameState::ShowUI()
{
	ImGui::Begin("Util");
	ImGui::Text("FPS: %f", 1.0f / mdeltaTime);
	ImGui::End();

	auto& cameraDir = NFGE::sApp.GetMainCamera().GetDirection();
	auto& cameraPos = NFGE::sApp.GetMainCamera().GetPosition();
	ImGui::Text("Camera Pos, %f, %f, %f", cameraPos.x, cameraPos.y, cameraPos.z);
	ImGui::Text("Camera Dir, %f, %f, %f", cameraDir.x, cameraDir.y, cameraDir.z);
	if (ImGui::Button("Camera Regular"))
	{
		NFGE::sApp.GetMainCamera().SetPosition({ 0.0f,0.0f,-10.0f });
		NFGE::sApp.GetMainCamera().SetDirection({ 0.0f, 0.0f, 1.00f });
	}
	if (ImGui::Button("Camera Stander"))
	{
		NFGE::sApp.GetMainCamera().SetPosition({ 650.0f,630.0f,-200.0f });
		NFGE::sApp.GetMainCamera().SetDirection({ 0.0f,-0.7f,0.6f });
	}
	if (ImGui::Button("Camera TopDown"))
	{
		NFGE::sApp.GetMainCamera().SetPosition({ 600.0f,750.0f,350.0f });
		NFGE::sApp.GetMainCamera().SetDirection({ 0.00001f,-0.9f,0.01f });
	}

	ImGui::Begin("Physics");
	if (ImGui::Button("Boom_Particle"))
	{
		mPhysicsWorld.ClearDynamic();
		for (int i = 0; i < 100; i++)
		{
			auto vel = RandomUnitSphere() * 0.1f;
			vel.y = abs(vel.y);

			auto particle = new Particle();
			particle->SetPosition({ 0.0f,0.0f,0.0f });
			particle->SetVelocity(vel);
			particle->radius = 0.05f;
			particle->invMass = 1.0f;
			mPhysicsWorld.AddParticle(particle);
		}
	}

	if (ImGui::Button("Boom_Line"))
	{
		mPhysicsWorld.ClearDynamic();
		for (int i = 0; i < 100; i++)
		{
			auto vel = RandomUnitSphere() * 1.0f;
			vel.y = abs(vel.y);

			auto firstParticle = mPhysicsWorld.AddLine({ 0.0f,0.0f,0.0f }, {0.0f,2.0f,0.0f})->mVertices.front();
			firstParticle->SetPosition({ 0.0f,0.0f,0.0f });
			firstParticle->SetVelocity(vel);
		}
	}

	if (ImGui::Button("Boom_Cube"))
	{
		mPhysicsWorld.ClearDynamic();
		for (int i = 0; i < 100; i++)
		{
			auto vel = RandomUnitSphere() * 5.0f;
			vel.y = abs(vel.y);

			auto firstParticle = mPhysicsWorld.AddCube({ 0.0f,0.0f,0.0f }, 1.0f)->mVertices.front();
			firstParticle->SetPosition({ 0.0f,0.0f,0.0f });
			firstParticle->SetVelocity(vel);
		}
	}

	if (ImGui::Button("Boom_Piramid"))
	{
		mPhysicsWorld.ClearDynamic();
		for (int i = 0; i < 100; i++)
		{
			auto vel = RandomUnitSphere() * 2.0f;
			vel.y = abs(vel.y);

			auto firstParticle = mPhysicsWorld.AddPiramid({ 0.0f,0.0f,0.0f }, 1.0f)->mVertices.front();
			firstParticle->SetPosition({ 0.0f,0.0f,0.0f });
			firstParticle->SetVelocity(vel);
		}
	}
	ImGui::End();
}


void GameState::CameraControl(Camera& camera)
{
	static auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
	lastTime = currentTime;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		camera.Walk(cameraMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		camera.Walk(-cameraMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		camera.Strafe(cameraMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		camera.Strafe(-cameraMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(inputSystem->GetMouseMoveX() * cameraTurnSpeed * deltaTime);
		camera.Pitch(-inputSystem->GetMouseMoveY() * cameraTurnSpeed * deltaTime);
	}
}