#include "GameState.h"

const float cameraMoveSpeed = 20.0f;
const float cameraTurnSpeed = 20.0f;

const char* modelFileName_0 = "../../Assets/Model/StephCurry.NFModelRV";
const char* modelFileName_1 = "../../Assets/Model/ybot.NFModelRV";
const char* modelFileName_2 = "../../Assets/Model/pirate.NFModelRV";
const wchar_t* shaderFileName = L"../../Assets/Shaders/Standard_Model_ZMZ.fx";

void GameState::Initialize()
{
	auto& camera = NFGE::sApp.GetMainCamera();
	camera.SetPosition({-50.0f,200.0f, -200.0f});
	camera.SetDirection({ 0.6f,-0.3f, 0.7f});
	camera.SetFarPlane(10000.0f);

	mScene_0.Load();
}

void GameState::Terminate()
{
	mScene_0.Unload();
}

void GameState::Update(float deltaTime)
{
	GraphicsSystem::Get()->SetClearColor(NFGE::Graphics::Colors::DarkSlateGray);

	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		NFGEApp::ShutDown();
		return;
	}

	mScene_0.Update(deltaTime);

	CameraControl(NFGE::sApp.GetMainCamera());
}

void GameState::Render()
{
	mScene_0.Render(NFGE::sApp.GetMainCamera());

	for (float i = -50.0f; i <= 50.0f; i += 1.0f)
	{
		SimpleDraw::AddLine({ i,0.0f,-50.0f }, { i,0.0f,50.0f }, Colors::White);
		SimpleDraw::AddLine({ -50.0f,0.0f,i }, { 50.0f,0.0f,i }, Colors::White);
	}
	NFGE::Graphics::SimpleDraw::Render(NFGE::sApp.GetMainCamera());
}

void GameState::DebugUI()
{
	mScene_0.DebugUI();

	ShowUI();
	Vector3 origin{0.0f,0.0f,0.0f};
	NFGE::sApp.DrawLine(origin, origin + Vector3::ZAxis * 200.0f, NFGE::Graphics::Colors::Red);
	NFGE::sApp.DrawSphere(origin + Vector3::ZAxis * 200.0f, 2.0f, NFGE::Graphics::Colors::Red);
	NFGE::sApp.DrawLine(origin, origin + Vector3::YAxis * 200.0f, NFGE::Graphics::Colors::Green);
	NFGE::sApp.DrawSphere(origin + Vector3::YAxis* 200.0f, 2.0f, NFGE::Graphics::Colors::Green);
	NFGE::sApp.DrawLine(origin, origin + Vector3::XAxis * 200.0f, NFGE::Graphics::Colors::Cyan);
	NFGE::sApp.DrawSphere(origin + Vector3::XAxis  * 200.0f, 2.0f, NFGE::Graphics::Colors::Cyan);
}

void GameState::ShowUI()
{
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
		NFGE::sApp.GetMainCamera().SetDirection({ 0.0f,-0.7f,0.6f});
	}
	if (ImGui::Button("Camera TopDown"))
	{
		NFGE::sApp.GetMainCamera().SetPosition({ 600.0f,750.0f,350.0f });
		NFGE::sApp.GetMainCamera().SetDirection({ 0.00001f,-0.9f,0.01f});
	}
	
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