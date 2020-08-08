//====================================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) 2019 MingzhuoZhang. All rights reserved.
//====================================================================================================

#include "Precompiled.h"
#include "PhysicsWorld.h"
#include "RigBone.h"

void NFGE::Physics::PhysicsWorld::Initialize(const Settings & settings)
{
	mSettings = settings;
}

void NFGE::Physics::PhysicsWorld::Update(float deltaTime)
{
	mTimer += deltaTime;
	while (mTimer >= mSettings.timeStep)
	{
		mTimer -= mSettings.timeStep;

		AccumulateForces();
		Integrate();
		SatisfyConstraints();
	}
}

void NFGE::Physics::PhysicsWorld::DebugDraw() const
{
	using namespace NFGE::Graphics;

	for (auto p : mParticles)
		SimpleDraw::AddSphere(p->position, p->radius, Colors::Cyan, 4, 2);

	for (auto c : mConstrains)
		c->DebugDraw();

	for (auto o : mOBBs)
		Graphics::SimpleDraw::AddOBB(o, NFGE::Graphics::Colors::BlueViolet);
	
}

NFGE::Physics::Particle* NFGE::Physics::PhysicsWorld::AddParticle(Particle * p)
{
	mParticles.push_back(p); // TODO - this is bad :p, turn this into a pool
	return mParticles.back();
}

NFGE::Physics::Constraint* NFGE::Physics::PhysicsWorld::AddConstraint(Constraint * c)
{
	mConstrains.push_back(c); // TODO - this is bad :p, turn this into a pool
	return mConstrains.back();
}

NFGE::Math::Plane * NFGE::Physics::PhysicsWorld::AddPlane(const NFGE::Math::Plane& plane)
{
	mPlanes.push_back(plane);
	return &mPlanes.back();
}

NFGE::Math::OBB * NFGE::Physics::PhysicsWorld::AddOBB(const NFGE::Math::OBB& obb)
{
	mOBBs.push_back(obb);
	return &mOBBs.back();
}

NFGE::Physics::RigBone * NFGE::Physics::PhysicsWorld::AddRigBone(RigBone * rigBone)
{
	mRigBones.push_back(rigBone);
	return mRigBones.back();
}

NFGE::Physics::PhysicsShape* NFGE::Physics::PhysicsWorld::AddLine(const NFGE::Math::Vector3& p0, const NFGE::Math::Vector3& p1)
{
	const int vertexCount = 2;
	const int edgeCount = 1;

	PhysicsShape* newLine = new PhysicsShape();
	newLine->mVertices.reserve(vertexCount);
	newLine->mEdges.reserve(edgeCount);

	Particle* v0 = newLine->mVertices.emplace_back(AddParticle(new Particle{ p0, p0 }));
	v0->radius = 0.1f;
	v0->invMass = 1.0f;
	Particle* v1 = newLine->mVertices.emplace_back(AddParticle(new Particle{ p1, p1 }));
	v1->radius = 0.1f;
	v1->invMass = 1.0f;

	AddConstraint(newLine->mEdges.emplace_back(new Spring(v0, v1)));

	return mShapes.emplace_back(newLine);
}

