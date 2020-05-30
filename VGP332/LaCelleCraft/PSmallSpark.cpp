#include "PSmallSpark.h"
#include "LevelManager.h"

using namespace LC;

extern BattleMap myBattleMap;
namespace
{
	std::string defaultTexturePrefix = "LaCelleCraft/Projectiles/SmallSpark/projectile_smallSpark_";
	std::string defaultTextureSubfix = ".png";

	const float bulletMass = 1.0f;
	const float bulletMaxSpeed = 0.0f;
	const float dmg = 0.0f;
	const bool IsAOE = false;
	const Vector2 Size = {32.0f,32.0f};
	const float turningSpeed = 10.0f;
	const float Altitude = -10.0f;

	std::string texturePrefix[ProjectileState::TotalStateCount] =
	{

		"LaCelleCraft/Projectiles/SmallSpark/projectile_smallSpark_",
		"LaCelleCraft/Projectiles/SmallSpark/projectile_smallSpark_",
		"LaCelleCraft/Projectiles/SmallSpark/projectile_smallSpark_",
	};
	std::string textureSubfix = ".png";
	const int frameCount[ProjectileState::TotalStateCount] =
	{
		1, 1 ,6
	};
	const bool isLoop[ProjectileState::TotalStateCount] =
	{
		 true, true, false
	};
	const int toState[ProjectileState::TotalStateCount] =
	{
		 -1, -1, ProjectileState::Idle
	};
}

TextureId PSmallSpark::mDefaultTexture[directionCount];

LC::PSmallSpark::PSmallSpark()
	:Projectile(myBattleMap)
{
	if (mDefaultTexture[0] == NULL)
	{
		for (int i = 0; i < directionCount; i++)
		{
			if (i < 10)
			{
				mDefaultTexture[i] = LoadTexture((defaultTexturePrefix + "0" + std::to_string(i) + defaultTextureSubfix).c_str());
			}
			else
			{
				mDefaultTexture[i] = LoadTexture((defaultTexturePrefix + std::to_string(i) + defaultTextureSubfix).c_str());
			}
		}
	}
	mAnimator.Load();

	for (int i = 0; i < ProjectileState::TotalStateCount; i++)
	{
		mAnimator.AddClip(LCAnimationClip(texturePrefix[i], textureSubfix, isLoop[i], frameCount[i], 1, toState[i]));
	}

	mDefaultWidth = GetSpriteWidth(mDefaultTexture[0]);
	mDefaultHeight = GetSpriteHeight(mDefaultTexture[0]);

	mSize = Size;
	mDrawScale = { mSize.x / mDefaultWidth, mSize.y / mDefaultHeight };

	mass = bulletMass;
	maxSpeed = bulletMaxSpeed;
	mDmg = dmg;
	mIsAOE = IsAOE;
	mTurningSpeed = turningSpeed;
	mAltitude = Altitude;

	stateMachine->AddState<Projectile_State::IdleState>();
	//stateMachine->AddState<Projectile_State::InTravelState>();
	stateMachine->AddState<Projectile_State::DestoryedState>();
}

void LC::PSmallSpark::Fire(const Vector2 & initPos, const Vector2 & size, int commanderIndex, Building & building)
{
	Load();
	mIsActive = true;
	mDrawScale = {mDrawScale.x * size.x, mDrawScale.y * size.y};
	position = initPos;
	destination = initPos;
	mTargetBuilding = &building;
	stateMachine->ChangeState("Destoryed");
}

void LC::PSmallSpark::Fire(const Vector2 & initPos, const Vector2 & size, int commanderIndex, Unit & structure)
{

}

void LC::PSmallSpark::Render(const Camera2D & camera)
{
	if (!mIsActive)
		return;

	Vector2 positionWithAltitude = { position.x, position.y + mAltitude };
	Vector2 drawPos = camera.ConvertToScreen(positionWithAltitude);
	if (!camera.IsOutOfView(drawPos, { mDefaultWidth * mDrawScale.x * camera.GetScale() *2.0f, mDefaultHeight * mDrawScale.y * camera.GetScale()*2.0f }))
	{

		DrawSprite(mAnimator.GetCurrentTexture(), drawPos, 0.0f, 1.0f, 0.5f, 0.5f, mDrawScale.x * camera.GetScale(), mDrawScale.y * camera.GetScale());
		
	}
}
