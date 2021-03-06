//====================================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) 2019 MingzhuoZhang. All rights reserved.
//====================================================================================================

#ifndef NFGE_GRAPHICS_ANIMATIONSET_H
#define NFGE_GRAPHICS_ANIMATIONSET_H

#include "AnimationClip.h"

namespace NFGE::Graphics
{
	struct AnimationSet
	{
		std::vector<std::unique_ptr<AnimationClip>> boneAnimationClips;
	};
}

#endif // !NFGE_GRAPHICS_ANIMATIONSET_H
