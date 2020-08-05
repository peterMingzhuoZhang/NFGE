#include "GameSettingManager.h"

namespace
{
	std::unique_ptr<GameSettingManager> sGameSettingManager;
}

void GameSettingManager::StaticInitialize()
{
	ASSERT(sGameSettingManager == nullptr, "[GameSettingManager] alrerady initialized!");
	sGameSettingManager = std::make_unique<GameSettingManager>();
	sGameSettingManager->Initlialize();
}

void GameSettingManager::StaticTerminate()
{
	if (sGameSettingManager != nullptr)
	{
		sGameSettingManager->Terminate();
		sGameSettingManager.reset();
	}
}

GameSettingManager * GameSettingManager::Get()
{
	ASSERT(sGameSettingManager != nullptr, "[GameSettingManager] not initialized!");
	return sGameSettingManager.get();
}

GameSettingManager::~GameSettingManager()
{
	ASSERT(sGameSettingManager == nullptr, "[GameSettingManager] Terminate must be called to clean up!");
}
