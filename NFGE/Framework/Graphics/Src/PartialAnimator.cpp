#include "Precompiled.h"
#include "PartialAnimator.h"
#include "BlendTree.h"

using namespace NFGE::Graphics;

void NFGE::Graphics::PartialAnimator::Blender::SetBlender(float blendDuration, float(*blendEaser)(float))
{
	if (mIsActive == true)
	{
		LoadInnerTransition();
		mIsInnerTransitionBlending = true;
	}
	mBlendingTimeCounter = 0.0f;

	mBlendingPositionHint = 0;
	mBlendingRotationHint = 0;
	mBlendingScaleHint = 0;

	mBlendDuration = blendDuration;
	funcPtr = blendEaser;
	mIsActive = true;
}

void NFGE::Graphics::PartialAnimator::Blender::Reset()
{
	mBlendingTimeCounter = 0.0f;

	mBlendingPositionHint = 0;
	mBlendingRotationHint = 0;
	mBlendingScaleHint = 0;

	mBlendDuration = 0.0f;
	mIsActive = false;
}

void NFGE::Graphics::PartialAnimator::Blender::GetMixedTransform(int boneIndex, NFGE::Math::Vector3 & translate, NFGE::Math::Quaternion & rotation, NFGE::Math::Vector3 & scale, bool& hasAnimation)
{
	ASSERT(mAnimator->mBlendTree != nullptr, "[PartialAnimator] can not retrive mix animation with null blendTree");
	auto blendTreePtr = mAnimator->mBlendTree;
	const std::vector<ClipModifyer*>& currentActiveClip = blendTreePtr->GetClipModifyers();
	hasAnimation = true;
	for (auto& clipsModifyer : currentActiveClip)
	{
		auto& animationClip = mAnimator->mClips[clipsModifyer->mClipIndex];
		auto& targetAnimation = animationClip->boneAnimations[boneIndex];

		float clipTime = blendTreePtr->GetBlendTime() * animationClip->ticksPerSecond * mAnimator->mClipSpeedMutiplyer[clipsModifyer->mClipIndex];
		float duration = animationClip->duration;
		float currentTime = fmod(clipTime, animationClip->duration);
		if (clipsModifyer->mIsPlayTimeRelateWeight)
		{
			currentTime = duration * clipsModifyer->mPlayTimeWeight;
		}

		if (targetAnimation == nullptr)
		{
			hasAnimation = false;
			translate += NFGE::Math::Vector3::Zero() * clipsModifyer->mWeight;
			//rotation = rotation * (NFGE::Math::Quaternion::Identity() * clipsModifyer->mWeight);//[May cause problem, cause the quaternion need to slerp]
			//rotation = NFGE::Math::Slerp(rotation, NFGE::Math::Quaternion::Identity(), clipsModifyer->mWeight);
			scale += NFGE::Math::Vector3::One() * clipsModifyer->mWeight;
		}
		else
		{
		
			auto& currentHints = mAnimator->mClipsHints[clipsModifyer->mClipIndex];
			
			translate += targetAnimation->animation->GetValue_OnKeyframes(targetAnimation->animation->mPositionChannel.mKeyFrames, currentTime, &currentHints.positionHint) * clipsModifyer->mWeight;
			
			auto currentRotation = targetAnimation->animation->GetValue_OnKeyframes(targetAnimation->animation->mRotationChannel.mKeyFrames, currentTime, &currentHints.rotationHint);
			if (currentRotation.w < 0.0f)
				currentRotation = currentRotation * -1.0f;					// The animation get from Mixamo has rotation quaterion fliped for some bone. It won't effect the final rotation, but fuked up when mix them with weight.
			rotation = rotation + (currentRotation * clipsModifyer->mWeight);//[May cause problem, cause the quaternion need to slerp]
			//rotation = NFGE::Math::Slerp(rotation, targetAnimation->animation->GetValue_OnKeyframes(targetAnimation->animation->mRotationChannel.mKeyFrames, currentTime, &currentHints.rotationHint), clipsModifyer->mWeight);
			
			scale += targetAnimation->animation->GetValue_OnKeyframes(targetAnimation->animation->mScaleChannel.mKeyFrames, currentTime, &currentHints.scaleHint) * clipsModifyer->mWeight;
		}
	}
	
	mLastMixPos[boneIndex] = translate;
	mLastMixRot[boneIndex] = rotation;
	mLastMixScl[boneIndex] = scale;
}

