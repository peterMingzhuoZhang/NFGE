#ifndef LACELLECRAFT_GAMEOBJECT_UNIT
#define LACELLECRAFT_GAMEOBJECT_UNIT

#include <NFGE/Inc/NFGE.h>
#include "EngineWrapper.h"
#include "TextureGridMap.h"
#include "Animation.h"
#include "HealthBar.h"
#include "Fightable.h"

#include "Projectile.h"

using namespace NFGE;
using namespace EngineWrapper;
using namespace LC;

struct Unit : public AI::Agent, Fightable
{
	Unit(AI::AIWorld& world)
		:AI::Agent(world)
	{

	}

	const static int UnitDefaultDirectionCount = 32;
	int directionCount = 0;
	bool mIsActive = false;
	bool mIsBuilding = false;

	TextureGridMap* mWorldMap;
	int mCommanderIndex;

	int mCurrentDirection;
	LCAnimator mAnimator;
	float mDefaultWidth;
	float mDefaultHeight;
	Vector2 mDrawScale;
	Vector2 mSize;
	float mAltitude = -10.0f;

	float mMaxHealth;
	float mCurrentHealth;

	HealthBar mHealthBar;

	float mTurningSpeed = 2.0f;
	//------------------
	bool mIsSelected = false;
	bool mIsMouseOnTop = false;
	//------------------

	//------------------------------------------------------------
	std::unique_ptr<AI::SteeringModule> steeringModule;

	Unit** mInLevelRegistry = nullptr;

	void Load(TextureGridMap& worldMap, const Vector2& initPos, const Vector2& size, float maxHealth, int commanderIndex);
	

	virtual void Update(float deltaTime);
	

	virtual	void ControlUpdate(const Vector2& currentMousePos, float deltaTime);
	virtual void Render(const Camera2D& camera) = 0;
	virtual void DebugUIUpdate(float deltaTime, const Camera2D& camera)
	{
		Vector2 checkPos = camera.ConvertToScreen(position);
		DrawScreenCircle( checkPos, mSize.x * 0.5f, Red);
	}

	bool Selecting(const Vector2& mousePos, const Camera2D& camera)
	{
		Vector2 positionWithAltitude = { position.x, position.y + mAltitude };
		Vector2 checkPos = camera.ConvertToScreen(positionWithAltitude);
		mIsSelected = EngineWrapper::PointInCircle(mousePos, Circle{ checkPos, mSize.x * 0.5f });
		return mIsSelected;
	}
	bool Selecting(const Rect& selectingRect, const Camera2D& camera)
	{
		Vector2 checkPos = camera.ConvertToScreen(position);
		mIsSelected = EngineWrapper::PointInRect(checkPos, selectingRect);
		return mIsSelected;
	}
	void DeSelecting()
	{
		mIsSelected = false;
	}

	virtual void Interact() = 0;

	void UpdatePhysics(float deltaTime)
	{
		auto force = steeringModule->Calculate() * mTurningSpeed;
		auto acceleration = force / mass;
		velocity += acceleration * deltaTime;
		// TODO add truncate method here
		position += velocity * deltaTime;

		if (NFGE::Math::MagnitudeSqr(velocity) > 1.0f)
			heading = NFGE::Math::Normalize(velocity);
	}

	virtual TextureId GetDefaultTexture(int index = 0) = 0;

	virtual void Attack( Unit& unit, float deltaTime) = 0;
	virtual void Attack( Building& strcuture, float deltaTime) = 0;
	virtual void TakeDamage(Projectile& bullet) = 0;
	virtual void Die() override;
};

#endif // !LACELLECRAFT_GAMEOBJECT_UNIT
