#ifndef LACELLECRAFT_GAMEOBJECT_BUILDING_BASE_HEADER
#define LACELLECRAFT_GAMEOBJECT_BUILDING_BASE_HEADER

#include "Building.h"
#include "Projectile.h"

struct Commander;

struct Base : Building
{
	RTTI(Base)
	std::string mName = "Base";
	std::vector<BuildFactoryObject> mFactory;
	Vector2 mFactoryObjTextureSize{ 64.0f,64.0f };
	std::list<Unit*> mProducingQueue;
	Vector2 mProducingQueueTextureSize{64.0f,64.0f};
	float mBuildSpeed;

	ProcessBar mProcessBar;

	void Load(TextureGridMap& worldMap, int CommandorIndex, const Vector2& initPos, BuildingRace race);
	void EditorLoad(TextureGridMap& worldMap, const Vector2& initPos);
	void UnLoad();
	void Update(float deltaTime) override
	{
		if (!mIsActive)
			return;
		Building::Update(deltaTime);

		if (!mIsInBuild)
		{
			//Interact();
			if (!mProducingQueue.empty())
			{
				mProcessBar.Update(mProducingQueue.front()->mCurrentHealth / mProducingQueue.front()->mMaxHealth);
				BuildingUnit(mProducingQueue.front(), deltaTime);
			}
		}
	}
	void Render(const Camera2D& camera) override
	{
		if (!mIsActive)
			return;
		Building::Render(camera);

		if (!mProducingQueue.empty())
		{
			float processBarLength = mDefaultWidth * mDrawScale * camera.GetScale();
			mProcessBar.Render(camera.ConvertToScreen(mPosition + mTextureRenderOffSet), processBarLength, camera.GetScale());
		}
	}

	void DebugUIUpdate(float deltaTime, const Camera2D& camera)
	{
		Building::DebugUIUpdate(deltaTime, camera);
	}

	void Interact() override;

	void Attack( Unit& unit, float deltaTime) override;
	void Attack( Building& strcuture, float deltaTime) override;
	void TakeDamage(LC::Projectile& bullet) override;
	void Die()override;
private:

	void BuildUnit(int index) override;

	void BuildingUnit(Unit* unit, float deltaTime);

	void Destory() override
	{
		if (mIsDying)
			return;

		mIsDying = true;
		float delay;
		if ((mAnimator.mState == BuildingState::InBuilding) || (mAnimator.mState == BuildingState::InBuildingAlmostDestoryed))
		{
			mAnimator.ChangeToState(static_cast<int>(BuildingState::InBuildDestoryed), false);
			delay = mAnimator.mAllClips[static_cast<int>(BuildingState::InBuildDestoryed)].mTotalFrame * mAnimator.mFrameTime;
		}
		else
		{
			mAnimator.ChangeToState(static_cast<int>(BuildingState::Destoryed), false);
			delay = mAnimator.mAllClips[static_cast<int>(BuildingState::Destoryed)].mTotalFrame * mAnimator.mFrameTime;
		}
		mBoolAnimator.Load(&mIsFinishDying, true, delay);
		
	}
};

#endif // !LACELLECRAFT_GAMEOBJECT_BUILDING_BASE_HEADER