void NFGE::Graphics::PartialAnimator::Blender::LoadInnerTransition()
{
	for (int i = 0; i < static_cast<int>(mInnerTransitionPos.size()); i++)
	{
		mInnerTransitionPos[i] = mAnimator->mCurrentBlendPos[i];
		mInnerTransitionRot[i] = mAnimator->mCurrentBlendRot[i];
		mInnerTransitionScl[i] = mAnimator->mCurrentBlendScl[i];
	}
}

void PartialAnimator::Blender::Update(float deltaTime)
{

	if (!mIsActive) return;

	if (mIsMixing)
	{
		mAnimator->mBlendTree->Update(deltaTime);
		//int indexCount = 0;
		//for (auto& clipsModifyer : *mMixingClipsModifersPtr)
		//{
		//	auto& targetAnimationClip = mAnimator->mClips[clipsModifyer.mClipIndex];
		//	if (indexCount == 0)
		//	{
		//		clipsModifyer.timer = mAnimator->mCounter;
		//	}
		//	else
		//	{
		//		float animationLastFrameTime = targetAnimationClip->duration;
		//		if (clipsModifyer.timer > animationLastFrameTime)
		//		{
		//			clipsModifyer.timer -= animationLastFrameTime;
		//		}
		//		clipsModifyer.timer += deltaTime * targetAnimationClip->ticksPerSecond * mAnimator->mClipSpeedMutiplyer[clipsModifyer.mClipIndex];
		//	}
		//	++indexCount;
		//}
		return;
	}

	float changingAnimationTime = deltaTime * mAnimator->mClips[mAnimator->mCurrentClip]->ticksPerSecond * mAnimator->mClipSpeedMutiplyer[mAnimator->mCurrentClip]; // Get the speed with speed mutiplyer
	float newDeltaTime = changingAnimationTime * mAnimator->mClipSpeedMutiplyer[mAnimator->mNextClip];
	mBlendingTimeCounter += newDeltaTime;

	if (mBlendingTimeCounter > mBlendDuration)
	{
		mAnimator->mCurrentClip = mAnimator->mNextClip;
		auto& currentHint = mAnimator->mClipsHints[mAnimator->mCurrentClip];
		mAnimator->mCounter = mBlendingTimeCounter;	//|Set finish blend time counter to the real animator
		currentHint.positionHint = mBlendingPositionHint;
		currentHint.rotationHint = mBlendingRotationHint;
		currentHint.scaleHint = mBlendingScaleHint;

		mBlendingTimeCounter = 0.0f;
		mBlendingPositionHint = 0;
		mBlendingRotationHint = 0;
		mBlendingScaleHint = 0;
		mAnimator->mNextClip = -1;
		mIsActive = false;
		mIsInnerTransitionBlending = false;
	}
}

void NFGE::Graphics::PartialAnimator::ChangeTo(int clipIndex, float nextClipBlendPercentage, float(*funcPtr)(float))
{
	ASSERT(nextClipBlendPercentage >= 0.0f, "[Graphic::Animator] BlendPercentage for nextClip can not less than 0.0f");
	mNextClip = clipIndex;
	mBlender->SetBlender(nextClipBlendPercentage * mClips[mNextClip]->duration, funcPtr);

	for (auto effectBone : mEffectedBones)
	{
		mLastEffectingPartial[effectBone] = mBoneRegistry[effectBone];
	}

	AdjustRegistry();

	mIsPause = false;
	mBlender->mIsMixing = false;
}

void NFGE::Graphics::PartialAnimator::BindBlendTree(BlendTreeBase* blendTree)
{
	//auto& currentClipModifyers = mBlender->mMixingClipsModifersPtr;
	//if (currentClipModifyers != nullptr && isSyncHint)
	//{
	//	ASSERT(currentClipModifyers->size() == clipModifyers.size(), "[PartialAnimator] Can not sync the animation hint with clipModifyer with different size");
	//	
	//	int index = 0;
	//	for (auto& clipModifyer : clipModifyers)
	//	{
	//		clipModifyer.mPositionHint = (*currentClipModifyers)[index].mPositionHint;
	//		clipModifyer.mRotationHint = (*currentClipModifyers)[index].mRotationHint;
	//		clipModifyer.mScaleHint = (*currentClipModifyers)[index].mScaleHint;
	//		++index;
	//	}
	//}

	//currentClipModifyers = &clipModifyers;

	mBlendTree = blendTree;

	for (auto effectBone : mEffectedBones)
	{
		mLastEffectingPartial[effectBone] = mBoneRegistry[effectBone];
	}

	AdjustRegistry();

	mIsPause = false;
	mBlender->mIsActive = true;
	mBlender->mIsMixing = true;
}

