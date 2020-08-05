//====================================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) 2019 MingzhuoZhang. All rights reserved.
//====================================================================================================

#ifndef INCLUDE_NFGE_PHYSICS_PHYSICSWORLD_H
#define INCLUDE_NFGE_PHYSICS_PHYSICSWORLD_H

#include "Particle.h"
#include "Constraints.h"
#include "SimplePhysicsShape.h"

namespace NFGE::Physics
{
	struct RigBone;
	struct Settings
	{
		Math::Vector3 gravity = Math::Vector3{ 0.0f,-9.8f,0.0f };
		float timeStep = 1.0f / 60.0f;
		float drag = 0.0f;
		int iterations = 1;
	};
	class PhysicsWorld
	{
	public:
		void Initialize(const Settings& settings);

		void Update(float deltaTime);
		void DebugDraw() const;

		Particle* AddParticle(Particle* p);
		Constraint* AddConstraint(Constraint* c);
		NFGE::Math::Plane* AddPlane(const NFGE::Math::Plane& plane); // TODO: Change to construct variable in future, when using object pool.
		NFGE::Math::OBB* AddOBB(const NFGE::Math::OBB& obb); // TODO: Change to construct variable in future, when using object pool.
		RigBone* AddRigBone(RigBone* rigBone);

		PhysicsShape* AddLine(const NFGE::Math::Vector3& p0, const NFGE::Math::Vector3& p1);
		PhysicsShape* AddCube(const NFGE::Math::Vector3& midPosition, float edgeLength);
		PhysicsShape* AddPiramid(const NFGE::Math::Vector3& midPosition, float edgeLength);



		void ClearDynamic();

	private:
		void AccumulateForces();
		void Integrate();
		void SatisfyConstraints();

		std::vector<Particle*> mParticles;
		std::vector<Constraint*> mConstrains;
		std::vector<NFGE::Math::Plane> mPlanes;
		std::vector<NFGE::Math::OBB> mOBBs;
		std::vector<RigBone*> mRigBones;
		std::vector<PhysicsShape*> mShapes;

		Settings mSettings;
		float mTimer = 0.0f;

	};
}

#endif // !INCLUDE_NFGE_PHYSICS_PHYSICSWORLD_H
