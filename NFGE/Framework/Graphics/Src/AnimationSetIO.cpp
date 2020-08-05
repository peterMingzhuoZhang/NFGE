#include "Precompiled.h"
#include "AnimationSetIO.h"
#include "Keyframe.h"

using namespace NFGE;
using namespace NFGE::Graphics;

bool NFGE::Graphics::AnimationSetIO::Save(const AnimationSet & animationSet, FILE * file)
{
	int totalAnimationClips = static_cast<int>(animationSet.boneAnimationClips.size());
	fprintf_s(file, "Total_AniamtionClip: %d\n", totalAnimationClips);

	for (int animationClipIndex = 0; animationClipIndex < totalAnimationClips; ++animationClipIndex)
	{
		auto& boneAnimationClip = animationSet.boneAnimationClips[animationClipIndex];
		fprintf_s(file, "Bone_Aniamtion_Clip_Name: %s\n", boneAnimationClip->name.c_str());
		
		fprintf_s(file, "Duration: %f\n", boneAnimationClip->duration);
		fprintf_s(file, "Tick_Per_Second: %f\n", boneAnimationClip->ticksPerSecond);

		int totalBoneAnimations = static_cast<int>(boneAnimationClip->boneAnimations.size());
		fprintf_s(file, "Total_Bone_Animation: %d\n", totalBoneAnimations);

		for (int boneAnimaionIndex = 0; boneAnimaionIndex < totalBoneAnimations; ++boneAnimaionIndex)
		{
			auto& boneAnimation = boneAnimationClip->boneAnimations[boneAnimaionIndex];

			int isEmpty = static_cast<int>(boneAnimation == nullptr);
			fprintf_s(file, "IsEmpty: %d\n", isEmpty);
			if (isEmpty == 1)
				continue;
			
			fprintf_s(file, "Bone_Name: %s\n", boneAnimation->boneName.c_str());


			auto& posChnl = boneAnimation->animation->mPositionChannel;
			fprintf_s(file, "Position Channel: \n");
			fprintf_s(file, "Keyframe Count: %d\n", static_cast<int>(posChnl.mKeyFrames.size()));
			for (auto& keyframe : posChnl.mKeyFrames)
			{
				fprintf_s(file, "Value: %f, %f, %f. Time: %f\n", keyframe.value.x, keyframe.value.y, keyframe.value.z, keyframe.time);
			}

			auto& rotChnl = boneAnimation->animation->mRotationChannel;
			fprintf_s(file, "Rotation Channel: \n");
			fprintf_s(file, "Keyframe Count: %d\n", static_cast<int>(rotChnl.mKeyFrames.size()));
			for (auto& keyframe : rotChnl.mKeyFrames)
			{
				fprintf_s(file, "Value: %f, %f, %f, %f. Time: %f\n", keyframe.value.x, keyframe.value.y, keyframe.value.z, keyframe.value.w, keyframe.time);
			}

			auto& sclChnl = boneAnimation->animation->mScaleChannel;
			fprintf_s(file, "Scale Channel: \n");
			fprintf_s(file, "Keyframe Count: %d\n", static_cast<int>(sclChnl.mKeyFrames.size()));
			for (auto& keyframe : sclChnl.mKeyFrames)
			{
				fprintf_s(file, "Value: %f, %f, %f. Time: %f\n", keyframe.value.x, keyframe.value.y, keyframe.value.z, keyframe.time);
			}
		}
	}
	return true;
}

