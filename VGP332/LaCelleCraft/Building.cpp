#include "Building.h"
#include "LevelManager.h"

void Building::Load(TextureGridMap & worldMap, const Vector2 & initPos, float maxHealth, int sizeCol, int sizeRow, std::string defaultTexture, int capacity, int healthBarSliceCount)
{
	mType = StructureType::Building;
	mState = BuildingState::Idle;
	mMaxHealth = maxHealth;
	mCurrentHealth = maxHealth;
	mIsInBuild = false;
	mIsDying = false;
	mIsFinishDying = false;

	Structure::Load
	(
		worldMap,
		initPos,
		sizeCol,
		sizeRow,
		defaultTexture,
		capacity
	);

	if (mIsActive == false)
		return;

	if (!mOccupiedTile.empty())
	{
		mSpwanTile = mOccupiedTile.front();
	}

	mHealthBar.Load(healthBarSliceCount);

	ASSERT(mInLevelRegistry == nullptr, "[Building] Try Load Building twice, NOT ALLOWED!");
	mInLevelRegistry = LevelManager::Get()->RegisterNewBuilding(this);
}
