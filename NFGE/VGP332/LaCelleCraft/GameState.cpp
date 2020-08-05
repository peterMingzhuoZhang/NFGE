#include "GameState.h"


using namespace NFGE::Input;

Camera2D myCamera2D;
extern BattleMap myBattleMap;
std::string MapName = "mytilemap_10.txt";
int Commanders_ZhenYing[8] = { 0,1,1,1,1,1,1,1 };
int Commanders_Race[8] = { 0,0,0,1,1,1,1,1 };

void GameState::Initialize()
{
	GameSettingManager::StaticInitialize();
	ObjectPoolManager::StaticInitialize();
	LevelManager::StaticInitialize();

	myCamera2D = Camera2D(static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenWidth()));

}

void GameState::Terminate()
{
	myBattleMap.Unload();
	LevelManager::StaticTerminate();
	ObjectPoolManager::StaticTerminate();
	GameSettingManager::StaticTerminate();
}

void GameState::Update(float deltaTime)
{
	mDeltaTime = deltaTime;

	auto inputSystem = NFGE::Input::InputSystem::Get();

	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		NFGEApp::ShutDown();
		return;
	}

	if (!isDebugMode)
		LevelManager::Get()->Update(deltaTime);
	myCamera2D.Update(deltaTime);
}

void GameState::Render()
{
	LevelManager::Get()->Render(myCamera2D);
	LevelManager::Get()->UIUpdate(mDeltaTime, myCamera2D);
	//Debug Update & Render
	if (NFGE::Input::InputSystem::Get()->IsKeyPressed(KeyCode::P))
		isDebugMode = !isDebugMode;
	if (isDebugMode)
	{
		LevelManager::Get()->DebugUIUpdate(mDeltaTime, myCamera2D);
		myCamera2D.DebugUIUpdate(mDeltaTime);
	}
	
}

void GameState::DebugUI()
{
	

	// LateUpdate()
	LevelManager::Get()->LateUpdate(mDeltaTime);
}

