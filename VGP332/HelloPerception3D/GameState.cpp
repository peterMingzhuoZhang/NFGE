#include "GameState.h"

BehaviorControl behaviorControl;
bool isDrawDebugLine = false;

int initTinyFishCOunt = 10;
const int totalTinyFIsh = 1000;
int currentTinyFish = 0;

int initPreditor = 10;
const int totalPreditor = 10;

const float worldSizeMutiplyer = 0.5f;

const float cameraMoveSpeed = 500.0f;
const float cameraTurnSpeed = 20.0f;

void GameState::Initialize()
{
	size *= worldSizeMutiplyer;
	mWorldWidth = (static_cast<float>(col) * size);
	mWorldHeight = (static_cast<float>(height) * size);
	mWorldDepth = (static_cast<float>(row) * size);
	mTreeCount *= static_cast<int>(worldSizeMutiplyer);
	if (mTreeCount < 1) mTreeCount = 3;

	world_3D.Initialize(mWorldWidth, mWorldHeight, mWorldDepth);
	world_3D.world.mGrid.Init(col, row,height, size, totalTinyFIsh);

	//- SeeWeedTree INIT -------------------------------------------------------------------------------------------
	for (int i = 0; i < mTreeCount; ++i)
	{
		mTrees.push_back(SeeWeedTree());
		mTrees.back().Initialize(0.5f,world_3D.world, mWorldWidth, mWorldHeight, mWorldDepth);
	}
	//-----------------------------------------------------------------------------------------------------------

	//- TinyFish INIT -------------------------------------------------------------------------------------------
	PTEntityManager<TinyFish>::StaticInitialize(totalTinyFIsh, world_3D.world);
	for (int i = 0; i < totalTinyFIsh; ++i)
	{
		auto tinyFishPtr = PTEntityManager<TinyFish>::Get()->GetNext();
		tinyFishPtr->Initialize(mWorldWidth,mWorldHeight, mWorldDepth);
		world_3D.world.RegisterEntity3D<TinyFish>(tinyFishPtr);
	}
	for (int i = 0; i < initTinyFishCOunt; ++i)
	{
		auto tinyFishPtr = PTEntityManager<TinyFish>::Get()->GetNext();
		tinyFishPtr->mIsActive = true;
		++currentTinyFish;
	}
	//-----------------------------------------------------------------------------------------------------------

	//- PreditorFish INIT -------------------------------------------------------------------------------------------
	std::vector<PreditorFish*> temp;
	PTEntityManager<PreditorFish>::StaticInitialize(initPreditor, world_3D.world);
	for (int i = 0; i < totalPreditor; ++i)
	{
		auto preditorPtr = PTEntityManager<PreditorFish>::Get()->GetNext();
		preditorPtr->Initialize(mWorldWidth, mWorldHeight, mWorldDepth);
		world_3D.world.RegisterEntity3D<PreditorFish>(preditorPtr);
	}
	for (int i = 0; i < initPreditor; ++i)
	{
		auto preditorFishPtr = PTEntityManager<PreditorFish>::Get()->GetNext();
		preditorFishPtr->mIsActive = true;
		temp.push_back(preditorFishPtr);
	}
	temp[0]->steeringModule->GetBehavior < AI::WanderBehavior3D >("Wander")->SetActive(true);

	for (int i = 1; i < initPreditor; ++i)
	{
		auto beh = temp[i]->steeringModule->GetBehavior<AI::OffsetPursuitBehavior3D<AI::ConeTeamFormContext>>("OffsetPursuit");
		beh->leader = (temp[0]);
		beh->agentIndex = i;
		beh->SetActive(true);
	}
	//-----------------------------------------------------------------------------------------------------------

	randomGenerateObs();

	auto& camera = NFGE::sApp.GetMainCamera();
	camera.SetPosition({-50.0f,200.0f, -200.0f});
	camera.SetDirection({ 0.6f,-0.3f, 0.7f});
	camera.SetFarPlane(10000.0f);
}

