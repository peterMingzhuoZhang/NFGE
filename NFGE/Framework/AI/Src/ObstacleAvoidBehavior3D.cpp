#include "Precompiled.h"
#include "ObstacleAvoidBehavior3D.h"
#include "AIWorld3D.h"


using namespace NFGE::AI;
using namespace NFGE;

NFGE::Math::Vector3 GetCloseIntersectPoint(NFGE::Math::Vector3 localPos, float radius);

NFGE::Math::Vector3 AI::ObstacleAvoidBehavior3D::Calulate(AI::Agent3D & agent)
{
	NFGE::Math::Vector3 desiredVelocity;
	float effectRadius = NFGE::Math::Magnitude(agent.velocity);

	float closestObsDist = FLT_MAX;
	NFGE::Math::Vector3 closestObs;
	float closestObsExpandRadius;

	auto Obstacles = agent.mWorld.GetObstacle3Ds();
	for (auto& obs: Obstacles)
	{
		NFGE::Math::Vector3 obsLocalPos = obs.center * NFGE::Math::Inverse(agent.LocalToWorldMatrix());
		if ((obsLocalPos.y + obs.radius) < 0.0f)
			continue;

		NFGE::Math::Vector3 obsClosePoint = NFGE::Math::Normalize(-obsLocalPos) * obs.radius + obsLocalPos;
		if ((NFGE::Math::Magnitude(obsClosePoint) < effectRadius)) //|| (obsClosePoint.y < 0))  // check if obsClosePoint is in the current range || agent is inside the obs
		{
			float expandRadius = obs.radius + agent.radius * 0.5f;
			if (NFGE::Math::Abs(obsLocalPos.x) < expandRadius)
			{
				NFGE::Math::Vector3 currentClosePoint = GetCloseIntersectPoint(obsLocalPos, expandRadius);
				NFGE::Math::Vector3 worldPos = currentClosePoint * agent.LocalToWorldMatrix();
				
				if (IsDebugDraw3D)
					NFGE::Graphics::SimpleDraw::AddSphere(worldPos, 5.0f, NFGE::Graphics::Colors::White);	// Debug draw!!
				mDebugDrawPosition = worldPos;																	// For outside debug draw

				if (closestObsDist > currentClosePoint.y)
				{
					closestObs = obsLocalPos;
					closestObsExpandRadius = expandRadius;
					closestObsDist = currentClosePoint.y;
				}
			}
		}
	}

	if (closestObsDist == FLT_MAX)
	{
		return NFGE::Math::Vector3();
	}
	else
	{
		float obsCenterToAgent_Sqr = NFGE::Math::MagnitudeSqr(closestObs);
		float obsCenterToYAxis = NFGE::Math::Sqrt(obsCenterToAgent_Sqr - closestObs.y * closestObs.y);
		float obsToYaxis = closestObsExpandRadius - obsCenterToYAxis;
		NFGE::Math::Vector3 agentToObs = closestObs - agent.position;
		
		NFGE::Math::Vector3 worldHeadingAxis_X = NFGE::Math::Normalize(NFGE::Math::Cross(agent.heading, (agentToObs)));
		NFGE::Math::Vector3 worldHeadingAxis_Z = NFGE::Math::Normalize(NFGE::Math::Cross(agent.heading, worldHeadingAxis_X));
		
		NFGE::Math::Vector3 lateralDir = NFGE::Math::Normalize(worldHeadingAxis_X * NFGE::Math::Dot(agentToObs, worldHeadingAxis_X)+ worldHeadingAxis_Z * NFGE::Math::Dot(agentToObs, worldHeadingAxis_Z));
		NFGE::Math::Vector3 lateralVelocity{};
		lateralVelocity = lateralDir / (obsToYaxis);
		//lateralVelocity = X::Math::TransformCoord(dirX, agent.LocalToWorld()) * obsToYaxis;

		NFGE::Math::Vector3 breakingVelocity = (-agent.heading)  / (closestObsDist);
		desiredVelocity = NFGE::Math::Normalize(lateralVelocity + breakingVelocity) * agent.maxSpeed;
		return (desiredVelocity - agent.velocity)  * agent.mass / 1.0f;	// F = mass * (vel / time);
	}
}

NFGE::Math::Vector3 GetCloseIntersectPoint(NFGE::Math::Vector3 localPos, float radius)
{
	//--- circle equation: (x - localPos.x) ^ 2 + (y - localPos.y) ^ 2 = radius ^ 2;
	float y1 = (NFGE::Math::Sqrt(radius * radius - localPos.x * localPos.x) + localPos.y);
	float y2 = (-NFGE::Math::Sqrt(radius * radius - localPos.x * localPos.x) + localPos.y);

	return y1 < y2 ? NFGE::Math::Vector3(0.0f, y1, 0.0f) : NFGE::Math::Vector3(0.0f, y2, 0.0f);
}