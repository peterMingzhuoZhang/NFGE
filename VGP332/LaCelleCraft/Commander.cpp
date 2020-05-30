#include "Commander.h"
#include "ObjectPoolManager.h"


void Commander::Load(int zhenYing,int Race, BattleMap* theMap, const Vector2 & initPos, bool IsLocalControl, bool IsAIControl, int index)
{
	mWorldMap = theMap;
	mZhenYing = zhenYing;
	mIsLocalControl = IsLocalControl;
	mIsAIControl = IsAIControl;
	stateMachine = std::make_unique<AI::StateMachine<Commander>>(*this);

	auto newBase = static_cast<Base*>(ObjectPoolManager::Get()->GetBuildingFromPool("Base"));

	newBase->Load((*theMap), index, initPos, static_cast<BuildingRace>(Race));
	ASSERT(newBase->mIsActive != false, "[Commander] Initialize commander with invalide base position");

	this->RegisterNewBuilding(newBase);
}