void GameState::Terminate()
{
	SeeWeed::mSphere.Terminate();
	TinyFish::mGeometry.Terminate();
	PreditorFish::mGeometry.Terminate();
	DestroyerFish::mSphere.Terminate();
	
	world_3D.Terminate();
	for (int i = 0; i < mTreeCount; ++i)
		mTrees[i].Terminate();
	
	PTEntityManager<TinyFish>::StaticTerminate();
	PTEntityManager<PreditorFish>::StaticTerminate();
	
}

void GameState::Update(float deltaTime)
{
	mDeltaTime = deltaTime;

	world_3D.Update(deltaTime);

	auto inputSystem = InputSystem::Get();

	for (int i = 0; i < mTreeCount; ++i)
		mTrees[i].Update(deltaTime);

	SeeWeed::mSphere.Update(deltaTime);
	TinyFish::mGeometry.Update(deltaTime);
	PreditorFish::mGeometry.Update(deltaTime);
	DestroyerFish::mSphere.Update(deltaTime);
	
	PTEntityManager<TinyFish>::Get()->Update(deltaTime);
	PTEntityManager<PreditorFish>::Get()->Update(deltaTime);

	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		NFGEApp::ShutDown();
		return;
	}

	CameraControl(NFGE::sApp.GetMainCamera());
}

void GameState::Render()
{

	for (int i = 0; i < mTreeCount; ++i)
		mTrees[i].Render();

	PTEntityManager<TinyFish>::Get()->Render();
	PTEntityManager<PreditorFish>::Get()->Render();

	////---------------------------------------------------------------------------------------------------------------------------
	//NFGE::sApp.DrawScreenCircle(dudes[0]->position, dudes[0]->neighbourRange, NFGE::Graphics::Colors::LightPink);

	//for (auto& dude1Neighbor : dudes[0]->neighborhood)
	//{
	//	NFGE::sApp.DrawScreenCircle(dude1Neighbor->position, 10.0f, NFGE::Graphics::Colors::HotPink);
	//}
	////---------------------------------------------------------------------------------------------------------------------------

	auto& obs = world_3D.world.GetObstacle3Ds(); 
	for (int i = 0; i < obs.size(); i++)
	{
		SimpleDraw::AddSphere(obs[i].center.x, obs[i].center.y, obs[i].center.z, obs[i].radius, Colors::Azure);
	}
	
	NFGE::Graphics::SimpleDraw::Render(NFGE::sApp.GetMainCamera());
	world_3D.Render();
}

void GameState::DebugUI()
{
	ImGui::Begin("Util");
	ImGui::Text("FPS: %f", 1.0f / mDeltaTime);
	ImGui::End();

	for (int i = 0; i < mTreeCount; ++i)
		mTrees[i].DebugDraw();

	PTEntityManager<TinyFish>::Get()->DebugDraw();
	PTEntityManager<PreditorFish>::Get()->DebugDraw();

	ShowUI();
}

void GameState::randomGenerateObs()
{
	int howManyObs = NFGE::Math::Random() % maxOfObs;
	for (int i = 0; i < howManyObs; i++)
	{
		float randomX = NFGE::Math::RandomFloat(0.0f, mWorldWidth);
		float randomY = NFGE::Math::RandomFloat(0.0f, mWorldHeight);
		float randomZ = NFGE::Math::RandomFloat(0.0f, mWorldDepth);
		float randomR = NFGE::Math::RandomFloat(20.0f, 50.0f);
		world_3D.world.AddObstacle3D(NFGE::Math::Sphere(randomX, randomY, randomZ, randomR));
	}
}

void GameState::ShowUI()
{
	ImGui::Begin("Eyes Blind Alert!");
	if (ImGui::Button("Toggle Debug Line"))
	{
		isDrawDebugLine = isDrawDebugLine ? false : true;
	}
	ImGui::End();

	ImGui::Begin("Ecosystem Info");
	ImGui::Text("Tiny Fish Count: %d", currentTinyFish);
	ImGui::End();

	TinyFishBehaviorControl_DebugUI();

	world_3D.DebugDraw();
	
}

