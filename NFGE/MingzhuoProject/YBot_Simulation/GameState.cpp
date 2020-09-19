#include "GameState.h"

const float cameraMoveSpeed = 50.0f;
const float cameraTurnSpeed = 20.0f;

using namespace NFGE::Input;

namespace
{
	bool IsShowWorldDebug = false;

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
	NFGE::Math::Plane obbPlane_low_1{ 0.0f, 1.0f, 0.0f, 70.0f };
	NFGE::Math::Plane obbPlane_low_2{ 0.0f, 1.0f, 0.0f, 35.0f };
	NFGE::Math::Vector3 obbNonEffectPosition{ 0.0f,-20.0f,0.0f };

	NFGE::Math::Vector3 wallPosition{ 100.0f,100.0f,1500.0f };
	NFGE::Math::Vector3 roofPosition{ -100.0f,210.0f,2500.0f };

	OBB* InitCubeGameObject(NFGE::Physics::PhysicsWorld& world, GameObject& cube, const Vector3& position, const Vector3& size)
	{
		// init cube 1
		auto transFormComponent = cube.AddComponent<NFGE::TransformComponent>();
		transFormComponent->position = position;
		transFormComponent->scale = size;
		auto renderComponent = cube.AddComponent<NFGE::MeshRenderComponent>();
		renderComponent->mGemotryType = 1;
		renderComponent->mControlContext.mDiffuseTextureDir.mFileName = "Cobblestone_001_COLOR.jpg";
		renderComponent->mControlContext.mIsUsingDiffuse = true;
		renderComponent->mControlContext.mNormalextureDir.mFileName = "Cobblestone_001_NORM.jpg";
		renderComponent->mControlContext.mIsUsingNormal = true;
		renderComponent->mControlContext.mIsCastShadow = true;
		renderComponent->Initialize();
		return world.AddOBB(OBB{ position,size,{ Math::Quaternion::Identity() } });
	}
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

	// init cube 1
	InitCubeGameObject(mPhysicsWorld, mCube_0, Vector3{ 200.0f,-100.0f,700.0f }, Vector3{ 100.0f,50.0f,100.0f });
	// init cube 2
	InitCubeGameObject(mPhysicsWorld, mCube_1, Vector3{ 400.0f,50.0f,500.0f }, Vector3{ 100.0f,50.0f,100.0f });
	// init cube 3
	InitCubeGameObject(mPhysicsWorld, mCube_2, Vector3{ 200.0f,-100.0f,300.0f }, Vector3{ 100.0f,50.0f,100.0f });
	// init cube wall
	mWallObb = InitCubeGameObject(mPhysicsWorld, mCube_wall, wallPosition, Vector3{ 20.0f,100.0f,200.0f });
	// init cube roof
	mRoofObb = InitCubeGameObject(mPhysicsWorld, mCube_roof, roofPosition, Vector3{ 200.0f,20.0f,200.0f });
	
	mYBots.reserve(10);
	mYBots.emplace_back(std::make_unique<YBot>(mWorld3D, mPhysicsWorld));
	//mYBots.emplace_back(std::make_unique<YBot>(mWorld3D, mPhysicsWorld));
	//mYBots.emplace_back(std::make_unique<YBot>(mWorld3D, mPhysicsWorld));
	//mYBots.emplace_back(std::make_unique<YBot>(mWorld3D, mPhysicsWorld));
	//mYBots.emplace_back(std::make_unique<YBot>(mWorld3D, mPhysicsWorld));

	mYBots[0]->Load(dopeIdleAnimation);
	//mYBots[1]->Load(boxingIdleAnimation, Graphics::Colors::Pink);
	//mYBots[1]->SetInitPosition(Vector3{ 200.0f, 0.0f, 500.0f });
	//mYBots[1]->heading = Vector3{ -1.0f,0.0f,0.0f };
	//mYBots[2]->Load(dopeIdleAnimation, Graphics::Colors::Orange);
	//mYBots[2]->SetInitPosition(Vector3{ -200.0f, 0.0f, 500.0f });
	//mYBots[2]->heading = Vector3{ 1.0f,0.0f,0.0f };
	//mYBots[3]->Load(noFeelingIdleAnimation, Graphics::Colors::Green);
	//mYBots[3]->SetInitPosition(Vector3{ 200.0f, 0.0f, 900.0f });
	//mYBots[3]->heading = Vector3{ -1.0f,0.0f,0.0f };
	//mYBots[4]->Load(powerIdleAnimation, Graphics::Colors::Red);
	//mYBots[4]->SetInitPosition(Vector3{ -200.0f, 0.0f, 900.0f });
	//mYBots[4]->heading = Vector3{ 1.0f,0.0f,0.0f };

	mPhysicsWorld.AddOBB(OBB{ { 0.0f,-250.0f,0.0f },{ 5000.0f,250.0f,5000.0f },{ Math::Quaternion::Identity()} });
	mCollisionOBB = mPhysicsWorld.AddOBB(OBB{ { 0.0f,22.0f,0.0f },{ 10.0f,10.0f,10.0f },{ Math::Quaternion::Identity() } });
	mPhysicsWorld.AddPlane(NFGE::Math::Plane{});
	mPhysicsWorld.Initialize({
		Math::Vector3{ 0.0f,-9.8f,0.0f },
		1.0f / 60.0f,
		0.0f, 
		1 ,
		});

