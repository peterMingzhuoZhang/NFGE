#ifndef NFGE_GRAPHIC_SKELETONIO_H
#define NFGE_GRAPHIC_SKELETONIO_H

#include "Skeleton.h"

namespace NFGE::Graphics
{
	namespace SkeletonIO
	{
		bool Save(const Skeleton& mesh, FILE* file);
		bool Load(Skeleton& mesh, FILE* file);
	}
}

#endif // !NFGE_GRAPHIC_SKELETONIO_H
