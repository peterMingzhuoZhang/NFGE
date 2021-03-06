//====================================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) 2019 MingzhuoZhang. All rights reserved.
//====================================================================================================

#ifndef NFGE_PHYSICS_SIMPLEPHYSICSSHAPE_H
#define NFGE_PHYSICS_SIMPLEPHYSICSSHAPE_H

namespace NFGE::Physics
{
	struct Particle;
	class Spring;
	struct PhysicsShape
	{
		std::vector<Particle*>	mVertices;
		std::vector<Spring*>	mEdges;
	};
}

#endif // !NFGE_PHYSICS_SIMPLEPHYSICSSHAPE_H
