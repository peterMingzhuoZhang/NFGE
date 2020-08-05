#ifndef LACELLCRAFT_GAMEOBJECT_RESOURCE_HEADER
#define LACELLCRAFT_GAMEOBJECT_RESOURCE_HEADER

#include "Structure.h"

enum ResourceType
{
	Money = 0,
	Matrix
};


enum ResourceState
{
	Sufficient = 0,
	AlmostGone,
	Gone
};

namespace LCUnit 
{
	struct Worker;
}
struct Resource : Structure
{
	ResourceType mResourceType;
	ResourceState mState;
	int mTotalAmount;
	int m25PercentAmount;
	int mCurrentAmount;
	std::list<LCUnit::Worker*> mWorkers;

	void Load(TextureGridMap& worldMap, const Vector2& initPos, ResourceType whichType, int amount);
	
	void Update(float deltaTime) override
	{
		Structure::Update(deltaTime);
		
		if (!mIsActive)
			return;

		if ((mCurrentAmount < m25PercentAmount) && (mCurrentAmount > 0))
		{
			mState = ResourceState::AlmostGone;
		}
		else if (mCurrentAmount == 0)
		{
			mState = ResourceState::Gone;
		}

		UpdateWorkers(deltaTime);
	}

	void Render(const Camera2D& camera) override
	{
		Structure::Render(camera);
		if (!mIsActive)
			return;
	}

	void DebugUIUpdate(float deltaTime, const Camera2D& camera) override
	{
		Structure::DebugUIUpdate(deltaTime, camera);
		if (!mIsActive)
			return;
	}
	void Unload()
	{
		Structure::Unload();
	}

	void Interact() override
	{

	}

private:
	void UpdateWorkers(float deltaTime);
};

#endif // !LACELLCRAFT_GAMEOBJECT_RESOURCE_HEADER