NFGE::Physics::PhysicsShape * NFGE::Physics::PhysicsWorld::AddCube(const NFGE::Math::Vector3 & midPosition, float edgeLength)
{
	const int vertexCount = 8;
	const int edgeCount = 24;

	PhysicsShape* newCube = new PhysicsShape();
	newCube->mVertices.reserve(vertexCount);
	newCube->mEdges.reserve(edgeCount);

	float vertexRadius = 0.1f;
	float vertexInveMass = 1.0f;
	float halfLength = edgeLength * 0.5f;
	Particle* v0 = newCube->mVertices.emplace_back(AddParticle(new Particle
		{ 
			{ midPosition.x - halfLength, midPosition.y + halfLength, midPosition.z - halfLength },
			{ midPosition.x - halfLength, midPosition.y + halfLength, midPosition.z - halfLength },
			{ 0.0f,0.0f,0.0f},
			vertexRadius, vertexInveMass
		}));

	Particle* v1 = newCube->mVertices.emplace_back(AddParticle(new Particle
		{
			{ midPosition.x + halfLength, midPosition.y + halfLength, midPosition.z - halfLength },
			{ midPosition.x + halfLength, midPosition.y + halfLength, midPosition.z - halfLength },
			{ 0.0f,0.0f,0.0f },
			vertexRadius, vertexInveMass
		}));

	Particle* v2 = newCube->mVertices.emplace_back(AddParticle(new Particle
		{
			{ midPosition.x - halfLength, midPosition.y - halfLength, midPosition.z - halfLength },
			{ midPosition.x - halfLength, midPosition.y - halfLength, midPosition.z - halfLength },
			{ 0.0f,0.0f,0.0f },
			vertexRadius, vertexInveMass
		}));

	Particle* v3 = newCube->mVertices.emplace_back(AddParticle(new Particle
		{
			{ midPosition.x + halfLength, midPosition.y - halfLength, midPosition.z - halfLength },
			{ midPosition.x + halfLength, midPosition.y - halfLength, midPosition.z - halfLength },
			{ 0.0f,0.0f,0.0f },
			vertexRadius, vertexInveMass
		}));

	Particle* v4 = newCube->mVertices.emplace_back(AddParticle(new Particle
		{
			{ midPosition.x - halfLength, midPosition.y + halfLength, midPosition.z + halfLength },
			{ midPosition.x - halfLength, midPosition.y + halfLength, midPosition.z + halfLength },
			{ 0.0f,0.0f,0.0f },
			vertexRadius, vertexInveMass
		}));

	Particle* v5 = newCube->mVertices.emplace_back(AddParticle(new Particle
		{
			{ midPosition.x + halfLength, midPosition.y + halfLength, midPosition.z + halfLength },
			{ midPosition.x + halfLength, midPosition.y + halfLength, midPosition.z + halfLength },
			{ 0.0f,0.0f,0.0f },
			vertexRadius, vertexInveMass
		}));

	Particle* v6 = newCube->mVertices.emplace_back(AddParticle(new Particle
		{
			{ midPosition.x - halfLength, midPosition.y - halfLength, midPosition.z + halfLength },
			{ midPosition.x - halfLength, midPosition.y - halfLength, midPosition.z + halfLength },
			{ 0.0f,0.0f,0.0f },
			vertexRadius, vertexInveMass
		}));

	Particle* v7 = newCube->mVertices.emplace_back(AddParticle(new Particle
		{
			{ midPosition.x + halfLength, midPosition.y - halfLength, midPosition.z + halfLength },
			{ midPosition.x + halfLength, midPosition.y - halfLength, midPosition.z + halfLength },
			{ 0.0f,0.0f,0.0f },
			vertexRadius, vertexInveMass
		}));

	//		  4	 ______________________ 5  
	//			|\				       \
	//			| \				        \
	//		  6	|  \			         \---->7
	//			\   \_0___________________\ 1  
	//			 \  |					  |
	//			  \ |					  |
	//			  2\|_____________________| 3

	AddConstraint(newCube->mEdges.emplace_back(new Spring(v0, v1)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v1, v3)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v3, v2)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v2, v0)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v0, v4)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v2, v6)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v1, v5)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v3, v7)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v4, v5)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v5, v7)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v7, v6)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v6, v4)));

	AddConstraint(newCube->mEdges.emplace_back(new Spring(v0, v3)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v1, v2)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v4, v2)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v0, v6)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v5, v6)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v4, v7)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v1, v7)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v5, v3)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v4, v1)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v5, v0)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v2, v7)));
	AddConstraint(newCube->mEdges.emplace_back(new Spring(v3, v6)));

	return mShapes.emplace_back(newCube);
}

NFGE::Physics::PhysicsShape * NFGE::Physics::PhysicsWorld::AddPiramid(const NFGE::Math::Vector3 & midPosition, float height)
{
	const int vertexCount = 4;
	const int edgeCount = 6;

	PhysicsShape* newPiramid = new PhysicsShape();
	newPiramid->mVertices.reserve(vertexCount);
	newPiramid->mEdges.reserve(edgeCount);

	float vertexRadius = 0.1f;
	float vertexInveMass = 1.0f;
	float halfLength = height * 0.5f;
	Particle* v0 = newPiramid->mVertices.emplace_back(AddParticle(new Particle
		{
			{ midPosition.x , midPosition.y + 0.5f, midPosition.z },
			{ midPosition.x , midPosition.y + 0.5f, midPosition.z },
			{ 0.0f,0.0f,0.0f },
			vertexRadius, vertexInveMass
		}));

	Particle* v1 = newPiramid->mVertices.emplace_back(AddParticle(new Particle
		{
			{ midPosition.x - halfLength, midPosition.y - halfLength, midPosition.z - halfLength },
			{ midPosition.x - halfLength, midPosition.y - halfLength, midPosition.z - halfLength },
			{ 0.0f,0.0f,0.0f },
			vertexRadius, vertexInveMass
		}));

	Particle* v2 = newPiramid->mVertices.emplace_back(AddParticle(new Particle
		{
			{ midPosition.x + halfLength, midPosition.y - halfLength, midPosition.z - halfLength },
			{ midPosition.x + halfLength, midPosition.y - halfLength, midPosition.z - halfLength },
			{ 0.0f,0.0f,0.0f },
			vertexRadius, vertexInveMass
		}));

	Particle* v3 = newPiramid->mVertices.emplace_back(AddParticle(new Particle
		{
			{ midPosition.x , midPosition.y - halfLength, midPosition.z + halfLength },
			{ midPosition.x , midPosition.y - halfLength, midPosition.z + halfLength },
			{ 0.0f,0.0f,0.0f },
			vertexRadius, vertexInveMass
		}));

	//               /\------0
	//              /  \
	//             /    \
	//            /      \---------3
	//          1/________\--2

	AddConstraint(newPiramid->mEdges.emplace_back(new Spring(v0, v1)));
	AddConstraint(newPiramid->mEdges.emplace_back(new Spring(v1, v2)));
	AddConstraint(newPiramid->mEdges.emplace_back(new Spring(v2, v0)));
	AddConstraint(newPiramid->mEdges.emplace_back(new Spring(v1, v3)));
	AddConstraint(newPiramid->mEdges.emplace_back(new Spring(v2, v3)));
	AddConstraint(newPiramid->mEdges.emplace_back(new Spring(v0, v3)));
	
	return mShapes.emplace_back(newPiramid);
}

