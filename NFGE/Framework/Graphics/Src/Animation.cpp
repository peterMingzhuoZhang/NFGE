#include "Precompiled.h"
#include "Keyframe.h"
#include "Animation.h"

using namespace NFGE;
using namespace NFGE::Graphics;

NFGE::Math::Matrix4 NFGE::Graphics::Animation::GetAllTransform(float time, size_t* positionHint, size_t* rotationHint, size_t* scaleHint) const
{
	Math::Vector3 translation = GetValue_OnKeyframes(mPositionChannel.mKeyFrames, time, positionHint);
	Math::Quaternion rotation = GetValue_OnKeyframes(mRotationChannel.mKeyFrames, time, rotationHint);
	Math::Vector3 scale = GetValue_OnKeyframes(mScaleChannel.mKeyFrames, time, scaleHint);

	Math::Matrix4 mat = Math::MatrixRotationQuaternion(rotation);
	mat._11 *= scale.x;
	mat._22 *= scale.y;
	mat._33 *= scale.z;
	mat._41 = translation.x;
	mat._42 = translation.y;
	mat._43 = translation.z;
	
	return mat;
	//return GetTransform_OnChannel(mScaleChannel ,time) * GetTransform_OnChannel(mRotationChannel ,time) * GetTransform_OnChannel(mPositionChannel, time);
}

void NFGE::Graphics::Animation::SetEaseMode(float(*funcPtr)(float))
{
	mEaser.funcPtr = funcPtr;
}

NFGE::Math::Matrix4 NFGE::Graphics::Animation::ChannelKeyFrameToMatrix(const Animation_Position_Channel & channel, NFGE::Math::Vector3 value) const
{
	return Math::Translation(value);
}

NFGE::Math::Matrix4 NFGE::Graphics::Animation::ChannelKeyFrameToMatrix(const Animation_Rotation_Channel & channel, NFGE::Math::Quaternion value) const
{
	return Math::MatrixRotationQuaternion(value);
}

NFGE::Math::Matrix4 NFGE::Graphics::Animation::ChannelKeyFrameToMatrix(const Animation_Scale_Channel & channel, NFGE::Math::Vector3 value) const
{
	return Math::Matrix4::sScaling(value);
}

NFGE::Math::Matrix4 NFGE::Graphics::Animation::ChannelKeyFrameToMatrix(const Animation_Color_Channel & channel, NFGE::Math::Vector4 value) const
{
	ASSERT(false, "[NFGE::Graphic::Animation] Trying to get a color value to Matrix!! That is so wrong! Programmer!");
	return Math::Matrix4();
}

NFGE::Math::Quaternion NFGE::Graphics::Animation::InterpolationChannelKeys(const NFGE::Math::Quaternion & v0, const NFGE::Math::Quaternion & v1, float ratio)
{
	return NFGE::Math::Slerp(v0, v1, ratio);
}