#include "GameState.h"

void GameState::Initialize()
{
	randomGenerateObs();
	for (int i = 0; i < 15; i++)
	{
		dudes.emplace_back(world);
	}


	for (int i = 0; i < dudes.size(); i++)
	{
		dudes[i].Initialize();
		dudes[i].steeringModule->GetBehavior < AI::OffsetPursuitBehavior<AI::TriangleTeamFormContext>>("OffsetPursuit")->SetActive(true);
		dudes[i].steeringModule->GetBehavior < AI::OffsetPursuitBehavior<AI::TriangleTeamFormContext>>("OffsetPursuit")->leader = &dudes[0];
		dudes[i].steeringModule->GetBehavior < AI::OffsetPursuitBehavior<AI::TriangleTeamFormContext>>("OffsetPursuit")->agentIndex = i;
		dudes[i].steeringModule->GetBehavior < AI::OffsetPursuitBehavior<AI::TriangleTeamFormContext>>("OffsetPursuit")->arriveDistance = 500.0f;
		if (i == 0)
		{
			dudes[i].steeringModule->GetBehavior < AI::OffsetPursuitBehavior<AI::TriangleTeamFormContext>>("OffsetPursuit")->SetActive(false);
			dudes[i].steeringModule->GetBehavior < AI::OffsetPursuitBehavior<AI::TriangleTeamFormContext>>("OffsetPursuit")->agentIndex = 0;
		}
	}

	dudes[0].steeringModule->GetBehavior<AI::ArriveBehavior>("Arrive")->SetActive(true);
	dudes[0].steeringModule->GetBehavior < AI::ObstacleAvoidBehavior >("ObstacleAvoid")->SetActive(true);
	dudes[0].position = { 100.0f, 100.0f };

	dudes[1].steeringModule->GetBehavior<AI::PursuitBehavior>("Pursuit")->targetAgent = &dudes[0];
	dudes[1].steeringModule->GetBehavior<AI::PursuitBehavior>("Pursuit")->predictFactor = 0.5f;
	dudes[1].steeringModule->GetBehavior<AI::PursuitBehavior>("Pursuit")->SetActive(false);
	dudes[1].steeringModule->GetBehavior<AI::EvadeBehavior>("Evade")->targetAgent = &dudes[0];
	dudes[1].steeringModule->GetBehavior<AI::EvadeBehavior>("Evade")->SetActive(false);
	dudes[1].steeringModule->GetBehavior < AI::ObstacleAvoidBehavior >("ObstacleAvoid")->SetActive(true);
	dudes[1].position = { 500.0f, 500.0f };
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
	GraphicsSystem::Get()->SetClearColor(NFGE::Graphics::Colors::DarkSlateGray);

	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		NFGEApp::ShutDown();
		return;
	}

	if (NFGE::Input::InputSystem::Get()->IsMousePressed(NFGE::Input::MouseButton::LBUTTON))
	{
		dudes[0].destination.x = static_cast<float>(NFGE::Input::InputSystem::Get()->GetMouseScreenX());
		dudes[0].destination.y = static_cast<float>(NFGE::Input::InputSystem::Get()->GetMouseScreenY());
	}

	for (int i = 0; i < dudes.size(); i++)
	{
		dudes[i].Update(deltaTime);
	}

	
}

void GameState::Render()
{
	for (int i = 0; i < dudes.size(); i++)
	{
		dudes[i].Render();
	}
	auto& obs = world.GetObstacles();
	for (int i = 0; i < obs.size(); i++)
	{
		NFGE::sApp.DrawScreenCircle(obs[i].center, obs[i].radius, NFGE::Graphics::Colors::Azure);
	}
}

void GameState::DebugUI()
{
	
}

