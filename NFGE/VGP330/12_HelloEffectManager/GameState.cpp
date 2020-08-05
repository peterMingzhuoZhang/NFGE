#include "GameState.h"

void CameraControl(Camera& camera);

void GameState::Initialize()
{
	auto graphicSystem = GraphicsSystem::Get();
	GraphicsSystem::Get()->SetClearColor(Colors::DimGray);

	NFGE::sApp.GetMainCamera().SetPosition({ 0.0f, 0.0f, -10.0f });
	NFGE::sApp.GetMainCamera().SetDirection({ 0.0f, 0.0f, 1.0f });
	NFGE::sApp.GetMainCamera().SetFarPlane(500.0f);

	mMainLight = &NFGE::sApp.GetMainLight();
	mMainLight->direction = Vector3(0.5f,-0.5f,0.5f);
	mMainLight->ambient = NFGE::Graphics::Colors::White;
	mMainLight->diffuse = NFGE::Graphics::Colors::White;
	mMainLight->specular = NFGE::Graphics::Colors::White;

	mScreenTriangle.Load();
	mColorMesh.Load();
	mTextureMesh.Load();
	mStandardMesh.Load(mMainLight);
	mModelMesh.Load("../../Assets/Model/Spaceship.NFModelRV", mMainLight);
}

void GameState::Terminate()
{
	mScreenTriangle.Unload();
	mColorMesh.Unload();
	mTextureMesh.Unload();
	mStandardMesh.Unload();
	mModelMesh.UnLoad();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		NFGEApp::ShutDown();
		return;
	}

	CameraControl(NFGE::sApp.GetMainCamera());

	mScreenTriangle.Update(deltaTime);
	mColorMesh.Update(deltaTime);
	mTextureMesh.Update(deltaTime);
	mStandardMesh.Update(deltaTime);
	mModelMesh.Update(deltaTime);
}

void GameState::Render()
{
	for (float i = -50.0f; i <= 50.0f; i += 1.0f)
	{
		SimpleDraw::AddLine({ i,0.0f,-50.0f }, { i,0.0f,50.0f }, Colors::White);
		SimpleDraw::AddLine({ -50.0f,0.0f,i }, { 50.0f,0.0f,i }, Colors::White);
	}
	SimpleDraw::Render(NFGE::sApp.GetMainCamera());

	mScreenTriangle.Render(&NFGE::sApp.GetMainCamera());
	mColorMesh.Render(&NFGE::sApp.GetMainCamera());
	mTextureMesh.Render(&NFGE::sApp.GetMainCamera());
	mStandardMesh.Render(NFGE::sApp.GetMainCamera());
	//mModelMesh.Render(NFGE::sApp.GetMainCamera());

}

void GameState::DebugUI()
{
	PostProcessManager::Get()->OptionUI();

	auto& cameraDir = NFGE::sApp.GetMainCamera().GetDirection();
	auto& cameraPos = NFGE::sApp.GetMainCamera().GetPosition();
	ImGui::Text("Camera Pos, %f, %f, %f", cameraPos.x, cameraPos.y, cameraPos.z);
	ImGui::Text("Camera Dir, %f, %f, %f", cameraDir.x, cameraDir.y, cameraDir.z);

	ImGui::Begin("Light Setting");
	if (ImGui::CollapsingHeader("Light"))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mMainLight->direction.x, 0.05f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mMainLight->direction.y, 0.05f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mMainLight->direction.z, 0.05f, -1.0f, 1.0f);
		if (directionChanged)
		{
			mMainLight->direction = NFGE::Math::Normalize(mMainLight->direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mMainLight->ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mMainLight->diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mMainLight->specular.r);
	}

	ImGui::End();
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