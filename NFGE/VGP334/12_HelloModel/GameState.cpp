#include "GameState.h"

const float cameraMoveSpeed = 20.0f;
const float cameraTurnSpeed = 20.0f;

const char* modelFileName_0 = "../../Assets/Model/StephCurry.NFModelRV";
const char* modelFileName_1 = "../../Assets/Model/ybot.NFModelRV";
const char* modelFileName_2 = "../../Assets/Model/pirate.NFModelRV";
const wchar_t* shaderFileName = L"../../Assets/Shaders/Standard_Model_ZMZ.fx";

void GameState::Initialize()
{
	mModel_0.Load(Vector3(), Vector3(), Vector3(1.0f,1.0f,1.0f), modelFileName_0, shaderFileName, shaderFileName);
	//////mModel.mRotation.x = -90;
	mModel_1.Load(Vector3(0.0f,0.0f,-40.0f), Vector3(), Vector3(1.0f, 1.0f, 1.0f), modelFileName_1, shaderFileName, shaderFileName);
	mModel_2.Load(Vector3(0.0f, 0.0f, -80.0f), Vector3(), Vector3(1.0f, 1.0f, 1.0f), modelFileName_2, shaderFileName, shaderFileName);
	mSpaceShip.Load(Vector3(0.0f, 0.0f, 0.0f), Vector3(-90.0f,0.0f,0.0f), Vector3(1.0f, 1.0f, 1.0f));
	mSpaceShip.position = Vector3(0.0f, 0.0f, -100.0f);

	auto& camera = NFGE::sApp.GetMainCamera();
	camera.SetPosition({-50.0f,200.0f, -200.0f});
	camera.SetDirection({ 0.6f,-0.3f, 0.7f});
	camera.SetFarPlane(1000.0f);
}

void GameState::Terminate()
{
	mModel_0.Unload();
	mModel_1.Unload();
	mModel_2.Unload();
	mSpaceShip.UnLoad();
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

	mSpaceShip.Update(deltaTime);

	//CameraControl(NFGE::sApp.GetMainCamera());
}

void GameState::Render()
{
	for (float i = -50.0f; i <= 50.0f; i += 1.0f)
	{
		SimpleDraw::AddLine({ i,0.0f,-50.0f }, { i,0.0f,50.0f }, Colors::White);
		SimpleDraw::AddLine({ -50.0f,0.0f,i }, { 50.0f,0.0f,i }, Colors::White);
	}
	NFGE::Graphics::SimpleDraw::Render(NFGE::sApp.GetMainCamera());
	mModel_0.Render(NFGE::sApp.GetMainCamera());
	mModel_1.Render(NFGE::sApp.GetMainCamera());
	mModel_2.Render(NFGE::sApp.GetMainCamera());
	mSpaceShip.Render(NFGE::sApp.GetMainCamera());
}

void GameState::DebugUI()
{
	mSpaceShip.DebugUI();
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