void PartialAnimator::Update(float deltaTime)
{
	if (mIsPause) return;

	if (mCounter < 0.0f) mCounter = 0.0f; // Overflow check

	if (mCounter > mClips[mCurrentClip]->duration)
	{
		if (mIsLoop[mCurrentClip])
			mCounter -= mClips[mCurrentClip]->duration;
		else
		{
			if (!mBlender->mIsActive)
			{
				mCounter = 0.0f;
				//mPositionHint = 0;			//TODO delete this line after moding
				//mRotationHint = 0;			//TODO delete this line after moding
				//mScaleHint = 0;				//TODO delete this line after moding
				mIsPause = true;
			}
		}
	}

	float changingAnimationTime = deltaTime * mClips[mCurrentClip]->ticksPerSecond * mClipSpeedMutiplyer[mCurrentClip]; // Get the speed with speed mutiplyer

	mBlender->Update(deltaTime);

	mCounter += changingAnimationTime;
}

void NFGE::Graphics::PartialAnimator::SetClip(int clipId, bool isLoop, float speedMutilpyer)
{
	mIsLoop[clipId] = isLoop;
	mClipSpeedMutiplyer[clipId] = speedMutilpyer;
}

NFGE::Math::Matrix4 NFGE::Graphics::PartialAnimator::GetTranform(int boneIndex, bool& hasAnimation)
{
	ASSERT((boneIndex < static_cast<int>(mClips[mCurrentClip]->boneAnimations.size())), "[NFGE::Graphics::Animator] try to get a bone animation that is out of bounds");

	hasAnimation = true;
	if (mBlender->mIsActive)
	{
		auto& finalTranslate = mCurrentBlendPos[boneIndex];
		auto& finalRotation = mCurrentBlendRot[boneIndex];
		auto& finalScale = mCurrentBlendScl[boneIndex];

		if (mBlender->mIsMixing)
		{
			finalTranslate = NFGE::Math::Vector3::Zero();
			finalRotation = NFGE::Math::Quaternion::Zero();
			finalScale = NFGE::Math::Vector3::Zero();

			mBlender->GetMixedTransform(boneIndex, finalTranslate, finalRotation, finalScale, hasAnimation);
		}
		else
		{
			ASSERT((boneIndex < static_cast<int>(mClips[mNextClip]->boneAnimations.size())), "[NFGE::Graphics::Animator] try to get a bone animation that is out of bounds");
			auto& nextBoneAnimation = mClips[mNextClip]->boneAnimations[boneIndex];

			auto translate1 = NFGE::Math::Vector3::Zero();
			auto rot1 = NFGE::Math::Quaternion::Identity();
			auto scl1 = NFGE::Math::Vector3::One();

			if (nextBoneAnimation != nullptr)
			{
				auto& nextAnimation = nextBoneAnimation->animation;
				translate1 = nextAnimation->GetValue_OnKeyframes(nextAnimation->mPositionChannel.mKeyFrames, mBlender->mBlendingTimeCounter, &mBlender->mBlendingPositionHint);
				rot1 = nextAnimation->GetValue_OnKeyframes(nextAnimation->mRotationChannel.mKeyFrames, mBlender->mBlendingTimeCounter, &mBlender->mBlendingRotationHint);
				scl1 = nextAnimation->GetValue_OnKeyframes(nextAnimation->mScaleChannel.mKeyFrames, mBlender->mBlendingTimeCounter, &mBlender->mBlendingScaleHint);
			}

			auto translate0 = mAllPartials[mLastEffectingPartial[boneIndex]]->GetTranslationValue(boneIndex, hasAnimation);

			auto rot0 = mAllPartials[mLastEffectingPartial[boneIndex]]->GetRotationValue(boneIndex, hasAnimation);

			auto scl0 = mAllPartials[mLastEffectingPartial[boneIndex]]->GetScaleValue(boneIndex, hasAnimation);

			if (mBlender->mIsInnerTransitionBlending)
			{
				translate0 = mBlender->mInnerTransitionPos[boneIndex];
				rot0 = mBlender->mInnerTransitionRot[boneIndex];
				scl0 = mBlender->mInnerTransitionScl[boneIndex];
			}

			finalTranslate = Animation::InterpolationChannelKeys(translate0, translate1, mBlender->GetBlendInWeight());
			finalRotation = Animation::InterpolationChannelKeys(rot0, rot1, mBlender->GetBlendInWeight());
			finalScale = Animation::InterpolationChannelKeys(scl0, scl1, mBlender->GetBlendInWeight());
		}

		Math::Matrix4 mat = Math::MatrixRotationQuaternion(finalRotation);
		mat._11 *= finalScale.x;
		mat._22 *= finalScale.y;
		mat._33 *= finalScale.z;
		mat._41 = finalTranslate.x;
		mat._42 = finalTranslate.y;
		mat._43 = finalTranslate.z;
		return mat;
	}
	else
	{
		if (mClips[mCurrentClip]->boneAnimations[boneIndex] == nullptr)				
		{																			
			hasAnimation = false;												
			return Math::Matrix4::sIdentity();										
		}																			
		auto& currentAnimation = mClips[mCurrentClip]->boneAnimations[boneIndex]->animation;
		auto& currentHints = mClipsHints[mCurrentClip];
		return currentAnimation->GetAllTransform(mCounter, &currentHints.positionHint, &currentHints.rotationHint, &currentHints.scaleHint);
	}
}

