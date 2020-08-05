#ifndef LACELLECRAFT_PTANIMATION_H
#define LACELLECRAFT_PTANIMATION_H

#include <NFGE/Inc/NFGE.h>
#include <string>



enum AnimationType
{
	POSITION_ANIMATION = 0,
	INT_CHANGE_DELAY_ANIMATION,
	BOOL_CHANGE_DELAY_ANIMATION,
};


class PTAnimation
{
public:
	PTAnimation();
	PTAnimation(const PTAnimation& other);
	PTAnimation& operator=(const PTAnimation& other);
	~PTAnimation();

	void Load(NFGE::Math::Vector2* target, NFGE::Math::Vector2& toPositon, float duration);
	void Load(int* target, int ToValue, float duration);
	void Load(bool* target, bool ToValue, float duration);
	
	void Update(float deltaTime);

	AnimationType GetType() const { return mType; }
	bool GetIsActive() const { return mIsActive; }
private:
	AnimationType mType;

	float mFrameSecChecker = 0.0f;

	NFGE::Math::Vector2 mChangingPosition;
	NFGE::Math::Vector2* mTargetPositionPointer;

	int mChangeToInt;
	int* mTargetInt; 

	bool mChangeToBool;
	bool* mTargetBool;

	float mTimeInterval;
	float mTimeChecker;

	bool mIsActive;
	bool mIsForever;
};

#endif // !PTANIMATION_H