void NFGE::Physics::PhysicsWorld::ClearDynamic()
{
	for (auto particle : mParticles)
		delete particle;
	mParticles.clear();

	for (auto c : mConstrains)
		delete c;
	mConstrains.clear();

	for (auto s : mShapes)
		delete s;
	mShapes.clear();
}

void NFGE::Physics::PhysicsWorld::AccumulateForces()
{
	for (auto p : mParticles)
		p->acceleration = mSettings.gravity * p->gravityScale;
}

void NFGE::Physics::PhysicsWorld::Integrate()
{
	const float gamma = 1.0f - mSettings.drag;
	const float timeStepSqr = Math::Sqr(mSettings.timeStep);
	for (auto p : mParticles)
	{
		Math::Vector3 displacement = (p->position - p->lastPosition) * gamma + (p->acceleration * timeStepSqr);
		p->lastPosition = p->position;
		p->position = p->position + displacement;
	}
}

void NFGE::Physics::PhysicsWorld::SatisfyConstraints()
{
	for (int i = 0; i < mSettings.iterations; i++)
	{
		for (auto c : mConstrains)
			c->Apply();
	}

	for (auto rigBone : mRigBones)
	{// Mark rigBone that is effect by physics
		rigBone->mIsDominateByPhysics = false;
	}

	for (auto plane : mPlanes)
	{
		for (auto rigBone : mRigBones)
		{// Mark rigBone that is effect by physics
			if (rigBone->mIsDominateByPhysics)
				break;
			for (auto p : rigBone->mParticles)
			{
				if (Math::Dot(p->position, plane.n) <= plane.d		// Detect is the position is on the negative side of the plane.
					&& Math::Dot(p->lastPosition, plane.n) > plane.d)	// Detect is the lastPosition is on the positive side(different side with [position]) of plane .
				{
					rigBone->mIsDominateByPhysics = true;
					break;
				}
				rigBone->mIsDominateByPhysics = false;
			}
		}

		for (auto p : mParticles)
		{
			if (Math::Dot(p->position, plane.n) <= plane.d		// Detect is the position is on the negative side of the plane.
				&& Math::Dot(p->lastPosition, plane.n) > plane.d)	// Detect is the lastPosition is on the positive side(different side with [position]) of plane .
			{//Fliping velocity
				auto velocity = p->position - p->lastPosition;
				auto velocityPerpendicular = plane.n * Math::Dot(velocity, plane.n);
				auto velocityParallel = velocity - velocityPerpendicular;												// Get new velocity by flip the perpendicular velocity
				auto newVelocity = (velocityParallel * (1.0f - mSettings.drag)) - (velocityPerpendicular * p->bounce);
				p->SetPosition(p->position - velocityPerpendicular);
				p->SetVelocity(newVelocity);
			}
		}
	}

	for (auto obb : mOBBs)
	{
		for (auto rigBone : mRigBones)
		{// Mark rigBone that is effect by physics
			if (rigBone->mIsDominateByPhysics)
				break;
			for (auto p : rigBone->mParticles)
			{
				if (Math::Intersect(p->position, obb))
				{
					rigBone->mIsDominateByPhysics = true;
					break;
				}
			}
		}
		for (auto p : mParticles)
		{
			if (isnan(p->position.x))
				continue;

			if (Math::Intersect(p->position, obb))
			{
				bool isParticleInObb = false;
				auto velocity = p->position - p->lastPosition;

				Math::Vector3 direction;
				if (velocity == Math::Vector3::Zero())
					direction = Math::Normalize(obb.center - p->position);
				else
					direction = Math::Normalize(velocity);

				Math::Ray ray;
				if (Math::Intersect(p->lastPosition, obb))
				{
					auto c2P = (p->position - obb.center);

					float miniDistance = FLT_MAX;
					NFGE::Math::Vector3 rayDir = NFGE::Math::Normalize(c2P);;

					ray = { p->position + ( rayDir * (obb.extend.x + obb.extend.y + obb.extend.z)), -rayDir };
					isParticleInObb = true;
				}
				else
					ray = { p->lastPosition, direction };

				Math::Vector3 point, normal;
				Math::GetContactPoint(ray, obb, point, normal);

				auto velocityPerpendicular = normal * Math::Dot(velocity, normal);
				auto velocityParallel = velocity - velocityPerpendicular;												// Get new velocity by flip the perpendicular velocity
				auto newVelocity = (velocityParallel * (1.0f - mSettings.drag)) - (velocityPerpendicular * p->bounce);
				if (isParticleInObb)
					p->SetPosition(point);
				else
					p->SetPosition(p->position - velocityPerpendicular);
				p->SetVelocity(newVelocity);
			}
		}
	}
}
