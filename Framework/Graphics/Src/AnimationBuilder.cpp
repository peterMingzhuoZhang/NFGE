#include "Precompiled.h"
#include "Keyframe.h"
#include "Animation.h"
#include "AnimationBuilder.h"

using namespace NFGE::Graphics;

template<typename T>
inline bool compareTime(const T& v0, const T& v1)
{
	return (v0.time < v1.time);
}

template< typename T, typename Pred >
typename std::vector<T>::iterator
insert_sorted(std::vector<T> & vec, const T & item, Pred pred)
{
	return vec.insert
	(
		std::upper_bound(vec.begin(), vec.end(), item, pred),
		item
	);
}


NFGE::Graphics::AnimationBuilder::AnimationBuilder()
{
	mAnimationPtr = std::make_unique<Animation>();
}

NFGE::Graphics::AnimationBuilder::~AnimationBuilder()
{
	mAnimationPtr.release();
	mAnimationPtr.reset();
}

AnimationBuilder & NFGE::Graphics::AnimationBuilder::AddPositionKey(Math::Vector3 position, float time)
{
	Keyframe<Math::Vector3> newKey = Keyframe<Math::Vector3>{ position, time };
	//mAnimationPtr->mPositionChannel.mKeyFrames.emplace_back();
	insert_sorted(mAnimationPtr->mPositionChannel.mKeyFrames, newKey, compareTime<Keyframe<Math::Vector3>>);
	UpdateLastFrame(time);
	return *this;
}

AnimationBuilder & NFGE::Graphics::AnimationBuilder::AddRotationKey(Math::Quaternion rotation, float time)
{
	Keyframe<Math::Quaternion> newKey = Keyframe<Math::Quaternion>{ rotation, time };
	insert_sorted(mAnimationPtr->mRotationChannel.mKeyFrames, newKey, compareTime<Keyframe<Math::Quaternion>>);
	UpdateLastFrame(time);
	return *this;
}

AnimationBuilder & NFGE::Graphics::AnimationBuilder::AddScaleKey(Math::Vector3 scale, float time)
{
	Keyframe<Math::Vector3> newKey = Keyframe<Math::Vector3>{ scale, time };
	insert_sorted(mAnimationPtr->mScaleChannel.mKeyFrames, newKey, compareTime<Keyframe<Math::Vector3>>);
	UpdateLastFrame(time);
	return *this;
}

AnimationBuilder & NFGE::Graphics::AnimationBuilder::AddColorKey(Math::Vector4 color, float time)
{
	mAnimationPtr->mColorChannel.mKeyFrames.emplace_back(Keyframe<Math::Vector4>{color, time});
	Keyframe<Math::Vector4> newKey = Keyframe<Math::Vector4>{ color, time };
	insert_sorted(mAnimationPtr->mColorChannel.mKeyFrames, newKey, compareTime<Keyframe<Math::Vector4>>);
	UpdateLastFrame(time);
	return *this;
}

AnimationBuilder & NFGE::Graphics::AnimationBuilder::AddKey(Math::Vector3 position, Math::Quaternion rotation, float time)
{
	AddPositionKey(position, time);
	AddRotationKey(rotation, time);
	UpdateLastFrame(time);
	return *this;
}
void NFGE::Graphics::AnimationBuilder::AdaptExistingAniamtion(std::unique_ptr<Animation>& animationPtr)
{
	ASSERT(animationPtr != nullptr, "[AnimationBuilder] Try to adapt nullptr");
	mAnimationPtr = std::move(animationPtr);
}

std::unique_ptr<Animation> NFGE::Graphics::AnimationBuilder::Build()
{
	return std::move(std::exchange(mAnimationPtr, std::make_unique<Animation>()));
}

void NFGE::Graphics::AnimationBuilder::UpdateLastFrame(float time)
{
	ASSERT(mAnimationPtr, "[NFGE::Graphics::AnimationBuilder] mAnimationPtr is Empty");
	if (time > mAnimationPtr->mLastFrameTime)
		mAnimationPtr->mLastFrameTime = time;
}