#ifndef INCLUDED_NFGE_GRAPHICS_SKELETON_H
#define INCLUDED_NFGE_GRAPHICS_SKELETON_H

#include "Bone.h"

namespace NFGE::Graphics
{
	struct Skeleton
	{
		Bone* root = nullptr;
		std::vector<std::unique_ptr<Bone>> bones;
	};
}
#endif // !INCLUDED_NFGE_GRAPHICS_SKELETON_H
