#include "GameState.h"

const float cameraMoveSpeed = 20.0f;
const float cameraTurnSpeed = 20.0f;

using namespace NFGE::Input;

namespace
{
	const wchar_t* floorShaderFileName = L"../../Assets/Shaders/Standard_Model_ZMZ.fx";
	const char* floorSpecular = "Wood_012_Tiles_ambientOcclusion.jpg";
	const char* floorDiffuse = "Cobblestone_001_COLOR.jpg";
	const char* floorNormal = "Wood_012_Tiles_normal.jpg";
	const char* floorDisplacement = "Wood_012_Tiles_height.png";

	const char* defaultIdleAnimation = "../../Assets/Model/Idle.NFAnimationSet";
	const char* boxingIdleAnimation = "../../Assets/Model/Idle_boxing.NFAnimationSet";
	const char* dopeIdleAnimation = "../../Assets/Model/Idle_dope.NFAnimationSet";
	const char* noFeelingIdleAnimation = "../../Assets/Model/Idle_noFeeling.NFAnimationSet";
	const char* powerIdleAnimation = "../../Assets/Model/Idle_power.NFAnimationSet";

	NFGE::Math::Plane obbPlane_high{ 0.0f, 1.0f, 0.0f, 160.0f };
	NFGE::Math::Plane obbPlane_middle{ 0.0f, 1.0f, 0.0f, 130.0f };
	NFGE::Math::Plane obbPlane_low{ 0.0f, 1.0f, 0.0f, 100.0f };
	NFGE::Math::Vector3 obbNonEffectPosition{ 0.0f,-10.0f,0.0f };
}

void GameState::Initialize()
{
	auto& camera = NFGE::sApp.GetMainCamera();
	camera.SetPosition({ -50.0f,200.0f, -200.0f });
	camera.SetDirection({ 0.6f,-0.3f, 0.7f });
	camera.SetFarPlane(10000.0f);

	GraphicsSystem::Get()->SetClearColor(NFGE::Graphics::Colors::DarkSlateGray);

	mLight = &NFGE::sApp.GetMainLight();
	mLight->direction = NFGE::Math::Normalize({ 1.0f,-1.0f,0.0f });
	mLight->ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mLight->diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	mLight->specular = { 0.5f, 0.5f, 0.5f, 1.0f };

	mPlane.DefaultInitialize(mLight);
	
	mYBots.reserve(10);
	mYBots.emplace_back(std::make_unique<YBot>(mWorld3D, mPhysicsWorld));
	mYBots.emplace_back(std::make_unique<YBot>(mWorld3D, mPhysicsWorld));

	mYBots[0]->Load(dopeIdleAnimation);
	mYBots[1]->Load(boxingIdleAnimation, Graphics::Colors::Pink);
	mYBots[1]->SetInitPosition(Vector3{ 200.0f, 0.0f, 500.0f });
	mYBots[1]->heading = Vector3{ -1.0f,0.0f,0.0f };

	//mPhysicsWorld.AddOBB(OBB{ { 0.0f,150.0f,200.0f },{ 100.0f,10.0f,100.0f },{ Math::QuaternionRotationAxis(Math::Vector3::XAxis, -0.5f) } });
	mCollisionOBB = mPhysicsWorld.AddOBB(OBB{ { 0.0f,22.0f,0.0f },{ 10.0f,10.0f,10.0f },{ Math::Quaternion::Identity() } });
}

