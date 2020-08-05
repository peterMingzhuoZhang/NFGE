//====================================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) 2019 MingzhuoZhang. All rights reserved.
//====================================================================================================

#ifndef NFGE_PHYSICS_RIGBODY_H
#define NFGE_PHYSICS_RIGBODY_H

#include "RigBone.h"

namespace NFGE::Physics
{

	class RigBody
	{
		RigBody(const char* RigBodyFile, const NFGE::Graphics::Skeleton& skeleton);
	};
}

#endif // !NFGE_PHYSICS_RIGBODY_H
