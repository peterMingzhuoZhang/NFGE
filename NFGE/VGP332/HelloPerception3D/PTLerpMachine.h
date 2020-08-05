#ifndef HELLOPERCEPTION3D_PTLERPMATHINE_H
#define HELLOPERCEPTION3D_PTLERPMATHINE_H

#include <NFGE/Inc/NFGE.h>

template <typename ValueType>
class PTLerpMachine
{
public:
	PTLerpMachine(ValueType* valuePtr)
		: mLerpValuePtr(valuePtr)
	{}

	void Start(ValueType startValue, ValueType finalValue, float duration)
	{
		mStartValue = std::move(startValue);
		mFinalValue = std::move(finalValue);
		ASSERT(!NFGE::Math::IsZero(duration), "[PTLerpMachine] can not lerp with duration 0!");
		mDuration = duration;
		mCounter = 0.0f;
		mIsFinish = false;
	}

	void Update(float deltaTime)
	{
		ASSERT(mLerpValuePtr != nullptr, "[PTLerpMachine] not inialized yet!");
		if (mIsFinish == true)
			return;

		if (mCounter <= mDuration)
			*mLerpValuePtr = NFGE::Math::Lerp(mStartValue, mFinalValue, mCounter / mDuration);
		else
		{
			mIsFinish = true;
			*mLerpValuePtr = NFGE::Math::Lerp(mStartValue, mFinalValue, 1.0f);
		}

		mCounter += deltaTime;
	}
private:
	ValueType* mLerpValuePtr;
	bool mIsFinish = true;
	float mCounter = 0.0f;
	float mDuration = 0.0f;
	ValueType mStartValue;
	ValueType mFinalValue;

};

#endif // !HELLOPERCEPTION3D_PTLERPMATHINE_H
