//====================================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) 2019 MingzhuoZhang. All rights reserved.
//====================================================================================================

#ifndef INCLUDED_NFGE_PHYSICS_CONSTRAINTS_H
#define INCLUDED_NFGE_PHYSICS_CONSTRAINTS_H

#include "Particle.h"
#include "SimplePhysicsShape.h"

namespace NFGE::Physics
{
	class Constraint
	{
	public:
		virtual ~Constraint() = default;

		virtual void Apply() const = 0;
		virtual void DebugDraw() const {}
	};

	class Fixed : public Constraint
	{
	public:
		Fixed(Particle* p);
		Fixed(Particle* p, const Math::Vector3& position);

		void Apply() const override;
		void DebugDraw() const override;

	protected:
		Particle* mParticle;
		Math::Vector3 mPosition;
	};

	class Spring : public Constraint
	{
	public:
		Spring(Particle* a, Particle* b, float restLength = 0.0f);

		void Apply() const override;
		void DebugDraw() const override;

	protected:
		Particle* mParticleA;
		Particle* mParticleB;
		float mRestLength;
	};

	class Arriver : public Constraint
	{
	public:
		Arriver(Particle* a, Particle* b, float resetLength = 0.1f);// a is the not moving relatively, arrive to a

		void Apply()const override;
		void DebugDraw() const override;

	protected:
		Particle* mParticleA;
		Particle* mParticleB;
		float mRestLength;
	};
}

#endif // #ifndef INCLUDED_NFGE_PHYSICS_CONSTRAINTS_H
