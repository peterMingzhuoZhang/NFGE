//====================================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) 2019 MingzhuoZhang. All rights reserved.
//====================================================================================================

#ifndef INCLUDED_NFGE_PHYSICS_CONSTRAINTS_H
#define INCLUDED_NFGE_PHYSICS_CONSTRAINTS_H

#include "Particle.h"
#include "RagBody.h"
#include "SimplePhysicsShape.h"

namespace NFGE::Physics
{
	class NFGE::Graphics::SkeletonModel;
	class PhysicsWorld;
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

	class JointRotation : public Constraint
	{
	public:
		JointRotation(PhysicsWorld& world, RagPart* ragPart, NFGE::Graphics::SkeletonModel* skeleton, float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

		void Apply() const override;
		void ApplyFromParent(JointRotation* parent);
		JointRotation* AddJointConstraint(JointRotation* child) { mChildJoint.push_back(child); return mChildJoint.back(); }
		void DebugDraw() const override;
		Matrix4 GetOrientationMatrix() const;
		Matrix4 GetChildTranslationMatrix() const { return mChildTranslationMatrix; }

	protected:

		RagPart* mRagPart;
		PhysicsWorld& mWorld;
		std::vector<JointRotation*> mChildJoint;
		NFGE::Graphics::SkeletonModel* mSkeleton;
		
		NFGE::Math::Vector3 currentFrontVector;
		NFGE::Math::Vector3 currentUpVector;
		NFGE::Math::Vector3 currentRightVector;

		NFGE::Math::Matrix4 mChildTranslationMatrix;

		float mXMin;
		float mXMax;
		float mYMin;
		float mYMax;
		float mZMin;
		float mZMax;

		mutable Math::Vector3 idleBUL;
		mutable Math::Vector3 idleBBL;
		mutable Math::Vector3 idleBBR;
		mutable Math::Vector3 idleFBL;
	};
}

#endif // #ifndef INCLUDED_NFGE_PHYSICS_CONSTRAINTS_H
