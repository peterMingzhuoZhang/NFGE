//====================================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) 2019 MingzhuoZhang. All rights reserved.
//====================================================================================================

#ifndef NFGE_GRAPHIC_PARTIALANIMATOR_H
#define NFGE_GRAPHIC_PARTIALANIMATOR_H

#include "Common.h"
#include "AnimationSet.h"

namespace NFGE::Graphics
{
	class BlendTreeBase;
	class PartialAnimator
	{
	public:

		struct Blender
		{
		public:
			Blender(PartialAnimator& animator)
				: mAnimator(&animator)
			{
			}

			void SetBlender(float BlendDuration, float(*funcPtr)(float) = NFGE::Math::Ease::EaseNone);
			void Reset();
			float GetBlendInWeight() const { return funcPtr(mBlendingTimeCounter / mBlendDuration); };
			void GetMixedTransform(int boneIndex, NFGE::Math::Vector3& translate, NFGE::Math::Quaternion& rotation, NFGE::Math::Vector3& scale, bool& hasAnimation);
			void LoadInnerTransition();
			void Update(float deltaTime);

		public:
			PartialAnimator * mAnimator;
			bool mIsActive = false;
			bool mIsMixing = false;
			std::vector<NFGE::Math::Vector3>		mLastMixPos;
			std::vector<NFGE::Math::Quaternion>		mLastMixRot;
			std::vector<NFGE::Math::Vector3>		mLastMixScl;

			float mBlendingTimeCounter;
			size_t mBlendingPositionHint;		// optimazation when retrive animation data
			size_t mBlendingRotationHint;		// optimazation when retrive animation data
			size_t mBlendingScaleHint;			// optimazation when retrive animation data
			float mBlendDuration;
			float(*funcPtr)(float) = NFGE::Math::Ease::EaseNone;

			bool mIsInnerTransitionBlending = false;
			std::vector<NFGE::Math::Vector3>		mInnerTransitionPos;
			std::vector<NFGE::Math::Quaternion>		mInnerTransitionRot;
			std::vector<NFGE::Math::Vector3>		mInnerTransitionScl;
		};


		PartialAnimator(std::vector<AnimationClip*>& allClips, std::vector<std::unique_ptr<PartialAnimator>>& allPartials, std::vector<int>& boneRegistry, std::vector<int>& lastEffectingPartial, const std::vector<int> effectedBone, std::vector<NFGE::Math::Vector3>& currentBlendPos, std::vector<NFGE::Math::Quaternion>& currentBlendRot, std::vector<NFGE::Math::Vector3>& currentBlendScl, int particleId)
			: mClips(allClips)
			, mAllPartials(allPartials)
			, mBoneRegistry(boneRegistry)
			, mLastEffectingPartial(lastEffectingPartial)
			, mParticleId(particleId)
			, mBlender(std::make_unique<Blender>(*this))
			, mCurrentBlendPos(currentBlendPos)
			, mCurrentBlendRot(currentBlendRot)
			, mCurrentBlendScl(currentBlendScl)
		{
			mClipsHints.resize(allClips.size());
			size_t effectedBoneCount = effectedBone.size();
			mEffectedBones.reserve(effectedBoneCount);
			for (auto boneId : effectedBone)
			{
				mEffectedBones.emplace_back(boneId);
			}
		}

		PartialAnimator(const PartialAnimator&) = delete;
		PartialAnimator& operator=(const PartialAnimator& t) = delete;

		void Adapt(const PartialAnimator& other);

		void Play(int clipIndex);

		void Pause() { mIsPause = true; }
		void Resume() { mIsPause = false; }

		void ChangeTo(int clipIndex, float nextClipBlendWeight, float(*funcPtr)(float) = NFGE::Math::Ease::EaseNone);
		void BindBlendTree(BlendTreeBase* blendTree);

		void Update(float deltaTime);

		void SetClip(int clipId, bool isLoop, float speedMutilpyer);

		NFGE::Math::Matrix4 GetTranform(int boneIndex, bool& hasAnimation);
		NFGE::Math::Matrix4 GetTranslation(int boneIndex, bool& hasAnimation);
		NFGE::Math::Matrix4 GetRotation(int boneIndex, bool& hasAnimation);
		NFGE::Math::Matrix4 GetScale(int boneIndex, bool& hasAnimation);

		NFGE::Math::Vector3 GetTranslationValue(int boneIndex, bool& hasAnimation);
		NFGE::Math::Quaternion GetRotationValue(int boneIndex, bool& hasAnimation);
		NFGE::Math::Vector3 GetScaleValue(int boneIndex, bool& hasAnimation);

		//void GetTranform(std::vector<NFGE::Math::Matrix4>& boneMatrix) const;			//|
		//void GetTranslation(std::vector<NFGE::Math::Matrix4>& boneMatrix) const;		//|
		//void GetRotation(std::vector<NFGE::Math::Matrix4>& boneMatrix) const;			//|--- To increase performance by get all boneMatrix in one call, reduce function calling;
		//void GetScale(std::vector<NFGE::Math::Matrix4>& boneMatrix) const;			//|

		std::vector<float>& GetClipsSpeedMutiplayer() { return mClipSpeedMutiplyer; }
		std::vector<bool>& GetClipsIsLoop() { return mIsLoop; }
		std::vector<ClipAniamtionHint>& GetClipsHints() { return mClipsHints; }
		std::unique_ptr<Blender>& GetBlender() { return mBlender; }

		void SyncWith(const PartialAnimator& other);
	private:
		int mParticleId;
		std::vector<std::unique_ptr<PartialAnimator>>& mAllPartials;
		BlendTreeBase* mBlendTree;
		std::vector<AnimationClip*>& mClips;
		std::vector<ClipAniamtionHint> mClipsHints;			// optimazation when retrieve animation data
		std::vector<int>& mBoneRegistry;
		std::vector<int>& mLastEffectingPartial;
		
		std::vector<int> mEffectedBones;

		std::vector<NFGE::Math::Vector3>&		mCurrentBlendPos;
		std::vector<NFGE::Math::Quaternion>&	mCurrentBlendRot;
		std::vector<NFGE::Math::Vector3>&		mCurrentBlendScl;

		std::vector<float> mClipSpeedMutiplyer;
		std::vector<bool> mIsLoop;
		bool mIsPause = true;
		float mCounter;
		int mCurrentClip = 0;
		int mNextClip = -1;
		std::unique_ptr<Blender> mBlender;

	private:
		void AdjustRegistry();
	};
}
#endif // !NFGE_GRAPHIC_PARTIALANIMATOR_H
