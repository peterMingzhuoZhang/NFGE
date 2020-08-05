#include "Precompiled.h"
#include "Animator.h"
#include "Skeleton.h"
#include "BlendTree.h"

using namespace NFGE::Graphics;

void Animator::Update(float deltaTime)
{
	for (auto& partialAnimator : mPartials)
	{
		partialAnimator->Update(deltaTime);
	}
}

void NFGE::Graphics::Animator::SetClip(std::string clipName, bool isLoop, float speedMutilpyer, std::string partialName)
{
	SetClip(FindClip(clipName), isLoop, speedMutilpyer, FindPartial(partialName));
}

void NFGE::Graphics::Animator::SetClip(int clipId, bool isLoop, float speedMutilpyer, int partialId)
{
	mPartials[partialId]->SetClip(clipId, isLoop, speedMutilpyer);
}

NFGE::Math::Matrix4 NFGE::Graphics::Animator::GetTranform(int boneIndex, bool& hasAnimation)
{
	int usingPartial = mBoneRegistry[boneIndex];
	return mPartials[usingPartial]->GetTranform(boneIndex, hasAnimation);
}

NFGE::Math::Matrix4 NFGE::Graphics::Animator::GetTranslation(int boneIndex, bool& hasAnimation)
{
	int usingPartial = mBoneRegistry[boneIndex];
	return mPartials[usingPartial]->GetTranslation(boneIndex, hasAnimation);
}

NFGE::Math::Matrix4 NFGE::Graphics::Animator::GetRotation(int boneIndex, bool& hasAnimation)
{
	int usingPartial = mBoneRegistry[boneIndex];
	return mPartials[usingPartial]->GetRotation(boneIndex, hasAnimation);
}

NFGE::Math::Matrix4 NFGE::Graphics::Animator::GetScale(int boneIndex, bool& hasAnimation)
{
	int usingPartial = mBoneRegistry[boneIndex];
	return mPartials[usingPartial]->GetScale(boneIndex, hasAnimation);
}

NFGE::Graphics::Animator::Animator()
{
	
}

void NFGE::Graphics::Animator::Load(const Skeleton & skeleton, int PartialCount)
{
	int boneCount = static_cast<int>(skeleton.bones.size());
	mBoneRegistry.reserve(boneCount);
	mLastEffectingPartial.reserve(boneCount);

	for (int i = 0; i < boneCount; i++)
	{
		mBoneRegistry.emplace_back(0);
		mLastEffectingPartial.emplace_back(0);
	}

	ASSERT(PartialCount >= 1, "[Graphics::Animator] Has to have at least 1 [PartialAnimator]");

	mPartialName.reserve(PartialCount);
	mPartials.reserve(PartialCount);

	std::vector<int> allBoneIndex;
	allBoneIndex.reserve(skeleton.bones.size());
	for (auto& bone : skeleton.bones)
	{
		allBoneIndex.emplace_back(bone->index);
	}

	mIsInitialize = true;

	AddPartial("WholeBody", allBoneIndex);
}

void NFGE::Graphics::Animator::AddAnimationSet(const AnimationSet & animSet)
{
	ASSERT(mIsInitialize, "[Animator] has to be initialize before use, call Load().");
	for (auto& clip : animSet.boneAnimationClips)
		AddAnimationClip(clip.get());
}

int NFGE::Graphics::Animator::AddAnimationClip(AnimationClip * clipPtr, bool isloop)
{
	ASSERT(mIsInitialize, "[Animator] has to be initialize before use, call Load().");
	mClips.push_back(clipPtr);

	int newSize = static_cast<int>(clipPtr->boneAnimations.size());

	bool hasResize = false;
	for (auto& partialAnimator : mPartials)
	{
		partialAnimator->GetClipsSpeedMutiplayer().emplace_back(1.0f);
		partialAnimator->GetClipsIsLoop().emplace_back(isloop);
		partialAnimator->GetClipsHints().emplace_back();

		auto& blenderPtr = partialAnimator->GetBlender();
		if (newSize > static_cast<int>(blenderPtr->mInnerTransitionPos.size()))
		{
			blenderPtr->mInnerTransitionPos.resize(newSize, {});
			blenderPtr->mInnerTransitionRot.resize(newSize, {});
			blenderPtr->mInnerTransitionScl.resize(newSize, {});
			blenderPtr->mLastMixPos.resize(newSize, {});
			blenderPtr->mLastMixRot.resize(newSize, {});
			blenderPtr->mLastMixScl.resize(newSize, {});
			hasResize = true;
		}
	}

	if (hasResize)
	{
		mCurrentBlendPos.resize(newSize, {});
		mCurrentBlendRot.resize(newSize, {});
		mCurrentBlendScl.resize(newSize, {});
	}


	return static_cast<int>(mClips.size()) - 1;
}

