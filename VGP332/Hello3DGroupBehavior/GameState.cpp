#include "GameState.h"

void GameState::Initialize()
{
	world_3D.Initialize(mWorldWidth, mWorldHeight);

	for (int i = 0; i < totalDude; i++)
	{
		dudes.push_back(new SphereDude(world_3D.world));
	}

	world_3D.world.mGrid.Init(col, row, size, (int)dudes.size());

	for (int i = 0; i < dudes.size(); i++)
	{
		dudes[i]->Initialize(mWorldWidth, mWorldHeight);
		dudes[i]->steeringModule->GetBehavior< AI::WanderBehavior >("Wander")->SetActive(true);
		dudes[i]->steeringModule->GetBehavior< AI::GroupSeparationBehavior>("GroupSeparation")->SetActive(true);
		dudes[i]->steeringModule->GetBehavior< AI::GroupCohesionBehavior>("GroupCohesion")->SetActive(true);
		dudes[i]->steeringModule->GetBehavior< AI::GroupAlignmentBehavior>("GroupAlignment")->SetActive(true);
		dudes[i]->steeringModule->GetBehavior< AI::ObstacleAvoidBehavior>("ObsAvoid")->SetActive(true);
	}
	
	randomGenerateObs();

	auto& camera = NFGE::sApp.GetMainCamera();
	camera.SetPosition({-50.0f,200.0f, -200.0f});
	camera.SetDirection({ 0.6f,-0.3f, 0.7f});
	camera.SetFarPlane(10000.0f);
}

void GameState::Terminate()
{
	SphereDude::mSphere.Terminate();
	world_3D.Terminate();
	for (int i = 0; i < totalDude; i++)
	{
		delete dudes[i];
	}
	
}

void GameState::Update(float deltaTime)
{
	world_3D.world.Update();

	for (int i = 0; i < dudes.size(); i++)
	{
		dudes[i]->Update(behaviorControl, deltaTime);
	}

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
	world_3D.Render();
	for (int i = 0; i < dudes.size(); i++)
	{
		dudes[i]->mIsDebugDraw = isDrawDebugLine;
		dudes[i]->Render();
	}

	////---------------------------------------------------------------------------------------------------------------------------
	//NFGE::sApp.DrawScreenCircle(dudes[0]->position, dudes[0]->neighbourRange, NFGE::Graphics::Colors::LightPink);

	//for (auto& dude1Neighbor : dudes[0]->neighborhood)
	//{
	//	NFGE::sApp.DrawScreenCircle(dude1Neighbor->position, 10.0f, NFGE::Graphics::Colors::HotPink);
	//}
	////---------------------------------------------------------------------------------------------------------------------------

	auto& obs = world_3D.world.GetObstacles();
	for (int i = 0; i < obs.size(); i++)
	{
		SimpleDraw::AddCircle_FaceY(obs[i].center.x, 1.0f, obs[i].center.y, obs[i].radius, Colors::Azure);
	}
	
	NFGE::Graphics::SimpleDraw::Render(NFGE::sApp.GetMainCamera());
}

void GameState::DebugUI()
{
	ShowUI();
}

void GameState::randomGenerateObs()
{
	int howManyObs = NFGE::Math::Random() % 30;
	for (int i = 0; i < howManyObs; i++)
	{
		float randomX = NFGE::Math::RandomFloat(0.0f, (float)NFGE::sApp.GetScreenWidth());
		float randomY = NFGE::Math::RandomFloat(0.0f, (float)NFGE::sApp.GetScreenHeight());
		float randomR = NFGE::Math::RandomFloat(20.0f, 50.0f);
		world_3D.world.AddObstacles(NFGE::Math::Circle(randomX, randomY, randomR));
	}
}

void GameState::ShowUI()
{
	ImGui::Begin("Behavior Control");
	ImGui::SliderFloat("Wander weight:", &(behaviorControl.mWanderWeight), 0.0f, 10.0f);
	ImGui::SliderFloat("Group Alignment weight:", &(behaviorControl.mAlignmentWeight), 0.0f, 10.0f);
	ImGui::SliderFloat("Group Separation weight:", &(behaviorControl.mSeparationWeight), 0.0f, 10.0f);
	ImGui::SliderFloat("Group Cohesion weight:", &(behaviorControl.mCohesionWeight), 0.0f, 10.0f);
	ImGui::SliderFloat("Group ObsAvoid weight:", &(behaviorControl.mObsAvoidWeight), 0.0f, 10.0f);
	ImGui::SliderFloat("Group Neighbor radius:", &(behaviorControl.mNeighborRadius), 0.0f, 500.0f);
	ImGui::End();

	ImGui::Begin("ShortCut");
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
	ImGui::End();

	ImGui::Begin("Eyes Blind Alert!");
	if (ImGui::Button("Toggle Debug Line"))
	{
		isDrawDebugLine = isDrawDebugLine ? false : true;
	}
	ImGui::End();

	auto& cameraDir = NFGE::sApp.GetMainCamera().GetDirection();
	auto& cameraPos = NFGE::sApp.GetMainCamera().GetPosition();
	ImGui::Text("Camera Pos, %f, %f, %f", cameraPos.x, cameraPos.y, cameraPos.z);
	ImGui::Text("Camera Dir, %f, %f, %f", cameraDir.x, cameraDir.y, cameraDir.z); 
}

void GameState::CameraControl(Camera& camera)
{
	static auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
	lastTime = currentTime;

	const float moveSpeed = 50.0f;
	const float turnSpeed = 2.0f;
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