#include "GameState.h"

using namespace NFGE::Input;
using namespace NFGE::Math;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,0.0f,-10.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });
	mMesh = MeshBuilder::CreateSphere(20, 20, 10.0f);
	mMeshBuffer.Initialize(mMesh);

	mVertexShader.Initialize(L"../../Assets/Shaders/Standard.fx", Vertex::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/Standard.fx");
	mDiffuseMap = NFGE::Graphics::TextureManager::Get()->LoadTexture("earth.jpg");
	mSpecularMap = NFGE::Graphics::TextureManager::Get()->LoadTexture("earth_spec.jpg");
	//mSpecularMap = NFGE::Graphics::TextureManager::Get()->LoadTexture("brickwork_spec.jpg");

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mLight.direction = NFGE::Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	mLight.specular = { 0.1f, 0.1f, 0.1f, 1.0f };

	mMaterial.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mMaterial.diffuse = { 0.8f, 0.7f, 0.1f, 1.0f };
	mMaterial.specular = { 0.2f, 0.2f, 0.2f, 1.0f };
	mMaterial.power = 10.0f;
}

void GameState::Terminate()
{
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::ESCAPE)) { NFGEApp::ShutDown(); return; }
	// Camera transation
	if (inputSystem->IsKeyDown(KeyCode::W)) { mCamera.Walk(mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::S)) { mCamera.Walk(-mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::A)) { mCamera.Strafe(mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::D)) { mCamera.Strafe(-mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::Q)) { mCamera.Rise(mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::E)) { mCamera.Rise(-mMoveSpeed * deltaTime); }
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * mTurnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * mTurnSpeed * deltaTime);
	}
}

void GameState::Render()
{
	
	mVertexShader.Bind();
	mPixelShader.Bind();
	NFGE::Graphics::TextureManager::Get()->GetTexture(mDiffuseMap)->BindPS(0);
	NFGE::Graphics::TextureManager::Get()->GetTexture(mSpecularMap)->BindPS(1);
	NFGE::Graphics::SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();

	auto world = NFGE::Math::Translation(mPosition);
	auto view = mCamera.GetViewMatrix();
	auto proj = mCamera.GetPerspectiveMatrix();

	TransformData transformData;
	transformData.wvp = NFGE::Math::Transpose(world*view*proj);
	transformData.world = NFGE::Math::Transpose(world);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Load(transformData);
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mLightBuffer.Load(mLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Load(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	
	mMeshBuffer.Render();
}

void GameState::DebugUI()
{
	ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	
	if (ImGui::CollapsingHeader("Light"))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mLight.direction.x, 0.1f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mLight.direction.y, 0.1f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mLight.direction.z, 0.1f, -1.0f, 1.0f);
		if (directionChanged) 
		{
			mLight.direction = NFGE::Math::Normalize(mLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mLight.specular.r);
	}
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.r);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 0.1f, 0.0f, 100.0f);
	}
	ImGui::End();
}

