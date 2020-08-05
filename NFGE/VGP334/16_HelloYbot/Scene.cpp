#include "Scene.h"
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
}

Scene::Scene()
	// Meta system initlize
	: mGOAllocator(10)
	, mGameObjectFactory(mGOAllocator)
{
}

void Scene::Load()
{
	mLight = &NFGE::sApp.GetMainLight();
	mLight->direction = NFGE::Math::Normalize({ 1.0f,-1.0f,0.0f });
	mLight->ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mLight->diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	mLight->specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mLightBuffer.Initialize();

	//mPlane.Load(floorShaderFileName, floorShaderFileName, 100.0f, 100.0f, floorSpecular, floorDiffuse, floorDisplacement, floorNormal);
	mPlane2.DefaultInitialize(mLight);

	mYBots.reserve(100);
	mYBots.emplace_back(std::make_unique<YBot>(mWorld3D, mPhysicsWorld));
	mYBots.emplace_back(std::make_unique<YBot>(mWorld3D, mPhysicsWorld));
	mYBots.emplace_back(std::make_unique<YBot>(mWorld3D, mPhysicsWorld));
	mYBots.emplace_back(std::make_unique<YBot>(mWorld3D, mPhysicsWorld));
	mYBots.emplace_back(std::make_unique<YBot>(mWorld3D, mPhysicsWorld));
	//for (size_t i = 0; i < 50; i++)
	//{
	//	mYBots.emplace_back(std::make_unique<YBot>(mWorld3D, mPhysicsWorld));
	//}
	
	mYBots[0]->Load(defaultIdleAnimation);
	mYBots[1]->Load(boxingIdleAnimation,Graphics::Colors::Pink);
	mYBots[1]->SetInitPosition(Vector3{ 200.0f, 0.0f, 500.0f });
	mYBots[1]->heading = Vector3{ -1.0f,0.0f,0.0f };
	mYBots[2]->Load(dopeIdleAnimation, Graphics::Colors::Orange);
	mYBots[2]->SetInitPosition(Vector3{ -200.0f, 0.0f, 500.0f });
	mYBots[2]->heading = Vector3{ 1.0f,0.0f,0.0f };
	mYBots[3]->Load(noFeelingIdleAnimation, Graphics::Colors::Green);
	mYBots[3]->SetInitPosition(Vector3{ 200.0f, 0.0f, 900.0f });
	mYBots[3]->heading = Vector3{ -1.0f,0.0f,0.0f };
	mYBots[4]->Load(powerIdleAnimation, Graphics::Colors::Red);
	mYBots[4]->SetInitPosition(Vector3{ -200.0f, 0.0f, 900.0f });
	mYBots[4]->heading = Vector3{ 1.0f,0.0f,0.0f };
	//for (size_t i = 5; i < mYBots.size(); i++)
	//{
	//	mYBots[i]->Load(defaultIdleAnimation);
	//}

	mPhysicsWorld.AddOBB(OBB{ { 0.0f,150.0f,200.0f },{ 100.0f,10.0f,100.0f },{ Math::QuaternionRotationAxis(Math::Vector3::XAxis, -0.5f) } });

	//mGameObject = mGameObjectFactory.Create("");
	//auto transformComponent = mGameObject->AddComponent<NFGE::TransformComponent>();
	//transformComponent->position = { 0.0f, 1.0f,-10.0f };
	//auto colliderComponent = mGameObject->AddComponent<NFGE::ColliderComponent>();
	//colliderComponent->center = Vector3::Zero();
	//colliderComponent->extend = { 50.0f, 20.0f, 3.0f };
	//mGameObject->Initialize();

}

void Scene::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
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
		lookTargets.push_back(target->GetHeadRigBone()->GetPosition());
	}
	for (auto& ybot : mYBots)
	{
		ybot->Looking(lookTargets);
	}

	// Meta system test
	//mGameObject->Update(deltaTime);
}

void Scene::Render(const Graphics::Camera & camera)
{
	mLightBuffer.Load(*mLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	//mPlane.Render(NFGE::Math::Matrix4::sIdentity(), camera);
	mPlane2.Render(NFGE::Math::Matrix4::sIdentity(), camera);

	for (auto& ybot : mYBots)
	{
		ybot->Render(camera);
	}

	// Meta system test
	//mGameObject->Render();
}

void Scene::DebugUI()
{
	if (mCurrentControlIndex != -1)
	{
		mYBots[mCurrentControlIndex]->DebugUI();
		mYBots[mCurrentControlIndex]->GetController()->DebugUI();
	}
	
	mYBots[0]->DebugUI();

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

	ImGui::Begin("Test rig");
	
	if(ImGui::Button("Test head rig"))
	{
		mYBots[0]->GetAnimator().Play(0);
	}

	ImGui::End();

	mPhysicsWorld.DebugDraw();

	// Meta system test
	//mGameObject->DebugUI();
}

void Scene::Unload()
{
	mLightBuffer.Terminate();
	for (auto& ybot : mYBots)
	{
		ybot->UnLoad();
	}

	// Meta system test
	//mGameObject->Terminate();
	//mGameObjectFactory.Destory(mGameObject);
}
