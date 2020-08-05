#ifndef LACELLECRAFT_GAMEOBJECT_BUILDING_HEADER
#define LACELLECRAFT_GAMEOBJECT_BUILDING_HEADER

#include "Structure.h"
#include "ProcessBar.h"
#include "HealthBar.h"
#include "PTAnimation.h"
#include "Fightable.h"

using namespace LC;

enum BuildingRace
{
	PeterChanLegend = 0,
	StudentUnion,
};



enum BuildingState
{
	Idle = 0,
	InBuilding,
	InBuildingAlmostDestoryed,
	InBuildDestoryed,
	GetAttack,
	AlmostDestoryed,
	Destoryed,
	FinishingBuild
};

struct BuildFactoryObject
{
	std::string TypeName;
	TextureId texture;
};

struct Building : Structure, LC::Fightable
{
	static const int TotalStates = 8;

	BuildingState mState;
	float mMaxHealth;
	float mCurrentHealth;
	
	float mToAlmostGonePercentage = 0.25f;

	bool mIsInBuild;
	bool mIsDying;
	bool mIsResourceCollectable = false;
	PTAnimation mBoolAnimator;
	bool mIsFinishDying = false;

	HealthBar mHealthBar;

	int mSpwanTile = 0;
	
	int mCommanderIndex;

	Building** mInLevelRegistry = nullptr;

	void Load(TextureGridMap& worldMap, const Vector2& initPos, float maxHealth, int sizeCol, int sizeRow, std::string defaultTexture, int capacity, int healthBarSliceCount);

	void Unload()
	{

	}

	virtual void Update(float deltaTime) override
	{
		Structure::Update(deltaTime);

		if (mCurrentHealth <= 0.0f)
			Destory();

		if (mIsFinishDying)
			Die();

		mHealthBar.Update(mCurrentHealth / mMaxHealth);
		mBoolAnimator.Update(deltaTime);
	}
	virtual void Render(const Camera2D& camera) override
	{
		if (!mIsActive)
			return;
		//RenderSelectedHelo_backHalf();  // TODO
		//RenderAlienceHelo_backHalf();		// TODO
		Structure::Render(camera);
		//RenderAlienceHelo_frontHalf();// TODO
		//RenderAlienceHelo_frontHalf();// TODO
		
		Vector2 mousePos = { static_cast<float>(GetMouseScreenX()), static_cast<float>(GetMouseScreenY()) };
		Vector2 checkPos = camera.ConvertToScreen(mPosition + mTextureRenderOffSet + Vector2{ mDefaultWidth * mDrawScale * 0.5f, -mDefaultHeight * mDrawScale * 0.5f });
		bool IsMouseOnTop = EngineWrapper::PointInCircle(mousePos, Circle{ checkPos, mDefaultWidth * mDrawScale * 0.5f });
		if (mIsSelected || IsMouseOnTop)
		{
			float healthBarLength = mDefaultWidth * mDrawScale * camera.GetScale();
			Vector2 toTopLeft = { 0.0f, -mDefaultHeight * mDrawScale };
			mHealthBar.Render(camera.ConvertToScreen(mPosition + mTextureRenderOffSet + toTopLeft), healthBarLength, camera.GetScale());
		}
		
	}
	virtual void DebugUIUpdate(float deltaTime, const Camera2D& camera) override
	{
		Structure::DebugUIUpdate(deltaTime, camera);
	}

	virtual void Interact() = 0;

	virtual void Attack( Unit& unit, float deltaTime) = 0;
	virtual void Attack( Building& strcuture, float deltaTime) = 0;
	virtual void TakeDamage(Projectile& bullet) = 0;
	virtual void Die() override
	{
		mIsActive = false;
		*mInLevelRegistry = nullptr;
		mInLevelRegistry = nullptr;
		RemoveFromMap();
	}

private:
	void RenderSelectedHelo_frontHalf();
	void RenderSelectedHelo_backHalf();
	void RenderAlienceHelo_frontHalf();
	void RenderAlienceHelo_backHalf();

	virtual void Destory() = 0;
	virtual void BuildUnit(int index) = 0;
};

#endif // !LACELLECRAFT_GAMEOBJECT_BUILDING_HEADER