bool NFGE::Graphics::AnimationSetIO::Load(AnimationSet & animationSet, FILE * file)
{
	uint32_t totalAnimationClips;
	fscanf_s(file, "Total_AniamtionClip: %d\n", &totalAnimationClips);
	//animationSet.boneAnimationClips.reserve(totalAnimationClips + animationSet.boneAnimationClips.size()); // Allow Add on exist set

	for (uint32_t animationClipIndex = 0; animationClipIndex < totalAnimationClips; ++animationClipIndex)
	{
		auto& boneAnimationClip = animationSet.boneAnimationClips.emplace_back(std::make_unique<AnimationClip>());	// C++ 17
		
		char buffer_clipName[256];
		fscanf(file, "Bone_Aniamtion_Clip_Name: %s\n", buffer_clipName);
		boneAnimationClip->name = buffer_clipName;

		fscanf_s(file, "Duration: %f\n", &boneAnimationClip->duration);
		fscanf_s(file, "Tick_Per_Second: %f\n", &boneAnimationClip->ticksPerSecond);

		uint32_t totalBoneAnimations;
		fscanf(file, "Total_Bone_Animation: %d\n", &totalBoneAnimations);
		boneAnimationClip->boneAnimations.reserve(totalBoneAnimations);

		

		for (uint32_t boneAnimaionIndex = 0; boneAnimaionIndex < totalBoneAnimations; ++boneAnimaionIndex)
		{
			uint32_t isEmpty;
			fscanf_s(file, "IsEmpty: %d\n", &isEmpty);
			if (isEmpty == 1)
			{
				boneAnimationClip->boneAnimations.emplace_back(nullptr);
				continue;
			}

			float lastKeyframeTime = -1.0f;

			auto& boneAnimation = boneAnimationClip->boneAnimations.emplace_back(std::make_unique<BoneAnimation>());
			boneAnimation->animation = std::make_unique<Animation>();

			char buffer_boneName[256];
			fscanf(file, "Bone_Name: %s\n", buffer_boneName);
			boneAnimation->boneName = buffer_boneName;

			// Load position channel
			auto& posChnl = boneAnimation->animation->mPositionChannel;
			fscanf_s(file, "Position Channel: \n");
			int posKeyCount = 0;
			fscanf_s(file, "Keyframe Count: %d\n", &posKeyCount);
			posChnl.mKeyFrames.reserve(posKeyCount);
			for (int i = 0; i < posKeyCount; ++i)
			{
				auto& keyframe = posChnl.mKeyFrames.emplace_back();
				fscanf_s(file, "Value: %f, %f, %f. Time: %f\n", &keyframe.value.x, &keyframe.value.y, &keyframe.value.z, &keyframe.time);
			}
			if (posChnl.mKeyFrames.back().time > lastKeyframeTime)
				lastKeyframeTime = posChnl.mKeyFrames.back().time;

			// Load rotation channel
			auto& rotChnl = boneAnimation->animation->mRotationChannel;
			fscanf_s(file, "Rotation Channel: \n");
			int rotKeyCount = 0;
			fscanf_s(file, "Keyframe Count: %d\n", &rotKeyCount);
			for (int i = 0; i < rotKeyCount; ++i)
			{
				auto& keyframe = rotChnl.mKeyFrames.emplace_back();
				fscanf_s(file, "Value: %f, %f, %f, %f. Time: %f\n", &keyframe.value.x, &keyframe.value.y, &keyframe.value.z, &keyframe.value.w, &keyframe.time);
			}
			if (rotChnl.mKeyFrames.back().time > lastKeyframeTime)
				lastKeyframeTime = rotChnl.mKeyFrames.back().time;

			// Load scale channel
			auto& sclChnl = boneAnimation->animation->mScaleChannel;
			fscanf_s(file, "Scale Channel: \n");
			int sclKeyCount = 0;
			fscanf_s(file, "Keyframe Count: %d\n", &sclKeyCount);
			for (int i = 0; i < sclKeyCount; ++i)
			{
				auto& keyframe = sclChnl.mKeyFrames.emplace_back();
				fscanf_s(file, "Value: %f, %f, %f. Time: %f\n", &keyframe.value.x, &keyframe.value.y, &keyframe.value.z, &keyframe.time);
			}
			if (sclChnl.mKeyFrames.back().time > lastKeyframeTime)
				lastKeyframeTime = sclChnl.mKeyFrames.back().time;
			boneAnimation->animation->SetLastFrameTime(lastKeyframeTime);
		}
	}
	return true;
}
