#include "GameState.h"

const wchar_t* shaderFileName = L"../../Assets/Shaders/Texturing.fx";
const wchar_t* shaderFileName2 = L"../../Assets/Shaders/DoTransform.fx";
const wchar_t* shaderFileName3 = L"../../Assets/Shaders/DoLighting.fx";
const wchar_t* shaderFileName4 = L"../../Assets/Shaders/PostProcess_Standard.fx";
const wchar_t* shaderFileName5 = L"../../Assets/Shaders/PostProcess.fx";

void CameraControl(Camera& camera);

void GameState::Initialize()
{
	auto graphicSystem = GraphicsSystem::Get();
	GraphicsSystem::Get()->SetClearColor(Colors::DimGray);

	sphereStandard.Load({ 40.0f, 10.0f,10.0f }, 512, 512, 10.0f, shaderFileName4, shaderFileName4, "earth.jpg", "earth_spec.jpg", "earth_bump.jpg", "earth_normal.jpg");
	sphereStandard1.Load({ 80.0f, 10.0f,10.0f }, 512, 512, 10.0f, shaderFileName4, shaderFileName4, "earth.jpg", "earth_spec.jpg", "earth_bump.jpg", "earth_normal.jpg");
	sphereStandard2.Load({ 120.0f, 10.0f,10.0f }, 512, 512, 10.0f, shaderFileName4, shaderFileName4, "earth.jpg", "earth_spec.jpg", "earth_bump.jpg", "earth_normal.jpg");
	sphereStandard3.Load({ 0.0f, 10.0f,10.0f }, 512, 512, 10.0f, shaderFileName4, shaderFileName4, "earth.jpg", "earth_spec.jpg", "earth_bump.jpg", "earth_normal.jpg");
	sphereStandard4.Load({ -40.0f, 10.0f,10.0f }, 512, 512, 10.0f, shaderFileName4, shaderFileName4, "earth.jpg", "earth_spec.jpg", "earth_bump.jpg", "earth_normal.jpg");
	sphereStandard.IsSkyDome = false;

	NFGE::sApp.GetMainCamera().SetPosition({ -63.21f, 21.0f, -46.0f });
	NFGE::sApp.GetMainCamera().SetDirection({ 0.82f, -0.11f, 0.55f });
	NFGE::sApp.GetMainCamera().SetFarPlane(500.0f);

	mPostProcesser.Initialize();

}

void GameState::Terminate()
{
	mPostProcesser.Terminate();
	sphereStandard.Unload();
	sphereStandard1.Unload();
	sphereStandard2.Unload();
	sphereStandard3.Unload();
	sphereStandard4.Unload();

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
}

void GameState::Render()
{
	/*for (float i = -50.0f; i <= 50.0f; i += 1.0f)
	{
		SimpleDraw::AddLine({ i,0.0f,-50.0f }, { i,0.0f,50.0f }, Colors::DarkGray);
		SimpleDraw::AddLine({ -50.0f,0.0f,i }, { 50.0f,0.0f,i }, Colors::DarkGray);
	}
*/
	if (mIsPostProcessOn)
	{
		PostProcess();
	}
	else
	{
		RenderScene();
	}

}

void GameState::DebugUI()
{
	ImGui::Begin("Standard Setting");
	if (ImGui::CollapsingHeader("Light"))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &sphereStandard.mLight.direction.x, 0.05f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &sphereStandard.mLight.direction.y, 0.05f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &sphereStandard.mLight.direction.z, 0.05f, -1.0f, 1.0f);
		if (directionChanged)
		{
			sphereStandard.mLight.direction = NFGE::Math::Normalize(sphereStandard.mLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &sphereStandard.mLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &sphereStandard.mLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &sphereStandard.mLight.specular.r);
	}

	if (ImGui::CollapsingHeader("Material"))
	{

		ImGui::ColorEdit4("Ambient##Material", &sphereStandard.mMaterial.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &sphereStandard.mMaterial.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &sphereStandard.mMaterial.specular.r);
		ImGui::DragFloat("Power##Material", &sphereStandard.mMaterial.power, 1.0f, 0.0f, 100.0f);
	}

	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat("rotationX##Transform", &sphereStandard.mRotation.x, 1.0f, 0.0f, 360.0f);
		ImGui::DragFloat("rotationY##Transform", &sphereStandard.mRotation.y, 1.0f, 0.0f, 360.0f);
		ImGui::DragFloat("rotationZ##Transform", &sphereStandard.mRotation.z, 1.0f, 0.0f, 360.0f);
	}

	ImGui::DragFloat("bumpWeight", &sphereStandard.mBumpWeight, 0.01f, 0.0f, 10.0f);

	ImGui::Checkbox("Is Post Processing On", &mIsPostProcessOn);

	ImGui::End();

	auto& cameraDir = NFGE::sApp.GetMainCamera().GetDirection();
	auto& cameraPos = NFGE::sApp.GetMainCamera().GetPosition();
	ImGui::Text("Camera Pos, %f, %f, %f", cameraPos.x, cameraPos.y, cameraPos.z);
	ImGui::Text("Camera Dir, %f, %f, %f", cameraDir.x, cameraDir.y, cameraDir.z);

	mPostProcesser.DebugUI();
	
}

void GameState::RenderScene()
{
	/*for (float i = -50.0f; i <= 50.0f; i += 1.0f)
	{
		SimpleDraw::AddLine({ i,0.0f,-50.0f }, { i,0.0f,50.0f }, Colors::DarkGray);
		SimpleDraw::AddLine({ -50.0f,0.0f,i }, { 50.0f,0.0f,i }, Colors::DarkGray);
	}*/

	SimpleDraw::Render(NFGE::sApp.GetMainCamera());
	sphereStandard.Render(NFGE::sApp.GetMainCamera());
	sphereStandard1.Render(NFGE::sApp.GetMainCamera());
	sphereStandard2.Render(NFGE::sApp.GetMainCamera());
	sphereStandard3.Render(NFGE::sApp.GetMainCamera());
	sphereStandard4.Render(NFGE::sApp.GetMainCamera());
}

void GameState::PostProcess()
{
	for (int i = 0; i < mPostProcesser.step-1; i++)
	{
		mPostProcesser.mRenderTargets[i]->BeginRender();
		mPostProcesser.Prepare(i);
		RenderSceneWithoutShader();
		mPostProcesser.mRenderTargets[i]->EndRender();
	}
	
	mPostProcesser.mRenderTargets.back()->BeginRender();
	RenderScene();
	mPostProcesser.mRenderTargets.back()->EndRender();

	PostProcessManager::Get()->GetMasterRenderTarget().ResumeRender();
	mPostProcesser.Render();
	PostProcessManager::Get()->GetMasterRenderTarget().EndRender();
}

void GameState::RenderSceneWithoutShader()
{
	sphereStandard.PostProcess_Render(NFGE::sApp.GetMainCamera());
	sphereStandard1.PostProcess_Render(NFGE::sApp.GetMainCamera());
	sphereStandard2.PostProcess_Render(NFGE::sApp.GetMainCamera());
	sphereStandard3.PostProcess_Render(NFGE::sApp.GetMainCamera());
	sphereStandard4.PostProcess_Render(NFGE::sApp.GetMainCamera());
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