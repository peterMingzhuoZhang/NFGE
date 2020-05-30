#include "Unit.h"
#include "LevelManager.h"

using namespace NFGE::Input;

void Unit::Load(TextureGridMap & worldMap, const Vector2 & initPos, const Vector2 & size, float maxHealth, int commanderIndex)
{
	mIsActive = true;
	mWorldMap = &worldMap;
	mCommanderIndex = commanderIndex;
	mMaxHealth = maxHealth;
	mCurrentDirection = 0;
	mAnimator.Load();
	position = initPos;

	mSize = size;
	mDrawScale = { size.x / mDefaultWidth, size.y / mDefaultHeight };

	steeringModule = std::make_unique<AI::SteeringModule>(*this);

	ASSERT(mInLevelRegistry == nullptr, "[Unit] Try Load Unit twice, NOT ALLOWED!");
	mInLevelRegistry = LevelManager::Get()->RegisterNewUnit(this);
}

void Unit::Update(float deltaTime)
{
	if (mIsBuilding)
		return;

	//---------------//Agent physics Update
	UpdatePhysics(deltaTime);
	//---------------

	float angle = atan2(-heading.x, heading.y) + NFGE::Math::Constants::Pi;
	mCurrentDirection = (int)(angle / NFGE::Math::Constants::TwoPi * 32.0f) % 32;

	if (mAnimator.mClipCount != 0)
	{
		mAnimator.Update(deltaTime);
	}

	auto& levelManager = *LevelManager::Get();
	Vector2 positionWithAltitude = { position.x, position.y + mAltitude };
	Vector2 checkPos = levelManager.mWorldCamera->ConvertToScreen(positionWithAltitude);
	mIsMouseOnTop = EngineWrapper::PointInCircle(levelManager.mPlayerController.mCurrentMousePos, { checkPos, mSize.x * 0.5f });
	if (mIsMouseOnTop)
	{
		levelManager.mPlayerController.mIsHighLight = true;
		if (levelManager.mPlayerController.mCurrentHighLightUnit != this)
		{
			levelManager.mPlayerController.mCurrentHighLightStructure = nullptr;
			levelManager.mPlayerController.mCurrentHighLightResource = nullptr;
			levelManager.mPlayerController.mCurrentHighLightUnit = this;
		}
	}
}

void Unit::ControlUpdate(const Vector2 & currentMousePos, float deltaTime)
{
	if (IsMousePressed(MouseButton::RBUTTON))
	{
		destination = LevelManager::Get()->mWorldCamera->ConvertToWorld(currentMousePos);
	}
}

void Unit::Die()
{
	mIsActive = false;
	*mInLevelRegistry = nullptr;
	mInLevelRegistry = nullptr;
	mIsBuilding = false;
}
