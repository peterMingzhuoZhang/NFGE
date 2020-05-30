#include "EditState.h"


void CameraControl(Camera& camera);

void EditState::Initialize()
{
	auto graphicSystem = GraphicsSystem::Get();
	GraphicsSystem::Get()->SetClearColor(Colors::DimGray);
	//camera.SetPosition({ 0.0f, 0.0f, -10.0f });
	//camera.SetDirection({ 0.0f, 0.0f, 1.0f });

	spherePC.Load({ 10.0f, 10.0f,10.0f }, 20, 20, 10.0f);
	spherePN.Load({ -10.0f, 10.0f,10.0f }, 50, 50, 10.0f);
	spherePX.Load({ -30.0f, 10.0f,10.0f }, 20, 20, 10.0f, "earth.jpg");
	sphereStandard.Load({ 40.0f, 10.0f,10.0f }, 512, 512, 10.0f, "earth.jpg", "earth_spec.jpg", "earth_bump.jpg", "earth_normal.jpg");
	sphereSkydome.Load({ 0.0f, 0.0f,0.0f }, 50, 50, 200.0f, "universeDome2.jpg");
	sphereSkydome.IsSkyDome = true;

	auto cameraService = mWorld.AddService<NFGE::CameraService>();
	mCamera = cameraService->AddCamera("Editor");
	mCamera->SetPosition({ 0.0f,0.0f,-10.0f });
	mCamera->SetDirection({ 0.0f,0.0f,1.0f });
	cameraService->SetActiveCamera("Editor");

	auto terrainService = mWorld.AddService<NFGE::TerrainService>();

	mWorld.Initialize(10000);
	mWorld.Create("../../Assets/Templates/test.json", "testObject");

}


void EditState::Terminate()
{
	spherePC.Unload();
	spherePN.Unload();
	spherePX.Unload();
	sphereStandard.Unload();
	sphereSkydome.Unload();
	mWorld.Terminate();
}

void EditState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		NFGEApp::ShutDown();
		return;
	}

	mWorld.Update(deltaTime);
}

void EditState::Render()
{
	//mRenderTarget.BeginRender();
	RenderScene();
	//mRenderTarget.EndRender();

	//PostProcess();
}

void EditState::DebugUI()
{
	//ImGuI Editor
	mEditor.ShowMenuBar();
	mEditor.ShowMainWindowWithDockSpace();
	mEditor.ShowSceneView();
	mEditor.ShowWorldView();
	mEditor.ShowInspectorView();
	mEditor.ShowUtil();
	PostProcessManager::Get()->OptionUI();

	auto light = NFGE::sApp.GetMainLight();

	ImGui::Begin("PN Setting");
	if (ImGui::CollapsingHeader("Light"))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &light.direction.x, 0.05f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &light.direction.y, 0.05f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &light.direction.z, 0.05f, -1.0f, 1.0f);
		if (directionChanged)
		{
			light.direction = NFGE::Math::Normalize(light.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &light.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &light.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &light.specular.r);
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
		directionChanged |= ImGui::DragFloat("Direction X##Light", &light.direction.x, 0.05f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &light.direction.y, 0.05f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &light.direction.z, 0.05f, -1.0f, 1.0f);
		if (directionChanged)
		{
			light.direction = NFGE::Math::Normalize(light.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &light.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &light.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &light.specular.r);
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
		auto rotationData = &sphereStandard.mEffectContext.rotation;
		ImGui::DragFloat3("rotationX##Transform", (float*)rotationData);

		NFGE::Math::Vector3 eurlaAngle = NFGE::Math::GetEular(*rotationData);
		eurlaAngle *= NFGE::Math::Constants::RadToDeg;

		eurlaAngle *= NFGE::Math::Constants::DegToRad;

		*rotationData = NFGE::Math::Quaternion::ToQuaternion(eurlaAngle.x, eurlaAngle.y, eurlaAngle.z); 
		
	}

	ImGui::DragFloat("bumpWeight", &sphereStandard.mBumpWeight, 0.01f, 0.0f, 10.0f);

	ImGui::End();
}

void EditState::RenderScene()
{
	for (float i = -50.0f; i <= 50.0f; i += 1.0f)
	{
		SimpleDraw::AddLine({ i,0.0f,-50.0f }, { i,0.0f,50.0f }, Colors::DarkGray);
		SimpleDraw::AddLine({ -50.0f,0.0f,i }, { 50.0f,0.0f,i }, Colors::DarkGray);
	}

	mWorld.Render();
	SimpleDraw::Render(*mCamera);

	spherePC.Render(*mCamera);
	spherePN.Render(*mCamera);
	spherePX.Render(*mCamera);
	sphereStandard.Render(*mCamera);
	sphereSkydome.Render(*mCamera);

}

