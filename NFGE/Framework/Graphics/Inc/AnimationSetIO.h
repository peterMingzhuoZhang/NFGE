#ifndef NFGE_GRAPHICS_ANIMATIONSETIO_H
#define NFGE_GRAPHICS_ANIMATIONSETIO_H

#include "AnimationSet.h"

namespace NFGE::Graphics
{
	namespace AnimationSetIO
	{
		bool Save(const AnimationSet& animationSet, FILE* file);
		bool Load(AnimationSet& animaitonSet, FILE* file);
	}
}

#endif // !NFGE_GRAPHICS_ANIMATIONSETIO_H
