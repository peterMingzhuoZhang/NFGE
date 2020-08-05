#ifndef INCLUDED_NFGE_GRAPHICS_BONE_H
#define INCLUDED_NFGE_GRAPHICS_BONE_H

#include "Common.h"

namespace NFGE::Graphics
{
	const int MaxBoneCount = 128;

	struct Bone
	{
		std::string name;
		int index;

		Bone* parent = nullptr;
		int parentIndex = -1;

		std::vector<Bone*> children;
		std::vector<int> childIndices;

		Math::Matrix4 toParentTransform = Math::Matrix4::sIdentity();
		Math::Matrix4 offsetTransform = Math::Matrix4::sIdentity();
	};
}
#endif // !INCLUDED_NFGE_GRAPHICS_BONE_H