NFGE::Math::Matrix4 NFGE::Graphics::PartialAnimator::GetTranslation(int boneIndex, bool& hasAnimation)
{
	return NFGE::Math::Matrix4::sTranslation(GetTranslationValue(boneIndex, hasAnimation));
}

NFGE::Math::Matrix4 NFGE::Graphics::PartialAnimator::GetRotation(int boneIndex, bool& hasAnimation)
{
	return NFGE::Math::MatrixRotationQuaternion(GetRotationValue(boneIndex, hasAnimation));
}

NFGE::Math::Matrix4 NFGE::Graphics::PartialAnimator::GetScale(int boneIndex, bool& hasAnimation)
{
	return NFGE::Math::Matrix4::sScaling(GetScaleValue(boneIndex, hasAnimation));
}

NFGE::Math::Vector3 NFGE::Graphics::PartialAnimator::GetTranslationValue(int boneIndex, bool & hasAnimation)
{
	ASSERT((boneIndex < static_cast<int>(mClips[mCurrentClip]->boneAnimations.size())), "[NFGE::Graphics::Animator] try to get a bone animation that is out of bounds");																		//|

	hasAnimation = true;
	if (mBlender->mIsActive)
	{
		if (mBlender->mIsMixing)
		{
			return mBlender->mLastMixPos[boneIndex];
		}
		else
		{
			ASSERT((boneIndex < static_cast<int>(mClips[mNextClip]->boneAnimations.size())), "[NFGE::Graphics::Animator] try to get a bone animation that is out of bounds");
			auto& finalTranslate = mCurrentBlendPos[boneIndex];

			auto translate1 = NFGE::Math::Vector3::Zero();
			auto& nextBoneAnimation = mClips[mNextClip]->boneAnimations[boneIndex];
			if (nextBoneAnimation != nullptr)
			{
				auto& nextAnimation = nextBoneAnimation->animation;
				translate1 = nextAnimation->GetValue_OnKeyframes(nextAnimation->mPositionChannel.mKeyFrames, mBlender->mBlendingTimeCounter, &mBlender->mBlendingPositionHint);
			}

			NFGE::Math::Vector3 translate0;
			int currentLastEffectPartial = mLastEffectingPartial[boneIndex];
			if (currentLastEffectPartial != mParticleId)
				translate0 = mAllPartials[currentLastEffectPartial]->GetTranslationValue(boneIndex, hasAnimation);
			else
			{
				auto& currentBoneAnimation = mClips[mCurrentClip]->boneAnimations[boneIndex];
				auto& currentHints = mClipsHints[mCurrentClip];
				if (currentBoneAnimation == nullptr)
					translate0 = NFGE::Math::Vector3::Zero();
				else
					translate0 = currentBoneAnimation->animation->GetValue_OnKeyframes(currentBoneAnimation->animation->mPositionChannel.mKeyFrames, mCounter, &currentHints.positionHint);
			}

			if (mBlender->mIsInnerTransitionBlending)
				translate0 = mBlender->mInnerTransitionPos[boneIndex];

			finalTranslate = Animation::InterpolationChannelKeys(translate0, translate1, mBlender->GetBlendInWeight());
			return finalTranslate;
		}
	}
	else
	{
		if (mClips[mCurrentClip]->boneAnimations[boneIndex] == nullptr)				//|
		{																			//|
			hasAnimation = false;													//|--- Early out if current bone dont have animation
			return NFGE::Math::Vector3::Zero();										//|
		}
		auto& currentAnimation = mClips[mCurrentClip]->boneAnimations[boneIndex]->animation;
		auto& currentHints = mClipsHints[mCurrentClip];
		return currentAnimation->GetValue_OnKeyframes(currentAnimation->mPositionChannel.mKeyFrames, mCounter, &currentHints.positionHint);
	}
}

