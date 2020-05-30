#include "Base.h"
#include "ObjectPoolManager.h"
#include "LevelManager.h"
#include "Commander.h"
#include "PCLWorker.h"
#include "Carrier.h"

using namespace LCUnit;

namespace 
{
	const int totalBase = 2;

	const float maxHealths[totalBase] = { 1000.0f, 1000.0f };
	const int sizeCols[totalBase] = { 4,4 };
	const int sizeRows[totalBase] = { 2,2 };
	std::string defaultTexture[totalBase] = { "LaCelleCraft/PeterChanLegend/Building/PCL_base.png","LaCelleCraft/PeterChanLegend/Building/PCL_base.png" };
	const int capacity[totalBase] = { 0,0 };
	const int healthSlice[totalBase] = { 10,10 };
	std::string names[totalBase] = { "Peter Chan Base", "LaCelle Center" };
	const float buildSpeed[totalBase] = { 10.0f,10.0f};
	const int factoryObjCount = 2;
	enum class BuildAbleObject
	{
		Worker = 0,
		Carrier
	};
	std::string factorys[factoryObjCount]	//TODO add factory for different faction
	{
		"PCLWorker",
		"Carrier"
	};

	std::string texturePrefix[totalBase][Building::TotalStates] = 
	{
		{
			"LaCelleCraft/PeterChanLegend/Building/Base/Idle/PCL_base_idle_",
			"LaCelleCraft/PeterChanLegend/Building/Base/InBuilding/PCL_base_inBuilding_",
			"LaCelleCraft/PeterChanLegend/Building/Base/InBuildingAlmostDestoryed/PCL_base_inBuildingAlmostDestoryed_",
			"LaCelleCraft/PeterChanLegend/Building/Base/InBuildDestoryed/PCL_base_inBuildDestoryed_",
			"LaCelleCraft/PeterChanLegend/Building/Base/GetAttack/PCL_base_getAttack_",
			"LaCelleCraft/PeterChanLegend/Building/Base/AlmostDestoryed/PCL_base_almostDestoryed_",
			"LaCelleCraft/PeterChanLegend/Building/Base/Destoryed/PCL_base_destoryed_",
			"LaCelleCraft/PeterChanLegend/Building/Base/FinishBuild/PCL_base_finishBuild_",
		},
		{ 
			"LaCelleCraft/StudentUnion/Building/Base/Idle/SU_base_idle_",
			"LaCelleCraft/StudentUnion/Building/Base/InBuilding/SU_base_inBuilding_",
			"LaCelleCraft/StudentUnion/Building/Base/InBuildingAlmostDestoryed/SU_base_inBuildingAlmostDestoryed_",
			"LaCelleCraft/StudentUnion/Building/Base/InBuildDestoryed/SU_base_inBuildDestoryed_",
			"LaCelleCraft/StudentUnion/Building/Base/GetAttack/SU_base_getAttack_",
			"LaCelleCraft/StudentUnion/Building/Base/AlmostDestoryed/SU_base_almostDestoryed_",
			"LaCelleCraft/StudentUnion/Building/Base/Destoryed/SU_base_destoryed_",
			"LaCelleCraft/StudentUnion/Building/Base/Destoryed/SU_base_finishBuild_",
		}
	};
	std::string textureSubFix[totalBase] = { ".png",		".png" };
	const int frameCount[totalBase][Building::TotalStates] = 
	{ 
		{ 36, 12 ,12, 12,6,6,12,12 },
		{ 12, 12, 12, 12,6,6,12,12 }
	};
	const bool isLoop[totalBase][Building::TotalStates] =
	{
		{true, true, false, false, false, true, false, false},
		{ true, true, false, false, false, true, false, false},
	};
	const int toState[totalBase][Building::TotalStates] =
	{
		{ -1, -1, BuildingState::InBuilding, BuildingState::InBuilding, BuildingState::Idle, -1, BuildingState::Idle, BuildingState::Idle},
		{ -1, -1, BuildingState::InBuilding, BuildingState::InBuilding, BuildingState::Idle, -1, BuildingState::Idle, BuildingState::Idle},
	};

}

void Base::Load(TextureGridMap& worldMap, int CommandorIndex, const Vector2& initPos, BuildingRace race)
{
	Building::Load(worldMap, initPos, maxHealths[race], sizeCols[race], sizeRows[race], defaultTexture[race], capacity[race], healthSlice[race]);
	if (mIsActive == false)
		return;

	for (int i = 0; i < factoryObjCount; i++)
	{
		auto textureId = ObjectPoolManager::Get()->GetUnitFromPool(factorys[i])->GetDefaultTexture();
		mFactory.push_back(BuildFactoryObject{ factorys[i],textureId });
	}
	mCommanderIndex = CommandorIndex;
	mBuildSpeed = buildSpeed[race];
	mProcessBar.Load();

	for (int i = 0; i < Building::TotalStates; i++)
	{
		mAnimator.AddClip(LCAnimationClip(texturePrefix[race][i], textureSubFix[race], isLoop[race][i], frameCount[race][i], 1, toState[race][i]));
	}

	mIsResourceCollectable = true;
}

