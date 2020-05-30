#include "Precompiled.h"
#include "WallAvoidBehavior.h"
#include "AIWorld.h"

using namespace NFGE;

NFGE::Math::Vector2 AI::WallAvoidBehavior::Calulate(AI::Agent & agent)
{
	NFGE::Math::Vector2 desiredVelocity{0.0f,0.0f};

	auto walls = agent.mWorld.GetWalls();
	for (auto& wall : walls)
	{
		for (auto& detector : mDetectors)
		{
			desiredVelocity += ProduceForce(detector, wall, agent);
		}
	}

	if(IsDebugDraw2D)
		RenderDetectors(agent); // Debug Render

	if (NFGE::Math::IsZero(desiredVelocity))
		return { 0.0f,0.0f };

	return (desiredVelocity - agent.velocity)  * agent.mass / 1.0f;	// F = mass * (vel / time);
}

NFGE::Math::Vector2 AI::WallAvoidBehavior::ProduceForce(const Detector & detector, const NFGE::Math::LineSegment & wall, AI::Agent & agent)
{
	NFGE::Math::Vector2 ret{0.0f,0.0f};
	NFGE::Math::Vector2 intersectPoint;
	auto detectorDir = detector.mTilt == 0.0f ? agent.heading : NFGE::Math::Rotate(agent.heading, detector.mTilt);
	NFGE::Math::LineSegment detectorLine{ agent.position + detectorDir * detector.mNear, agent.position + detectorDir * detector.mFar };
	if (NFGE::Math::Intersect(wall, detectorLine, intersectPoint))
	{
		auto wallDir = wall.to - wall.from;
		auto wallNormal_0 = NFGE::Math::Normalize({ -wallDir.y,wallDir.x });
		auto wallNormal_1 = NFGE::Math::Normalize({ wallDir.y,-wallDir.x });
		auto usingNormal = wallNormal_0;

		auto DetectorDir = intersectPoint - detectorLine.from;

		float distanceToWall = NFGE::Math::Dot(DetectorDir, wallNormal_0);
		float distanceToPoint = NFGE::Math::Distance(agent.position, intersectPoint);
		float penatrDis = NFGE::Math::Distance(detectorLine.to, intersectPoint);

		if (distanceToWall > 0)
			usingNormal = wallNormal_1;
		if (NFGE::Math::IsZero(distanceToWall))
			distanceToWall = 0.001f;

		NFGE::Math::Vector2 lateralVelocity;
		if (NFGE::Math::Dot(NFGE::Math::Normalize(intersectPoint - agent.position), {usingNormal.y, -usingNormal.x}) > 0.0f)
		{
			lateralVelocity = NFGE::Math::Vector2(-agent.heading.y, agent.heading.x) ;
		}
		else
		{
			lateralVelocity = NFGE::Math::Vector2(agent.heading.y, -agent.heading.x);
		}

		lateralVelocity /= NFGE::Math::Dot(NFGE::Math::Normalize(DetectorDir), -usingNormal);

		ret = NFGE::Math::Normalize(usingNormal+ lateralVelocity) * agent.maxSpeed * penatrDis / NFGE::Math::Abs(distanceToWall);

		if (IsDebugDraw2D)
		{
			NFGE::Graphics::SimpleDraw::AddScreenCircle(intersectPoint, 5.0f, NFGE::Graphics::Colors::White);  // DebugRender
			NFGE::Graphics::SimpleDraw::AddScreenLine(intersectPoint, intersectPoint + usingNormal * 200.0f / NFGE::Math::Abs(distanceToWall), NFGE::Graphics::Colors::Gold);// DebugRender
			NFGE::Graphics::SimpleDraw::AddScreenLine(agent.position, agent.position + lateralVelocity * 200.0f / NFGE::Math::Abs(distanceToWall), NFGE::Graphics::Colors::Gold);// DebugRender
		}
		
	}
	return ret;
}

void AI::WallAvoidBehavior::RenderDetectors(AI::Agent & agent)
{
	for (auto& detector : mDetectors)
	{
		auto detectorDir = detector.mTilt == 0.0f ? agent.heading : NFGE::Math::Rotate(agent.heading, detector.mTilt);
		NFGE::Math::LineSegment detectorLine{ agent.position + detectorDir * detector.mNear, agent.position + detectorDir * detector.mFar };
		NFGE::Graphics::SimpleDraw::AddScreenLine(detectorLine.from, detectorLine.to, NFGE::Graphics::Colors::Yellow);
	}
}
