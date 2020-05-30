//===================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) Mingzhuo Zhang All rights reserved.
//===================================================================================


#ifndef INCLUDE_NFGE_PHYSICS_PARTICLE_H
#define INCLUDE_NFGE_PHYSICS_PARTICLE_H

namespace NFGE::Physics
{
	struct Particle
	{
		Math::Vector3 position = Math::Vector3::Zero();
		Math::Vector3 lastPosition = Math::Vector3::Zero();
		Math::Vector3 acceleration = Math::Vector3::Zero();
		float radius = 1.0f;
		float invMass = 0.0f;
		float bounce = 0.0f;
		float gravityScale = 1.0f;
		bool sleep = false;

		void SetPosition(const Math::Vector3& pos)
		{
			position = pos;
			lastPosition = pos;
		}

		void SetVelocity(const Math::Vector3& velocity)
		{
			lastPosition = position - velocity;
		}
	};
}

#endif // !INCLUDE_NFGE_PHYSICS_PARTICLE_H
