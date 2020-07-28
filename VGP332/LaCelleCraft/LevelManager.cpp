#include "LevelManager.h"

extern BattleMap myBattleMap;
extern Camera2D myCamera2D;
extern std::string MapName;
extern int Commanders_ZhenYing[8];
extern int Commanders_Race[8];

namespace
{
	std::unique_ptr<LevelManager> sLevelManager;
}

void LevelManager::StaticInitialize()
{
	ASSERT(sLevelManager == nullptr, "[LevelManager] alrerady initialized!");
	sLevelManager = std::make_unique<LevelManager>();
	sLevelManager->Initlialize();
}

void LevelManager::StaticTerminate()
{
	if (sLevelManager != nullptr)
	{
		sLevelManager->Terminate();
		sLevelManager.reset();
	}
}

LevelManager * LevelManager::Get()
{
	ASSERT(sLevelManager != nullptr, "[LevelManager] not initialized!");
	return sLevelManager.get();
}

LevelManager::~LevelManager()
{
	ASSERT(sLevelManager == nullptr, "[LevelManager] Terminate must be called to clean up!");
}

void LevelManager::Initlialize()
{
	myBattleMapPtr = &myBattleMap;
	myBattleMapPtr->Load(MapName);

	mWorldCamera = &myCamera2D;

 	mInLevelBuilding = std::list<Building*>(); // Clear the building add by Map initilazation

	int commanderCount = static_cast<int>(myBattleMapPtr->mBases.size());
	ASSERT(commanderCount <= 8, "[BattleMap] has more than 8 commander in the map");
	for (int i = 0; i < commanderCount; i++)
	{
		mCommanders.push_back(Commander());
		if (i == 0)
		{
			mCommanders.back().Load(Commanders_ZhenYing[i], Commanders_Race[i], myBattleMapPtr, myBattleMapPtr->mBases[i].mPosition, true, false, 0);
			mLocalCommandersIndex = i;
		}
		else
		{
			mCommanders.back().Load(Commanders_ZhenYing[i], Commanders_Race[i], myBattleMapPtr, myBattleMapPtr->mBases[i].mPosition, false, false, i);
			mAICommanders.push_back(i);
		}
	}

	mPlayerController.Load(&myCamera2D);
}

void LevelManager::Terminate()
{
	for (std::list<Unit*>::iterator it = mInLevelUnit.begin(); it != mInLevelUnit.end(); ++it)
	{
		*it = nullptr;
	}

	for (std::list<Building*>::iterator it = mInLevelBuilding.begin(); it != mInLevelBuilding.end(); ++it)
	{
		*it = nullptr;
	}

	for (std::list<Projectile*>::iterator it = mInLevelProjectile.begin(); it != mInLevelProjectile.end(); ++it)
	{
		*it = nullptr;
	}

	for (std::list<Projectile*>::iterator it = mInLevelAOEProjectile.begin(); it != mInLevelAOEProjectile.end(); ++it)
	{
		*it = nullptr;
	}

	for (auto& commander : mCommanders)
	{
		commander.UnLoad();
	}
	
}

void LevelManager::Update(float deltaTime)
{
	mPlayerController.Update(deltaTime);

	for (auto& commander : mCommanders)
	{
		commander.Update(deltaTime);
	}

	myBattleMapPtr->Update(deltaTime);

	for (std::list<Unit*>::iterator it = mInLevelUnit.begin(); it != mInLevelUnit.end(); ++it)
	{
		if (*it != nullptr)
		(*it)->Update(deltaTime);
	}

	for (std::list<Building*>::iterator it = mInLevelBuilding.begin(); it != mInLevelBuilding.end(); ++it)
	{
		if (*it != nullptr)
		(*it)->Update(deltaTime);
	}

	for (std::list<Projectile*>::iterator it = mInLevelProjectile.begin(); it != mInLevelProjectile.end(); ++it)
	{
		if (*it != nullptr)
		(*it)->Update(deltaTime);
	}

	for (std::list<Projectile*>::iterator it = mInLevelAOEProjectile.begin(); it != mInLevelAOEProjectile.end(); ++it)
	{
		if (*it != nullptr)
		(*it)->Update(deltaTime);
	}
}

void LevelManager::Render(const Camera2D & camera)
{
	for (auto& commander : mCommanders)
	{
		commander.Render(camera);
	}

	myBattleMapPtr->Render(camera);
	
	for (std::list<Building*>::iterator it = mInLevelBuilding.begin(); it != mInLevelBuilding.end(); ++it)
	{
		if (*it != nullptr)
		(*it)->Render(camera);
	}

	for (std::list<Unit*>::iterator it = mInLevelUnit.begin(); it != mInLevelUnit.end(); ++it)
	{
		if (*it != nullptr)
		(*it)->Render(camera);
	}

	for (std::list<Projectile*>::iterator it = mInLevelProjectile.begin(); it != mInLevelProjectile.end(); ++it)
	{
		if(*it != nullptr)
			(*it)->Render(camera);
	}

	for (std::list<Projectile*>::iterator it = mInLevelAOEProjectile.begin(); it != mInLevelAOEProjectile.end(); ++it)
	{
		if (*it != nullptr)
			(*it)->Render(camera);
	}

	mPlayerController.Render(camera);
}

void LevelManager::UIUpdate(float deltaTime, const Camera2D & camera)
{
	mPlayerController.UIUpdate(deltaTime, camera);
}

void LevelManager::DebugUIUpdate(float deltaTime, const Camera2D & camera)
{
	myBattleMapPtr->DebugUIUpdate(deltaTime, camera);

	for (std::list<Unit*>::iterator it = mInLevelUnit.begin(); it != mInLevelUnit.end(); ++it)
	{
		if (*it != nullptr)
		(*it)->DebugUIUpdate(deltaTime, camera);
	}

	for (std::list<Building*>::iterator it = mInLevelBuilding.begin(); it != mInLevelBuilding.end(); ++it)
	{
		if (*it != nullptr)
		(*it)->DebugUIUpdate(deltaTime, camera);
	}

	//for (std::list<Projectile*>::iterator it = mInLevelProjectile.begin(); it != mInLevelProjectile.end(); ++it)
	//{
	//	if (*it != nullptr)
	//	(*it)->DebugUIUpdate(deltaTime, camera);																						//|--- Projectile do not have DebugUI for now
	//}
	//
	//for (std::list<Projectile*>::iterator it = mInLevelAOEProjectile.begin(); it != mInLevelAOEProjectile.end(); ++it)
	//{
	//	if (*it != nullptr)
	//	(*it)->DebugUIUpdate(deltaTime, camera);																						//|--- Projectile do not have DebugUI for now
	//}

	ImGui::Begin("LevelManager");
	ImGui::Text("Current Unit: %d", mInLevelUnit.size());
	ImGui::Text("Current Building: %d", mInLevelBuilding.size());
	ImGui::Text("Current Projectiles: %d", mInLevelProjectile.size());
	ImGui::Text("Current Aoe Projectiles: %d", mInLevelAOEProjectile.size());
	ImGui::End();
}

void LevelManager::LateUpdate(float deltaTime)
{
	mPlayerController.LateUpdate(deltaTime);
}