int NFGE::Graphics::Animator::AddPartial(std::string partialName, const std::vector<int>& effectedBone)
{
	ASSERT(mIsInitialize, "[Animator] has to be initialize before use, call Load().");
	mPartialName.emplace_back(std::move(partialName));
	mPartials.emplace_back(std::make_unique<PartialAnimator>(mClips, mPartials, mBoneRegistry, mLastEffectingPartial,effectedBone, mCurrentBlendPos, mCurrentBlendRot, mCurrentBlendScl, static_cast<int>(mPartials.size())));
	mPartials.back()->Adapt(*mPartials[0]);

	return 0;
}

void NFGE::Graphics::Animator::Play(std::string clipName, std::string particalName)
{
	Play(FindClip(clipName), FindPartial(particalName));
}

void NFGE::Graphics::Animator::Play(int clipIndex, int partialId)
{
	mPartials[partialId]->Play(clipIndex);
}

void NFGE::Graphics::Animator::Pause(int partialId)
{
	mPartials[partialId]->Pause();
}

void NFGE::Graphics::Animator::Resume(int partialId)
{
	mPartials[partialId]->Resume();
}

void NFGE::Graphics::Animator::Pause(std::string partialName)
{
	mPartials[FindPartial(partialName)]->Pause();
}

void NFGE::Graphics::Animator::Resume(std::string partialName)
{
	mPartials[FindPartial(partialName)]->Resume();
}

void NFGE::Graphics::Animator::PauseAll()
{
	for (auto& partialAnimator : mPartials)
	{
		partialAnimator->Pause();
	}
}

void NFGE::Graphics::Animator::ResumeAll()
{
	for (auto& partialAnimator : mPartials)
	{
		partialAnimator->Resume();
	}
}

void NFGE::Graphics::Animator::ChangeTo(std::string clipName, float nextClipBlendPercentage, std::string particalName, float(*funcPtr)(float))
{
	ChangeTo(FindClip(clipName), nextClipBlendPercentage, FindPartial(particalName), funcPtr);
}

void NFGE::Graphics::Animator::ChangeTo(int clipIndex, float nextClipBlendPercentage, int particalId, float(*funcPtr)(float))
{
	auto& currentPartial = mPartials[particalId];
	currentPartial->ChangeTo(clipIndex, nextClipBlendPercentage, funcPtr);
}

void NFGE::Graphics::Animator::BindBlendTree(BlendTreeBase* blendTree, int particalId)
{
	auto& currentPartial = mPartials[particalId];
	currentPartial->BindBlendTree(blendTree);
}

int NFGE::Graphics::Animator::FindClip(const std::string& clipName)
{
	int index = 0;
	std::vector<AnimationClip*>::iterator it = std::find_if(mClips.begin(), mClips.end(),
		[&](AnimationClip* clipPtr) {
			++index;
			return clipPtr->name == clipName; 
		});

	if (it == mClips.end())
		return -1;
	else
		//return std::distance(mClips.begin(), it);
		return index - 1;
}

int NFGE::Graphics::Animator::FindPartial(const std::string & clipName)
{
	int index = 0;
	std::vector<std::string>::iterator it = std::find_if(mPartialName.begin(), mPartialName.end(),
		[&](std::string name) {
		++index;
		return clipName == name;
	});

	if (it == mPartialName.end())
		return -1;
	else
		//return std::distance(mClips.begin(), it);
		return index - 1;
}
