#include "GameState.h"

namespace
{
	const float cameraMoveSpeed = 20.0f;
	const float cameraTurnSpeed = 20.0f;

	NFGE::Math::Plane obbPlane{ 0.0f, 1.0f, 0.0f, 20.0f };
	NFGE::Math::Vector3 obbNonEffectPosition{ 0.0f,-10.0f,0.0f };

	const char* PlaneTextureName = "rock.jpg";
	const char* SkyDomTextureName = "SpaceSkydom.jpg";
	int GrassCount = 42;
	int GrassSegCount = 20;

	NFGE::Math::Vector3 InitGrassPos = { 0.0f,0.0f,0.0f };
	int GrassCol = 7;
	float GrassGapX = 5.0f;
	float GrassGapZ = 5.0f;

	bool IsDebugDraw = false;
}

void GameState::Initialize()
{
	groundPlane.AddComponent<NFGE::TransformComponent>();
	auto meshRenderComponent = groundPlane.AddComponent<NFGE::MeshRenderComponent>();
	meshRenderComponent->mControlContext.mDiffuseTextureDir.mFileName = PlaneTextureName;
	meshRenderComponent->mControlContext.mIsUsingDiffuse = true;
	meshRenderComponent->mEffectContext.AdjustRotation = { 90.0f,0.0f,0.0f };
	meshRenderComponent->mEffectContext.AdjustScale = { 10.0f,10.0f,1.0f };
	groundPlane.Initialize();

	skyDom.AddComponent<NFGE::TransformComponent>();
	auto meshRenderComponent_skydom = skyDom.AddComponent<NFGE::MeshRenderComponent>();
	meshRenderComponent_skydom->mGemotryType = 2;
	meshRenderComponent_skydom->mControlContext.mDiffuseTextureDir.mFileName = SkyDomTextureName;
	meshRenderComponent_skydom->mControlContext.mIsUsingDiffuse = true;
	meshRenderComponent_skydom->mEffectContext.AdjustScale = { 200.0f,200.0f,200.0f };
	meshRenderComponent_skydom->mEffectContext.rasterizerState = "CullFrontSolid";
	skyDom.Initialize();


	grasses.reserve(GrassCount);
	for (size_t i = 0; i < GrassCount; i++)
	{
		auto& theGrass = grasses.emplace_back();
		theGrass.Load(GrassSegCount, 2.0f, "Grass_mesh.png", "", "", "");
		theGrass.SetPosition({ InitGrassPos.x + i % GrassCol * GrassGapX, 0.0f, InitGrassPos.y + i / GrassCol * GrassGapZ });
		theGrass.InitPhysics(mPhysicsWorld);
	}

	mPhysicsWorld.Initialize({});
	mCollisionOBB = mPhysicsWorld.AddOBB(OBB{ { 0.0f,22.0f,0.0f },{ 3.0f,2.0f,3.0f },{ Math::Quaternion::Identity()} });
}
void GameState::Terminate() 
{
	groundPlane.Terminate();
	skyDom.Terminate();

	for (auto& grass : grasses)
	{
		grass.UnLoad();
	}
	mPhysicsWorld.ClearDynamic();
}
void GameState::Update(float deltaTime)
{
	groundPlane.Update(deltaTime);

	skyDom.Update(deltaTime);
	skyDom.GetComponent<NFGE::TransformComponent>()->position = NFGE::sApp.GetMainCamera().GetPosition();
	
	for (auto& grass : grasses)
	{
		grass.Update(deltaTime);
	}

	mPhysicsWorld.Update(deltaTime);

	CameraControl(NFGE::sApp.GetMainCamera(), deltaTime);
	OBBControl();
}
void GameState::Render()
{
	groundPlane.Render();
	skyDom.Render();

	for (auto& grass : grasses)
	{
		grass.Render(NFGE::sApp.GetMainCamera());
	}
}
void GameState::DebugUI()
{
	auto& cameraDir = NFGE::sApp.GetMainCamera().GetDirection();
	auto& cameraPos = NFGE::sApp.GetMainCamera().GetPosition();
	ImGui::Text("Camera Pos, %f, %f, %f", cameraPos.x, cameraPos.y, cameraPos.z);
	ImGui::Text("Camera Dir, %f, %f, %f", cameraDir.x, cameraDir.y, cameraDir.z);
	ImGui::Text("Obb Pos, %f, %f, %f", mCollisionOBB->center.x, mCollisionOBB->center.y, mCollisionOBB->center.z);
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
	if (ImGui::Button("Debug Draw"))
	{
		IsDebugDraw = !IsDebugDraw;
	}

	if (IsDebugDraw)
		mPhysicsWorld.DebugDraw();
}

void GameState::CameraControl(Camera& camera, float deltaTime)
{
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

void GameState::OBBControl()
{
	auto inputSystem = InputSystem::Get();
	
	if (inputSystem->IsMouseDown(Input::MouseButton::LBUTTON))
	{
		float mouseX = (float)inputSystem->GetMouseScreenX();
		float mouseY = (float)inputSystem->GetMouseScreenY();

		auto camera = NFGE::sApp.GetMainCamera();
		NFGE::Math::Matrix4 NDCToWorldMat = NFGE::Math::Inverse(camera.GetViewMatrix() * camera.GetPerspectiveMatrix());
		NFGE::Math::Vector3 mousePositionInWorld = Vector3
		(
			(mouseX / GraphicsSystem::Get()->GetBackBufferWidth()) * 2.0f - 1.0f,	// NDC space x is -1.0 to 1.0f, need special conversion 
			(mouseY / GraphicsSystem::Get()->GetBackBufferHeight()) * -2.0f + 1.0f, // NDC space y is -1.0 to 1.0f, need special conversion 
			0.0f
		) * NDCToWorldMat;

		NFGE::Math::Ray cameraRay = NFGE::Math::Ray(camera.GetPosition(), NFGE::Math::Normalize(mousePositionInWorld - camera.GetPosition()));

		float toPlaneDis;
		NFGE::Math::Intersect(cameraRay, obbPlane, toPlaneDis);

		mCollisionOBB->center = camera.GetPosition() + cameraRay.dir * toPlaneDis;

		SimpleDraw::AddOBB(*mCollisionOBB, NFGE::Graphics::Colors::AliceBlue);
	}
	else
	{
		mCollisionOBB->center = obbNonEffectPosition;
	}
	

	

}
