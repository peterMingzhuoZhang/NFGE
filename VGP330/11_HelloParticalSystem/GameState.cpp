#include "GameState.h"

void CameraControl(Camera& camera);

void GameState::Initialize()
{
	auto graphicSystem = GraphicsSystem::Get();
	GraphicsSystem::Get()->SetClearColor(Colors::DimGray);

	NFGE::sApp.GetMainCamera().SetPosition({ 0.0f, 0.0f, -10.0f });
	NFGE::sApp.GetMainCamera().SetDirection({ 0.0f, 0.0f, 1.0f });
	NFGE::sApp.GetMainCamera().SetFarPlane(500.0f);
	mParticleEmmiter.Load();
}

void GameState::Terminate()
{
	mParticleEmmiter.Unload();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		NFGEApp::ShutDown();
		return;
	}

	mParticleEmmiter.Update(deltaTime);

	if (inputSystem->IsKeyDown(KeyCode::SPACE))
	{
		mParticleEmmiter.SpawnPartice();
	}

	CameraControl(NFGE::sApp.GetMainCamera());
	mDeltaTime = deltaTime;
}

void GameState::Render()
{
	for (float i = -50.0f; i <= 50.0f; i += 1.0f)
	{
		SimpleDraw::AddLine({ i,0.0f,-50.0f }, { i,0.0f,50.0f }, Colors::White);
		SimpleDraw::AddLine({ -50.0f,0.0f,i }, { 50.0f,0.0f,i }, Colors::White);
	}
	SimpleDraw::Render(NFGE::sApp.GetMainCamera());
	mParticleEmmiter.Render();
}

void GameState::DebugUI()
{
	ImGui::SetNextWindowPos({ 30.0f, 30.0f });
	ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %f", 1.0f / mDeltaTime);
	ImGui::Text("Particles: %d", mParticleEmmiter.mCurrentParticleCount);
	const char* blendItems[] = { "Opaque", "AlphaBlend", "AlphaPremultiplied", "Additive" };
	
	ImGui::Combo("Blend Mode", &mBlendStateSelected, blendItems, IM_ARRAYSIZE(blendItems));
	mParticleEmmiter.mCurrentBlendState = blendItems[mBlendStateSelected];
	ImGui::ColorEdit4("Start Color", (float*)&mParticleEmmiter.startColor);
	ImGui::ColorEdit4("End Color", (float*)&mParticleEmmiter.endColor);
	ImGui::End();

	auto& cameraDir = NFGE::sApp.GetMainCamera().GetDirection();
	auto& cameraPos = NFGE::sApp.GetMainCamera().GetPosition();
	ImGui::Text("Camera Pos, %f, %f, %f", cameraPos.x, cameraPos.y, cameraPos.z);
	ImGui::Text("Camera Dir, %f, %f, %f", cameraDir.x, cameraDir.y, cameraDir.z);


}


void CameraControl(Camera& camera)
{
	static auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
	lastTime = currentTime;

	const float moveSpeed = 10.0f;
	const float turnSpeed = 5.0f;
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		camera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		camera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		camera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		camera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		camera.Pitch(-inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}