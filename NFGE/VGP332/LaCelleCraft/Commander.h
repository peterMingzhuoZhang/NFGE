#ifndef LACELLECRAFT_GAMEENTITY_COMMANDER_HEADER
#define LACELLECRAFT_GAMEENTITY_COMMANDER_HEADER

#include "Unit.h"
#include "Base.h"
#include "BattleMap.h"

struct Commander
{
	int mZhenYing;
	int race;
	bool mIsLocalControl;
	bool mIsAIControl;
	int mIndex;
	int mCurrnetMoney;

	BattleMap* mWorldMap;

	std::list<Unit*> myUnit;
	std::list<Building*> myBuilding;

	std::unique_ptr<AI::StateMachine<Commander>> stateMachine;	// For AI Commander
	
	void Load(int zhenYing, int race, BattleMap* theMap,const Vector2& initPos, bool IsLocalControl, bool IsAIControl, int index);
	

	void UnLoad()
	{
		stateMachine.reset();
	}

	void Update(float deltaTime)
	{
		if (mIsAIControl)
		{
			stateMachine->Update(deltaTime);
		}
	}

	void Render(const Camera2D& camera)
	{
		//May Render some display info related to the individual Commander
	}

	void RegisterNewUnit(Unit* unit)
	{
		
		if (!myUnit.empty())
		{
			for (std::list<Unit*>::iterator it = myUnit.begin(); it != myUnit.end(); ++it)
			{
				if (!(*it)->mIsActive)
				{
					*it = unit;
					return;
				}
			}
		}
		myUnit.push_back(unit);
	}

	void RegisterNewBuilding(Building* building)
	{
		if (!myBuilding.empty())
		{
			for (std::list<Building*>::iterator it = myBuilding.begin(); it != myBuilding.end(); ++it)
			{
				if (!(*it)->mIsActive)
				{
					*it = building;
					return;
				}
			}
		}
		
		myBuilding.push_back(building);
	}
};
#endif // !LACELLECRAFT_GAMEENTITY_COMMANDER_HEADER
