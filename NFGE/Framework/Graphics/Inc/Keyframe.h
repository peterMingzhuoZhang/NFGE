#ifndef INCLUDED_NFGE_GRAPHICS_KEYFRAME_H
#define INCLUDED_NFGE_GRAPHICS_KEYFRAME_H

#include "Common.h"

namespace NFGE::Graphics
{
	template<typename T>
	struct Keyframe
	{
		T value;
		float time;
	};
}

#endif // !INCLUDED_NFGE_GRAPHICS_KEYFRAME_H
