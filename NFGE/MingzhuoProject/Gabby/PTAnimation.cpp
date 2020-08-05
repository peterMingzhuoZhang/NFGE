#include "PTAnimation.h"

namespace
{
	const int FPS = 12;
	const float frameSec = 1.0f / FPS;
	const int totalTextureAnimationCount = 1;
}

PTAnimation::PTAnimation()
	: mChangingPosition{0.0f,0.0f}
	, mTargetPositionPointer(nullptr)
	, mTimeInterval(0.0f)
	, mTimeChecker(0.0f)
	, mIsActive(false)
	, mIsForever(false)
{
}

PTAnimation::PTAnimation(const PTAnimation& other)
	: mFrameSecChecker(other.mFrameSecChecker)
	, mChangingPosition(other.mChangingPosition)
	, mTargetPositionPointer(other.mTargetPositionPointer)
	, mChangeToInt(other.mChangeToInt)
	, mTargetInt(other.mTargetInt)
	, mChangeToBool(other.mChangeToBool)
	, mTargetBool(other.mTargetBool)
	, mTimeInterval(other.mTimeInterval)
	, mTimeChecker(other.mTimeChecker)
	, mIsActive(other.mIsActive)
	, mIsForever(other.mIsForever)
{

}

PTAnimation& PTAnimation::operator=(const PTAnimation & other)
{
	if (&other == this)
	{
		return *this;
	}

	mFrameSecChecker = (other.mFrameSecChecker);
	mChangingPosition = (other.mChangingPosition);
	mTargetPositionPointer = (other.mTargetPositionPointer);
	mChangeToInt = (other.mChangeToInt);
	mTargetInt = (other.mTargetInt);
	mChangeToBool = (other.mChangeToBool);
	mTargetBool = (other.mTargetBool);
	mTimeInterval = (other.mTimeInterval);
	mTimeChecker = (other.mTimeChecker);
	mIsActive = (other.mIsActive);
	mIsForever = (other.mIsForever);

	return *this;
}

PTAnimation::~PTAnimation()
{
	
}

void PTAnimation::Load(NFGE::Math::Vector2* target, NFGE::Math::Vector2& toPositon, float duration) 
{
	mType = AnimationType::POSITION_ANIMATION;
	mFrameSecChecker = 0.0f;
	mTargetPositionPointer = target;
	mChangeToInt = 0;
	mTargetInt = nullptr;
	mChangeToBool = false;
	mTargetBool = nullptr;
	mTimeInterval = duration;
	mTimeChecker = 0.0f;
	mIsActive = true;
	mIsForever = false;
	if (duration == 0.0f)
	{
		mChangingPosition = toPositon;
	}
	else
	{
		float changingX = (toPositon.x - target->x) / duration;
		float changingY = (toPositon.y - target->y) / duration;
		mChangingPosition = { changingX,changingY };
	}
}

void PTAnimation::Load(int* target, int ToValue, float duration)
{
	mType = AnimationType::INT_CHANGE_DELAY_ANIMATION;
	mFrameSecChecker = 0.0f;
	mChangingPosition = { 0.0f,0.0f };
	mTargetPositionPointer = nullptr;
	mChangeToInt = ToValue;
	mTargetInt = target;
	mChangeToBool = false;
	mTargetBool = nullptr;
	mTimeInterval = duration;
	mTimeChecker = 0.0f;
	mIsActive = true;
	mIsForever = false;
}

void PTAnimation::Load(bool* target, bool ToValue, float duration)
{
	mType = AnimationType::BOOL_CHANGE_DELAY_ANIMATION;
	mFrameSecChecker = 0.0f;
	mChangingPosition = { 0.0f,0.0f };
	mTargetPositionPointer = nullptr;
	mChangeToInt = 0;
	mTargetInt = nullptr;
	mChangeToBool = ToValue;
	mTargetBool = target;
	mTimeInterval = duration;
	mTimeChecker = 0.0f;
	mIsActive = true;
	mIsForever = false;
}

void PTAnimation::Update(float deltaTime)
{
	if (mIsActive)
	{
		switch (mType)
		{
		case POSITION_ANIMATION:
			mTimeChecker += deltaTime;
			*mTargetPositionPointer += mChangingPosition * deltaTime;
			if (mTimeChecker >= mTimeInterval)
			{
				mIsActive = false;
			}
			break;
		case INT_CHANGE_DELAY_ANIMATION:
			mTimeChecker += deltaTime;
			if (mTimeChecker >= mTimeInterval)
			{
				*mTargetInt = mChangeToInt;
				mIsActive = false;
			}
			break;
		case BOOL_CHANGE_DELAY_ANIMATION:
			mTimeChecker += deltaTime;
			if (mTimeChecker >= mTimeInterval)
			{
				*mTargetBool = mChangeToBool;
				mIsActive = false;
			}
			break;
		default:
			break;
		}
	}
}
