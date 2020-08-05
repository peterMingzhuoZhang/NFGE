#include "GameState.h"

const wchar_t* shaderFileName = L"../../Assets/Shaders/Texturing.fx";
const wchar_t* shaderFileName2 = L"../../Assets/Shaders/DoTransform.fx";
const wchar_t* shaderFileName3 = L"../../Assets/Shaders/DoLighting.fx";
const wchar_t* shaderFileName4 = L"../../Assets/Shaders/Standard.fx";
const wchar_t* shaderFileName5 = L"../../Assets/Shaders/PostProcess.fx";
const wchar_t* shaderFileName6 = L"../../Assets/Shaders/PostProcess_GetDepth.fx";

void CameraControl(Camera& camera);

void GameState::Initialize()
{
	auto graphicSystem = GraphicsSystem::Get();
	GraphicsSystem::Get()->SetClearColor(Colors::DimGray);
	camera.SetPosition({ 0.0f, 0.0f, -10.0f });
	camera.SetDirection({ 0.0f, 0.0f, 1.0f });

	mScreenMesh = MeshBuilder::CreateNDCQuad();
	mScreenMeshBuffer.Initialize(mScreenMesh);

	mPostProcessPixelShader.Initialize(shaderFileName5);
	mPostProcessVertexShader.Initialize(shaderFileName5,VertexPX::Format);

	spherePC.Load({ 10.0f, 10.0f,10.0f }, 20, 20, 10.0f, shaderFileName2, shaderFileName2);
	spherePN.Load({ -10.0f, 10.0f,10.0f }, 50, 50, 10.0f, shaderFileName3, shaderFileName3);
	spherePX.Load({ -30.0f, 10.0f,10.0f }, 20, 20, 10.0f, shaderFileName, shaderFileName, "earth.jpg");
	sphereStandard.Load({ 40.0f, 10.0f,10.0f }, 512, 512, 10.0f, shaderFileName4, shaderFileName4, "earth.jpg", "earth_spec.jpg", "earth_bump.jpg", "earth_normal.jpg");
	sphereSkydome.Load({ 0.0f, 0.0f,0.0f }, 50, 50, 200.0f, shaderFileName, shaderFileName, "universeDome2.jpg");
	sphereSkydome.IsSkyDome = true;

	mRenderTarget.Initialize(graphicSystem->GetBackBufferWidth(), graphicSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
}

void GameState::Terminate()
{
	mScreenMeshBuffer.Terminate();
	mPostProcessPixelShader.Terminate();
	mPostProcessVertexShader.Terminate();

	spherePC.Unload();
	spherePN.Unload();
	spherePX.Unload();
	sphereStandard.Unload();
	sphereSkydome.Unload();

	mRenderTarget.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		NFGEApp::ShutDown();
		return;
	}

	CameraControl(camera);
}

void GameState::Render()
{
	mRenderTarget.BeginRender();
	RenderScene();
	mRenderTarget.EndRender();

	PostProcess();
}

void GameState::DebugUI()
{
	ImGui::Begin("PN Setting");
	if(ImGui::CollapsingHeader("Light"))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &spherePN.mLight.direction.x, 0.05f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &spherePN.mLight.direction.y, 0.05f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &spherePN.mLight.direction.z, 0.05f, -1.0f, 1.0f);
		if (directionChanged)
		{
			spherePN.mLight.direction = NFGE::Math::Normalize(spherePN.mLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &spherePN.mLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &spherePN.mLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &spherePN.mLight.specular.r);
	}

	if (ImGui::CollapsingHeader("Material"))
	{
		
		ImGui::ColorEdit4("Ambient##Material", &spherePN.mMaterial.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &spherePN.mMaterial.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &spherePN.mMaterial.specular.r);
		ImGui::DragFloat("Power##Light", &spherePN.mMaterial.power, 1.0f, 0.0f, 100.0f);
	}
	
	ImGui::End();

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

	ImGui::End();
	
}

void GameState::RenderScene()
{
	for (float i = -50.0f; i <= 50.0f; i += 1.0f)
	{
		SimpleDraw::AddLine({ i,0.0f,-50.0f }, { i,0.0f,50.0f }, Colors::DarkGray);
		SimpleDraw::AddLine({ -50.0f,0.0f,i }, { 50.0f,0.0f,i }, Colors::DarkGray);
	}

	SimpleDraw::Render(camera);

	spherePC.Render(camera);
	spherePN.Render(camera);
	spherePX.Render(camera);
	sphereStandard.Render(camera);
	sphereSkydome.Render(camera);
}

void GameState::PostProcess()
{
	mPostProcessVertexShader.Bind();
	mPostProcessPixelShader.Bind();

	SamplerManager::Get()->GetSampler("PointWrap")->BindPS(0);
	
	PostProcessManager::Get()->GetMasterRenderTarget().ResumeRender();
	mRenderTarget.BindPS(0);
	mScreenMeshBuffer.Render();
	mRenderTarget.UnbindPS(0);
	PostProcessManager::Get()->GetMasterRenderTarget().EndRender();
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