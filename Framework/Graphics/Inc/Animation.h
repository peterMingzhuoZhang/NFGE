//====================================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) 2019 MingzhuoZhang. All rights reserved.
//====================================================================================================

#ifndef INCLUDED_NFGE_GRAPHICS_ANIMATION_H
#define INCLUDED_NFGE_GRAPHICS_ANIMATION_H

#include "Common.h"
#include "Keyframe.h"

namespace NFGE::Graphics
{
	class Animation
	{
	public:
		static NFGE::Math::Quaternion InterpolationChannelKeys(const NFGE::Math::Quaternion& v0, const NFGE::Math::Quaternion& v1, float ratio);
		template<typename T>
		static T InterpolationChannelKeys(const T& v0, const T& v1, float ratio)
		{
			return Lerp(v0, v1, ratio);
		}
	public:
		template<typename T>
		using Keyframes = std::vector<Keyframe<T>>;
		struct Animation_Position_Channel { std::vector<Keyframe<NFGE::Math::Vector3>> mKeyFrames; };
		struct Animation_Rotation_Channel { std::vector<Keyframe<NFGE::Math::Quaternion>> mKeyFrames; };
		struct Animation_Scale_Channel { std::vector<Keyframe<NFGE::Math::Vector3>> mKeyFrames; };
		struct Animation_Color_Channel { std::vector<Keyframe<NFGE::Math::Vector4>> mKeyFrames; };

		template<typename ChannelType>
		NFGE::Math::Matrix4 GetTransform_OnChannel(const ChannelType& channel ,float time) const
		{
			size_t channelSize = channel.mKeyFrames.size();
			if (channelSize == 0)
				return NFGE::Math::Matrix4::sIdentity();
			for (size_t i = 0; i + 1 < channelSize; i++)
			{
				if ((time >= channel.mKeyFrames[i].time) && (time < channel.mKeyFrames[i + 1].time))
				{
					float timePass = time - channel.mKeyFrames[i].time;
					float keyFrameDuration = channel.mKeyFrames[i + 1].time - channel.mKeyFrames[i].time;
					float ratio = timePass / keyFrameDuration;
					ratio = mEaser.easing(time, ratio);
					return ChannelKeyFrameToMatrix(channel, Animation::InterpolationChannelKeys(channel.mKeyFrames[i].value, channel.mKeyFrames[i + 1].value, ratio));
				}
			}

			return ChannelKeyFrameToMatrix(channel, channel.mKeyFrames[channelSize - 1].value);
		}
		template<typename T>
		T GetValue_OnKeyframes(const Keyframes<T>& keyframes, float time, size_t* hint = nullptr) const
		{
			size_t frameSize = keyframes.size();
			if (frameSize == 0)
				return T();
			
			size_t startFrame = 0;
			if (hint == nullptr)
				hint = &startFrame;
				
			for (size_t i = *hint; i + 1 < frameSize; i++)
			{
				if (i == frameSize - 1)
				{
					*hint = 0;
					return keyframes[i].value;
				}
				if ((time >= keyframes[i].time) && (time < keyframes[i + 1].time))
				{
					*hint = i;
					float timePass = time - keyframes[i].time;
					float keyFrameDuration = keyframes[i + 1].time - keyframes[i].time;
					float ratio = timePass / keyFrameDuration;
					ratio = mEaser.easing(time, ratio);
					return Animation::InterpolationChannelKeys(keyframes[i].value, keyframes[i + 1].value, ratio);
				}
			}

			*hint = 0;
			return keyframes[frameSize - 1].value;
		}

		NFGE::Math::Matrix4 GetAllTransform(float time, size_t* positionHint = nullptr, size_t* rotationHint = nullptr, size_t* scaleHint = nullptr) const;
		void SetEaseMode(float(*funcPtr)(float));
		
		// - Getter & Setter
		float GetLastFrameTime() const { return mLastFrameTime; };
		void SetLastFrameTime(float lastFrameTime) { mLastFrameTime = lastFrameTime; };
	public:
		Animation_Position_Channel	mPositionChannel;
		Animation_Rotation_Channel	mRotationChannel;
		Animation_Scale_Channel		mScaleChannel;
		Animation_Color_Channel		mColorChannel;
	private:
		friend class AnimationBuilder;
		friend class AnimationIO;

		NFGE::Math::EaseMachine mEaser;
		float mLastFrameTime = 0.0f;
	private:
		NFGE::Math::Matrix4 ChannelKeyFrameToMatrix(const Animation_Position_Channel& channel, NFGE::Math::Vector3 value) const;
		NFGE::Math::Matrix4 ChannelKeyFrameToMatrix(const Animation_Rotation_Channel& channel, NFGE::Math::Quaternion value) const;
		NFGE::Math::Matrix4 ChannelKeyFrameToMatrix(const Animation_Scale_Channel& channel, NFGE::Math::Vector3 value) const;
		NFGE::Math::Matrix4 ChannelKeyFrameToMatrix(const Animation_Color_Channel& channel, NFGE::Math::Vector4 value) const;
	};

	class BoneAnimation
	{
	public:
		std::string boneName;
		std::unique_ptr<Animation> animation;
	};

	using BoneAnimations = std::vector<std::unique_ptr<BoneAnimation>>;
}

#endif // !INCLUDED_NFGE_GRAPHICS_ANIMATION_H