NFGE::Math::Quaternion NFGE::Graphics::PartialAnimator::GetRotationValue(int boneIndex, bool & hasAnimation)
{
	ASSERT((boneIndex < static_cast<int>(mClips[mCurrentClip]->boneAnimations.size())), "[NFGE::Graphics::Animator] try to get a bone animation that is out of bounds");																		//|

	hasAnimation = true;
	if (mBlender->mIsActive)
	{
		if (mBlender->mIsMixing)
		{
			return mBlender->mLastMixRot[boneIndex];
		}
		else
		{
			ASSERT((boneIndex < static_cast<int>(mClips[mNextClip]->boneAnimations.size())), "[NFGE::Graphics::Animator] try to get a bone animation that is out of bounds");
			auto& finalRot = mCurrentBlendRot[boneIndex];

			auto rot1 = NFGE::Math::Quaternion::Identity();
			auto& nextBoneAnimation = mClips[mNextClip]->boneAnimations[boneIndex];
			if (nextBoneAnimation != nullptr)
			{
				auto& nextAnimation = nextBoneAnimation->animation;
				rot1 = nextAnimation->GetValue_OnKeyframes(nextAnimation->mRotationChannel.mKeyFrames, mBlender->mBlendingTimeCounter, &mBlender->mBlendingRotationHint);
			}

			NFGE::Math::Quaternion rot0;
			int currentLastEffectPartial = mLastEffectingPartial[boneIndex];
			if (currentLastEffectPartial != mParticleId)
				rot0 = mAllPartials[currentLastEffectPartial]->GetRotationValue(boneIndex, hasAnimation);
			else
			{
				auto& currentBoneAnimation = mClips[mCurrentClip]->boneAnimations[boneIndex];
				auto& currentHints = mClipsHints[mCurrentClip];
				if (currentBoneAnimation == nullptr)
					rot0 = NFGE::Math::Quaternion::Identity();
				else
					rot0 = currentBoneAnimation->animation->GetValue_OnKeyframes(currentBoneAnimation->animation->mRotationChannel.mKeyFrames, mCounter, &currentHints.rotationHint);
			}

			if (mBlender->mIsInnerTransitionBlending)
				rot0 = mBlender->mInnerTransitionRot[boneIndex];

			finalRot = Animation::InterpolationChannelKeys(rot0, rot1, mBlender->GetBlendInWeight());
			return finalRot;
		}	
	}
	else
	{
		if (mClips[mCurrentClip]->boneAnimations[boneIndex] == nullptr)				//|
		{																			//|
			hasAnimation = false;													//|--- Early out if current bone dont have animation
			return NFGE::Math::Quaternion::Identity();										//|
		}
		auto& currentAnimation = mClips[mCurrentClip]->boneAnimations[boneIndex]->animation;
		auto& currentHints = mClipsHints[mCurrentClip];
		return currentAnimation->GetValue_OnKeyframes(currentAnimation->mRotationChannel.mKeyFrames, mCounter, &currentHints.rotationHint);
	}
}

