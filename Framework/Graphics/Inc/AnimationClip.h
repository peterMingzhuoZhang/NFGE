#ifndef INCLUDED_NFGE_GRAPHICS_ANIMATIONCLIP_H
#define INCLUDED_NFGE_GRAPHICS_ANIMATIONCLIP_H

#include "Animation.h"

namespace NFGE::Graphics
{
	struct AnimationClip
	{
		bool GetTransform(float time, uint32_t boneIndex, Math::Matrix4& transform) const;

		std::string name;
		float duration;
		float ticksPerSecond = 0.0f;
		BoneAnimations boneAnimations;
	};
	struct ClipAniamtionHint
	{
		size_t positionHint = 0;
		size_t rotationHint = 0;
		size_t scaleHint = 0;
	};
}
#endif // !INCLUDED_NFGE_GRAPHICS_ANIMATIONCLIP_H