void GameState::Terminate()
{
	for (auto& ybot : mYBots)
	{
		ybot->UnLoad();
	}
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

	if (inputSystem->IsKeyPressed(KeyCode::TAB))
	{
		mCurrentControlIndex = mCurrentControlIndex == 0 ? -1 : 0;
		if (mCurrentControlIndex != -1)
			mYBots[mCurrentControlIndex]->GetController()->Set();
	}

	for (auto& ybot : mYBots)
	{
		ybot->Update(deltaTime);
	}

	if (mCurrentControlIndex == -1)
	{
		NFGE::sApp.SoSoCameraControl(cameraTurnSpeed, cameraMoveSpeed, NFGE::sApp.GetMainCamera(), deltaTime);
	}
	else
	{
		mYBots[mCurrentControlIndex]->GetController()->Update(deltaTime);
		mYBots[mCurrentControlIndex]->HookCamera(NFGE::sApp.GetMainCamera(), deltaTime);
	}

	mPhysicsWorld.Update(deltaTime);

	// Head rig Update;
	std::list<NFGE::Math::Vector3> lookTargets;
	for (auto& target : mYBots)
	{
		lookTargets.push_back(target->GetHeadRigBone()->GetPosition());
	}
	for (auto& ybot : mYBots)
	{
		ybot->Looking(lookTargets);
	}

	OBBControl();
}

void GameState::Render()
{
	BlendStateManager::Get()->GetBlendState("AlphaBlend")->Set();
	
	mPlane.Render(NFGE::Math::Matrix4::sIdentity(), NFGE::sApp.GetMainCamera());

	for (auto& ybot : mYBots)
	{
		ybot->Render(NFGE::sApp.GetMainCamera());
	}

}

void GameState::DebugUI()
{
	//PostProcessManager::Get()->OptionUI();

	mYBots[0]->DebugUI();
	mYBots[0]->GetController()->DebugUI();

	ImGui::Begin("Light direction");
	bool directionChanged = false;
	directionChanged |= ImGui::DragFloat("Direction X##Light", &mLight->direction.x, 0.05f, -1.0f, 1.0f);
	directionChanged |= ImGui::DragFloat("Direction Y##Light", &mLight->direction.y, 0.05f, -1.0f, 1.0f);
	directionChanged |= ImGui::DragFloat("Direction Z##Light", &mLight->direction.z, 0.05f, -1.0f, 1.0f);
	if (directionChanged)
	{
		mLight->direction = NFGE::Math::Normalize(mLight->direction);
	}

	ImGui::End();

	mPhysicsWorld.DebugDraw();

	ShowUI();

	for (float i = -50.0f; i <= 50.0f; i += 1.0f)
	{
		SimpleDraw::AddLine({ i,0.0f,-50.0f }, { i,0.0f,50.0f }, Colors::White);
		SimpleDraw::AddLine({ -50.0f,0.0f,i }, { 50.0f,0.0f,i }, Colors::White);
	}
	Vector3 origin{ 0.0f,0.0f,0.0f };
	NFGE::sApp.DrawLine(origin, origin + Vector3::ZAxis * 200.0f, NFGE::Graphics::Colors::Red);
	NFGE::sApp.DrawSphere(origin + Vector3::ZAxis * 200.0f, 2.0f, NFGE::Graphics::Colors::Red);
	NFGE::sApp.DrawLine(origin, origin + Vector3::YAxis * 200.0f, NFGE::Graphics::Colors::Green);
	NFGE::sApp.DrawSphere(origin + Vector3::YAxis* 200.0f, 2.0f, NFGE::Graphics::Colors::Green);
	NFGE::sApp.DrawLine(origin, origin + Vector3::XAxis * 200.0f, NFGE::Graphics::Colors::Cyan);
	NFGE::sApp.DrawSphere(origin + Vector3::XAxis  * 200.0f, 2.0f, NFGE::Graphics::Colors::Cyan);
	//NFGE::Graphics::SimpleDraw::Render(NFGE::sApp.GetMainCamera());
}

void GameState::ShowUI()
{
	PostProcessManager::Get()->OptionUI();

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

}

void GameState::OBBControl()
{
	auto inputSystem = InputSystem::Get();

	NFGE::Math::Plane obbPlane;

	if (inputSystem->IsKeyDown(Input::KeyCode::ONE))
		obbPlane = obbPlane_high;
	if (inputSystem->IsKeyDown(Input::KeyCode::TWO))
		obbPlane = obbPlane_middle;
	if (inputSystem->IsKeyDown(Input::KeyCode::THREE))
		obbPlane = obbPlane_low;

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