void Base::EditorLoad(TextureGridMap & worldMap, const Vector2 & initPos)
{
	Building::Load(worldMap, initPos, maxHealths[0], sizeCols[0], sizeRows[0], defaultTexture[0], capacity[0], healthSlice[0]);
	RemoveFromMap();
	if (mIsActive == false)
		return;

	for (int i = 0; i < factoryObjCount; i++)
	{
		auto textureId = ObjectPoolManager::Get()->GetUnitFromPool(factorys[i])->GetDefaultTexture();
		mFactory.push_back(BuildFactoryObject{ factorys[i],textureId });
	}
	mCommanderIndex = -1;
	mProcessBar.Load();

	for (int i = 0; i < Building::TotalStates; i++)
	{
		mAnimator.AddClip(LCAnimationClip(texturePrefix[0][i], textureSubFix[0], isLoop[0][i], frameCount[0][i], 1, toState[0][i]));
	}
}

void Base::UnLoad()
{
}

void Base::Interact()
{
	ImGui::Begin(mName.c_str());
	ImGui::Image(GetSprite(mDefaultTexture), { 100.0f,100.0f });
	ImGui::ProgressBar(mCurrentHealth / mMaxHealth);
	ImGui::Text("%f/%f", mCurrentHealth, mMaxHealth);

	ImGui::BeginChild("Producing", { 100.0f, 120.0f + static_cast<float>(mProducingQueue.size()) * (mProducingQueueTextureSize.y + 5.0f) }, true);

	std::list<Unit*>::iterator it = mProducingQueue.begin();
	for (; it != mProducingQueue.end(); ++it)
	{
		if (ImGui::ImageButton(GetSprite((*it)->GetDefaultTexture()), { mProducingQueueTextureSize.x, mProducingQueueTextureSize.y }))
		{
			break;
		}
		if (it == mProducingQueue.begin())
		{
			ImGui::ProgressBar((*it)->mCurrentHealth / (*it)->mMaxHealth);
			ImGui::Text("%f/%f", (*it)->mCurrentHealth, (*it)->mMaxHealth);
		}
	}
	if (it != mProducingQueue.end()) // remove the one we click
	{
		(*it)->Die();
		
		mProducingQueue.erase(it);
	}

	ImGui::EndChild();

	ImGui::BeginChild("Build", { 100.0f, 120.0f + static_cast<float>(mFactory.size()) * (mProducingQueueTextureSize.y + 5.0f) }, true);
	for (int i = 0; i < mFactory.size(); ++i)
	{
		auto& unit = mFactory[i];
		if (ImGui::ImageButton(GetSprite(unit.texture), { mFactoryObjTextureSize.x, mFactoryObjTextureSize.y }))
		{
			BuildUnit(i);
		}

	}
	ImGui::EndChild();
	ImGui::End();
}

void Base::Attack( Unit & unit, float deltaTime)
{
}

void Base::Attack( Building & strcuture, float deltaTime)
{
}

void Base::TakeDamage(LC::Projectile & bullet)
{
	mCurrentHealth -= bullet.mDmg;
	//if (mCurrentHealth <= 0.0f)				//--- Update function already check the death
	//	Die();
}

void Base::Die()
{
	//TODO --- stop some thing like buiding unit & tigger game over for current command if it is last building of that commender

	Building::Die();
}

void Base::BuildUnit(int index)
{
	BuildAbleObject whichToBuild = static_cast<BuildAbleObject>(index);
	Unit* newUnit = ObjectPoolManager::Get()->GetUnitFromPool(mFactory[index].TypeName);
	switch (whichToBuild)
	{
	case BuildAbleObject::Worker:
	{
		PCLWorker* newWorker = reinterpret_cast<PCLWorker*>(newUnit);
		newWorker->mIsBuilding = true;
		newWorker->Load(*mWorldMap, mWorldMap->gridGraph.GetNode(mSpwanTile).position, mCommanderIndex);
		newUnit = newWorker;
	}
		break;
	case BuildAbleObject::Carrier:
	{
		Carrier* newCarrier = reinterpret_cast<Carrier*>(newUnit);
		newCarrier->mIsBuilding = true;
		newCarrier->Load(*mWorldMap, mWorldMap->gridGraph.GetNode(mSpwanTile).position, mCommanderIndex);
		newUnit = newCarrier;
	}
		break;
	default:
		break;
	}
	
	mProducingQueue.push_back(newUnit);

}

void Base::BuildingUnit(Unit * unit, float deltaTime)
{
	unit->mCurrentHealth += mBuildSpeed * deltaTime;
	mProcessBar.Update(unit->mCurrentHealth / unit->mMaxHealth);
	if (unit->mCurrentHealth >= unit->mMaxHealth)
	{
		unit->mCurrentHealth = unit->mMaxHealth;
		unit->mIsBuilding = false;
		LevelManager::Get()->mCommanders[mCommanderIndex].RegisterNewUnit(unit);
		//LevelManager::Get()->RegisterNewUnit(unit);
		mProducingQueue.pop_front();
	}
}