NFGE::Math::Vector3 NFGE::Graphics::PartialAnimator::GetScaleValue(int boneIndex, bool & hasAnimation)
{
	ASSERT((boneIndex < static_cast<int>(mClips[mCurrentClip]->boneAnimations.size())), "[NFGE::Graphics::Animator] try to get a bone animation that is out of bounds");																		//|

	hasAnimation = true;
	if (mBlender->mIsActive)
	{
		if (mBlender->mIsMixing)
		{
			return mBlender->mLastMixScl[boneIndex];
		}
		else
		{
			ASSERT((boneIndex < static_cast<int>(mClips[mNextClip]->boneAnimations.size())), "[NFGE::Graphics::Animator] try to get a bone animation that is out of bounds");
			auto& finalScale = mCurrentBlendScl[boneIndex];

			auto scl1 = NFGE::Math::Vector3::One();
			auto& nextBoneAnimation = mClips[mNextClip]->boneAnimations[boneIndex];
			if (nextBoneAnimation != nullptr)
			{
				auto& nextAnimation = nextBoneAnimation->animation;
				scl1 = nextAnimation->GetValue_OnKeyframes(nextAnimation->mScaleChannel.mKeyFrames, mBlender->mBlendingTimeCounter, &mBlender->mBlendingScaleHint);
			}

			NFGE::Math::Vector3 scl0;
			int currentLastEffectPartial = mLastEffectingPartial[boneIndex];
			if (currentLastEffectPartial != mParticleId)
				scl0 = mAllPartials[currentLastEffectPartial]->GetScaleValue(boneIndex, hasAnimation);
			else
			{
				auto& currentBoneAnimation = mClips[mCurrentClip]->boneAnimations[boneIndex];
				auto& currentHints = mClipsHints[mCurrentClip];
				if (currentBoneAnimation == nullptr)
					scl0 = NFGE::Math::Vector3::One();
				else
					scl0 = currentBoneAnimation->animation->GetValue_OnKeyframes(currentBoneAnimation->animation->mScaleChannel.mKeyFrames, mCounter, &currentHints.scaleHint);
			}

			if (mBlender->mIsInnerTransitionBlending)
				scl0 = mBlender->mInnerTransitionScl[boneIndex];

			finalScale = Animation::InterpolationChannelKeys(scl0, scl1, mBlender->GetBlendInWeight());
			return finalScale;
		}
	}
	else
	{
		if (mClips[mCurrentClip]->boneAnimations[boneIndex] == nullptr)				//|
		{																			//|
			hasAnimation = false;													//|--- Early out if current bone dont have animation
			return NFGE::Math::Vector3::One();										//|
		}
		auto& currentAnimation = mClips[mCurrentClip]->boneAnimations[boneIndex]->animation;
		auto& currentHints = mClipsHints[mCurrentClip];
		return currentAnimation->GetValue_OnKeyframes(currentAnimation->mScaleChannel.mKeyFrames, mCounter, &currentHints.scaleHint);
	}
}

void NFGE::Graphics::PartialAnimator::SyncWith(const PartialAnimator & other)
{
	mCurrentClip = other.mCurrentClip;
	mCounter = other.mCounter;
}

void NFGE::Graphics::PartialAnimator::AdjustRegistry()
{
	for (auto boneIndex : mEffectedBones)
	{
		mBoneRegistry[boneIndex] = mParticleId;
	}
}

void NFGE::Graphics::PartialAnimator::Adapt(const PartialAnimator & other)
{
	int clipSize = static_cast<int>(other.mClips.size());
	mClipsHints.resize(clipSize);
	mClipSpeedMutiplyer.resize(clipSize);
	mIsLoop.resize(clipSize);
	for (int i = 0; i < clipSize; i++)
	{
		mClipSpeedMutiplyer[i] = other.mClipSpeedMutiplyer[i];
		mIsLoop[i] = other.mIsLoop[i];
	}

	int totalBoneCount = static_cast<int>(other.mCurrentBlendPos.size());
	mBlender->mInnerTransitionPos.resize(totalBoneCount, {});
	mBlender->mInnerTransitionRot.resize(totalBoneCount, {});
	mBlender->mInnerTransitionScl.resize(totalBoneCount, {});
	mBlender->mLastMixPos.resize(totalBoneCount, {});
	mBlender->mLastMixRot.resize(totalBoneCount, {});
	mBlender->mLastMixScl.resize(totalBoneCount, {});
}

void NFGE::Graphics::PartialAnimator::Play(int clipIndex)
{
	mCounter = 0.0f;
	mCurrentClip = clipIndex;
	mClipsHints[clipIndex].positionHint = 0;
	mClipsHints[clipIndex].rotationHint = 0;
	mClipsHints[clipIndex].scaleHint = 0;
	mBlender->Reset();
	mIsPause = false;
	AdjustRegistry();
}