	mTouchPoints.reserve(2);
	mTouchPoints.push_back({});//for wall
	mTouchPoints.push_back({});//for roof
}

void GameState::Terminate()
{
	for (auto& ybot : mYBots)
	{
		ybot->UnLoad();
	}
	mCube_0.Terminate();
	mCube_1.Terminate();
	mCube_2.Terminate();
	mCube_wall.Terminate();
	mCube_roof.Terminate();
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
		mCurrentControlIndex = mCurrentControlIndex + 1 == mYBots.size() ? -1 : mCurrentControlIndex + 1; // switch index between mybots index and -1
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
		lookTargets.push_back(target->GetHeadRigBone().GetPosition());
	}
	for (auto& ybot : mYBots)
	{
		ybot->Looking(lookTargets);
		//ybot->SenceHitingTarget(lookTargets);
		//
		//
		//for (auto& hiter : mYBots)
		//{
		//	if (((NFGE::Math::Distance(ybot->GetHeadRigBone().GetPosition(), hiter->GetLeftHandPosition()) <= 20.0f)&&(hiter->GetLeftArmTargetting())) ||
		//		((NFGE::Math::Distance(ybot->GetHeadRigBone().GetPosition(), hiter->GetRightHandPosition()) <= 20.0f) && (hiter->GetRightArmTargetting())))
		//	{
		//		ybot->SpwanRagDoll();
		//		ybot->mRagBody.mHip.pCenter->SetVelocity(Normalize(ybot->GetHeadRigBone().GetPosition() - hiter->GetLeftHandPosition()) * hiter->mHitPower);
		//		hiter->SetLeftArmTargetting(false);
		//		hiter->SetRightArmTargetting(false);
		//	}
		//}
	}
	
	mCube_0.Update(deltaTime);
	mCube_1.Update(deltaTime);
	mCube_2.Update(deltaTime);
	mCube_wall.Update(deltaTime);
	mCube_roof.Update(deltaTime);

	OBBControl();

		
	if (mCurrentControlIndex != -1)
	{
		mYBots[mCurrentControlIndex]->Touching(mTouchPoints);
		auto headPosition = mYBots[mCurrentControlIndex]->GetHeadRigBone().GetPosition();
		
		//if (NFGE::Math::Distance(headPosition, wallPosition )> 200.0f)
		rayToWall = Ray(headPosition + Vector3(0.0f,-80.0f,0.0f), NFGE::Math::Cross(NFGE::Math::Vector3::YAxis, mYBots[mCurrentControlIndex]->heading) + mYBots[mCurrentControlIndex]->heading * 0.1f);
		//else
		{
			float entryDistance = 0.0f;
			float exitDistance = 0.0f;
			if (NFGE::Math::Intersect(rayToWall, *mWallObb, entryDistance, exitDistance))
				mTouchPoints[0] = headPosition + Vector3(0.0f, -80.0f, 0.0f) + rayToWall.dir * entryDistance;
		}

		//if (NFGE::Math::Distance(headPosition, roofPosition)> 200.0f)
		rayToRoof = Ray(headPosition + rayToWall.dir * 50.0f, NFGE::Math::Vector3::YAxis + mYBots[mCurrentControlIndex]->heading * 0.05f);
		//else
		{
			float entryDistance_1 = 0.0f;
			float exitDistance_1 = 0.0f;
			if(NFGE::Math::Intersect(rayToRoof, *mRoofObb, entryDistance_1, exitDistance_1))
				mTouchPoints[1] = headPosition + rayToWall.dir * 50.0f + rayToRoof.dir * entryDistance_1;
		}

		

		
	}
	
}

void GameState::Render()
{
	BlendStateManager::Get()->GetBlendState("AlphaBlend")->Set();
	
	mPlane.Render(NFGE::Math::Matrix4::sIdentity(), NFGE::sApp.GetMainCamera());

	for (auto& ybot : mYBots)
	{
		ybot->Render(NFGE::sApp.GetMainCamera());
	}

	mCube_0.Render();
	mCube_1.Render();
	mCube_2.Render();
	mCube_wall.Render();
	mCube_roof.Render();
}

void GameState::DebugUI()
{
	//PostProcessManager::Get()->OptionUI();

	mYBots[0]->DebugUI();
	//mYBots[0]->GetController()->DebugUI();
	if (mCurrentControlIndex != -1)
		mYBots[mCurrentControlIndex]->GetController()->DebugUI();

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

	ImGui::Begin("World");
	if (ImGui::Button("Show World debug Line"))
	{
		IsShowWorldDebug = !IsShowWorldDebug;
	}
	ImGui::End();

	if (IsShowWorldDebug)
	{
		mPhysicsWorld.DebugDraw();
	}
	

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

	SimpleDraw::AddSphere(NFGE::Math::Sphere(mTouchPoints[0], 1.0f), NFGE::Graphics::Colors::Orange);
	SimpleDraw::AddSphere(NFGE::Math::Sphere(mTouchPoints[1], 1.0f), NFGE::Graphics::Colors::BlueViolet);
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
	if (inputSystem->IsKeyDown(Input::KeyCode::FOUR))
		obbPlane = obbPlane_low_1;
	if (inputSystem->IsKeyDown(Input::KeyCode::FIVE))
		obbPlane = obbPlane_low_2;


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