void GameState::TinyFishBehaviorControl_DebugUI()
{
	

	ImGui::Begin("Behavior Control");
	ImGui::SliderFloat("Wander weight:", &(behaviorControl.mWanderWeight), 0.0f, 10.0f);
	ImGui::SliderFloat("Group Alignment weight:", &(behaviorControl.mAlignmentWeight), 0.0f, 10.0f);
	ImGui::SliderFloat("Group Separation weight:", &(behaviorControl.mSeparationWeight), 0.0f, 10.0f);
	ImGui::SliderFloat("Group Cohesion weight:", &(behaviorControl.mCohesionWeight), 0.0f, 10.0f);
	ImGui::SliderFloat("Group ObsAvoid weight:", &(behaviorControl.mObsAvoidWeight), 0.0f, 10.0f);
	ImGui::SliderFloat("Group Neighbor radius:", &(behaviorControl.mNeighborRadius), 0.0f, 500.0f);

	if (ImGui::Button("Spread Out!"))
	{
		behaviorControl.mWanderWeight = 2.0f;
		behaviorControl.mAlignmentWeight = 1.0f;
		behaviorControl.mSeparationWeight = 8.0f;
		behaviorControl.mCohesionWeight = 0.0f;
	}
	if (ImGui::Button("Group In!"))
	{
		behaviorControl.mWanderWeight = 2.0f;
		behaviorControl.mAlignmentWeight = 1.0f;
		behaviorControl.mSeparationWeight = 0.0f;
		behaviorControl.mCohesionWeight = 8.5f;
	}
	if (ImGui::Button("Free Roam!"))
	{
		behaviorControl.mWanderWeight = 4.5f;
		behaviorControl.mAlignmentWeight = 0.0f;
		behaviorControl.mSeparationWeight = 0.0f;
		behaviorControl.mCohesionWeight = 0.0f;
	}
	if (ImGui::Button("Fish Like!"))
	{
		behaviorControl.mWanderWeight = 3.5f;
		behaviorControl.mAlignmentWeight = 4.0f;
		behaviorControl.mSeparationWeight = 1.0f;
		behaviorControl.mCohesionWeight = 3.0f;
	}

	if (ImGui::Button("Set"))
	{
		for (auto& fish : PTEntityManager<TinyFish>::Get()->GetPool() )
		{
			fish->UpdateBehaviorControl(behaviorControl);
		}
	}
	ImGui::End();

	//TODO :: add an imgui section that shows the current select TinyFish's behavior control info(May be add to tiny fish's debug())
	
	
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

	if (inputSystem->IsKeyDown(KeyCode::T))
	{
		auto theFish = PTEntityManager<TinyFish>::Get()->GetFirstActive();
		if (theFish != nullptr)
		{
			Vector3 pos = { theFish->position.x, theFish->position.y , theFish->position.z};
			Vector3 head = { theFish->heading.x, theFish->heading.y, theFish->heading.z};
			NFGE::sApp.GetMainCamera().SetPosition(pos - head * 20.0f);
			NFGE::sApp.GetMainCamera().SetDirection(head);
		}
	}
	if (inputSystem->IsKeyDown(KeyCode::P))
	{
		auto theFish = PTEntityManager<PreditorFish>::Get()->GetFirstActive();
		if (theFish != nullptr)
		{
			Vector3 pos = { theFish->position.x, theFish->position.y, theFish->position.z};
			Vector3 head = { theFish->heading.x, theFish->heading.y, theFish->heading.z };
			NFGE::sApp.GetMainCamera().SetPosition(pos - head * 40.0f);
			NFGE::sApp.GetMainCamera().SetDirection(head);
		}
	}
}