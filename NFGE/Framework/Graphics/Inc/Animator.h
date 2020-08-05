#ifndef NFGE_GRAPHICS_ANIMATOR_H
#define NFGE_GRAPHICS_ANIMATOR_H

#include "PartialAnimator.h"


namespace NFGE::Graphics
{
	struct Skeleton;
	class BlendTreeBase;

	class Animator
	{
	public:

		Animator();

		void Load(const Skeleton& skeleton, int PartialCount = 1);

		Animator(const Animator&) = delete;
		Animator& operator=(const Animator &t) = delete;

		void AddAnimationSet(const AnimationSet& animSet);
		int AddAnimationClip(AnimationClip* clipPtr, bool isLoop = false); // Return the index of new added clip
		int AddPartial(std::string partialName, const std::vector<int>& effectedBone);

		//void Play(std::string clipName, int partialId = 0);
		void Play(std::string clipName, std::string particalName = "WholeBody");
		void Play(int clipIndex, int Partial = 0);
		//void Play(int clipIndex, std::string particalName = "WholeBody");

		void Pause(int partialId = -1);
		void Resume(int partialId = -1);
		void Pause(std::string partialName);
		void Resume(std::string partialName);
		void PauseAll();
		void ResumeAll();

		//void ChangeTo(std::string clipName, float nextClipBlendPercentage, int partialId = 0, float(*funcPtr)(float) = NFGE::Math::Ease::EaseNone);
		void ChangeTo(std::string clipName, float nextClipBlendPercentage, std::string particalName = "WholeBody", float(*funcPtr)(float) = NFGE::Math::Ease::EaseNone);
		void ChangeTo(int clipIndex, float nextClipBlendWeight, int particalId = 0, float(*funcPtr)(float) = NFGE::Math::Ease::EaseNone);
		void BindBlendTree(BlendTreeBase* blendTree, int particalId = 0);
		//void ChangeTo(int clipIndex, float nextClipBlendWeight, std::string particalName = "WholeBody", float(*funcPtr)(float) = NFGE::Math::Ease::EaseNone);

		void Update(float deltaTime);

		//void SetClip(std::string clipName, bool isLoop, float speedMutilpyer, int partialId = 0);
		void SetClip(std::string clipName, bool isLoop, float speedMutilpyer, std::string partialName = "WholeBody");
		//void SetClip(int clipId, bool isLoop, float speedMutilpyer, std::string partialName = "WholeBody");
		void SetClip(int clipId, bool isLoop, float speedMutilpyer, int partialId = 0);

		NFGE::Math::Matrix4 GetTranform(int boneIndex, bool& hasAnimation);
		NFGE::Math::Matrix4 GetTranslation(int boneIndex, bool& hasAnimation);
		NFGE::Math::Matrix4 GetRotation(int boneIndex, bool& hasAnimation);
		NFGE::Math::Matrix4 GetScale(int boneIndex, bool& hasAnimation);
	
		//void GetTranform(std::vector<NFGE::Math::Matrix4>& boneMatrix) const;			//|
		//void GetTranslation(std::vector<NFGE::Math::Matrix4>& boneMatrix) const;		//|
		//void GetRotation(std::vector<NFGE::Math::Matrix4>& boneMatrix) const;			//|--- To increase performance by get all boneMatrix in one call, reduce function calling;
		//void GetScale(std::vector<NFGE::Math::Matrix4>& boneMatrix) const;			//|

		int GetClipCount() const { return static_cast<int>(mClips.size()); }
		int GetParticleCount() const { return static_cast<int>(mPartials.size()); }

	private:
		int FindClip(const std::string& clipName);
		int FindPartial(const std::string& clipName);

	private:
		std::vector<AnimationClip*> mClips;
		std::vector<NFGE::Math::Vector3>		mCurrentBlendPos;
		std::vector<NFGE::Math::Quaternion>		mCurrentBlendRot;
		std::vector<NFGE::Math::Vector3>		mCurrentBlendScl;

		std::vector<int> mBoneRegistry; //|--- For each bone to know which [PartialAnimationProcessor] is using.
		std::vector<int> mLastEffectingPartial;

		std::vector<std::string> mPartialName;
		std::vector<std::unique_ptr<PartialAnimator>> mPartials;

		bool mIsInitialize = false;
	};

}


#endif // !NFGE_GRAPHICS_ANIMATOR_H
