#include "Precompiled.h"
#include "WallAvoidBehavior3D.h"
#include "AIWorld3D.h"

using namespace NFGE;

NFGE::Math::Vector3 AI::WallAvoidBehavior3D::Calulate(AI::Agent3D & agent)
{
	NFGE::Math::Vector3 desiredVelocity{0.0f,0.0f,0.0f};

	auto walls = agent.mWorld.GetWalls();
	for (auto& wall : walls)
	{
		for (auto& detector : mDetectors)
		{
			desiredVelocity += ProduceForce(detector, wall, agent);
		}
	}

	if(IsDebugDraw3D)
		RenderDetectors(agent); // Debug Render

	if (NFGE::Math::IsZero(desiredVelocity))
		return {0.0f, 0.0f,0.0f };

	return (desiredVelocity - agent.velocity)  * agent.mass / 1.0f;	// F = mass * (vel / time);
}

NFGE::Math::Vector3 AI::WallAvoidBehavior3D::ProduceForce(const Detector & detector, const NFGE::Math::Plane & wall, AI::Agent3D & agent)
{
	NFGE::Math::Vector3 ret{0.0f,0.0f, 0.0f};
	NFGE::Math::Vector3 intersectPoint;
	
	float realDis = 0.0f;
	NFGE::Math::Ray detectorRayInWorld;
	detectorRayInWorld.org = agent.position;
	detectorRayInWorld.dir = NFGE::Math::Normalize( detector.mAntuna.dir * agent.LocalToWorldMatrix());

	if (!NFGE::Math::Intersect(detectorRayInWorld, wall, realDis))
		return ret;
	if ((realDis < detector.mLength) && (realDis > 0.0f))
	{
		NFGE::Math::Vector3 detectorVec = detectorRayInWorld.dir * realDis;
		auto wallNormal = wall.n;
		intersectPoint = detectorRayInWorld.org + detectorVec;

		float distanceToWall = NFGE::Math::Dot(detectorVec, wallNormal) * -1.0f;

		if (NFGE::Math::IsZero(distanceToWall))
			distanceToWall = 0.001f;

		NFGE::Math::Vector3 rejectVelocity = wallNormal;

		rejectVelocity /= distanceToWall;

		ret = rejectVelocity;

		if (IsDebugDraw3D)
		{
			float penatrDis = NFGE::Math::Dot(detectorRayInWorld.dir * detector.mLength, wallNormal) * -1.0f - distanceToWall;
			NFGE::Graphics::SimpleDraw::AddSphere(intersectPoint, 5.0f, NFGE::Graphics::Colors::White);  // DebugRender
			NFGE::Graphics::SimpleDraw::AddLine(intersectPoint, intersectPoint + wallNormal * penatrDis, NFGE::Graphics::Colors::Gold);// DebugRender
		}
		
	}
	return ret;
}

void AI::WallAvoidBehavior3D::RenderDetectors(AI::Agent3D & agent)
{
	for (auto& detector : mDetectors)
	{
		NFGE::Math::Vector3 from = agent.position + detector.mAntuna.org;
		NFGE::Math::Vector3 to = (detector.mAntuna.dir * detector.mLength * agent.LocalToWorldMatrix()) ;
		NFGE::Graphics::SimpleDraw::AddLine(from, to, NFGE::Graphics::Colors::Purple);
	}
}
