#include "Precompiled.h"
#include "ObstacleAvoidBehavior.h"
#include "AIWorld.h"

using namespace NFGE::AI;
using namespace NFGE;

NFGE::Math::Vector2 GetCloseIntersectPoint(NFGE::Math::Vector2 localPos, float radius);

NFGE::Math::Vector2 AI::ObstacleAvoidBehavior::Calulate(AI::Agent & agent)
{
	NFGE::Math::Vector2 desiredVelocity;
	float effectRadius = NFGE::Math::Magnitude(agent.velocity);

	float closestObsDist = FLT_MAX;
	NFGE::Math::Vector2 closestObs;
	float closestObsExpandRadius;

	auto Obstacles = agent.mWorld.GetObstacles();
	for (auto& obs: Obstacles)
	{
		NFGE::Math::Vector2 obsLocalPos = NFGE::Math::TransformCoord(obs.center, NFGE::Math::Inverse(agent.LocalToWorld()));
		if ((obsLocalPos.y + obs.radius) < 0.0f)
			continue;

		NFGE::Math::Vector2 obsClosePoint = NFGE::Math::Normalize(-obsLocalPos) * obs.radius + obsLocalPos;
		if ((NFGE::Math::Magnitude(obsClosePoint) < effectRadius)) //|| (obsClosePoint.y < 0))  // check if obsClosePoint is in the current range || agent is inside the obs
		{
			float expandRadius = obs.radius + agent.radius * 0.5f;
			if (NFGE::Math::Abs(obsLocalPos.x) < expandRadius)
			{
				NFGE::Math::Vector2 currentClosePoint = GetCloseIntersectPoint(obsLocalPos, expandRadius);
				NFGE::Math::Vector2 worldPos = NFGE::Math::TransformCoord(currentClosePoint, agent.LocalToWorld());
				
				if (IsDebugDraw2D)
					NFGE::Graphics::SimpleDraw::AddScreenCircle(worldPos, 5.0f, NFGE::Graphics::Colors::White);	// Debug draw!!
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
		return NFGE::Math::Vector2();
	}
	else
	{
		float obsToYaxis = closestObsExpandRadius - NFGE::Math::Abs(closestObs.x);
		
		
		
		NFGE::Math::Vector2 lateralVelocity;
		if (closestObs.x > 0.0f)
		{
			lateralVelocity = NFGE::Math::Vector2(-agent.heading.y, agent.heading.x) / (obsToYaxis);
		}
		else
		{
			lateralVelocity = NFGE::Math::Vector2(agent.heading.y, -agent.heading.x) / (obsToYaxis);
		}
		//lateralVelocity = X::Math::TransformCoord(dirX, agent.LocalToWorld()) * obsToYaxis;

		NFGE::Math::Vector2 breakingVelocity = (-agent.heading)  / (closestObsDist);
		desiredVelocity = NFGE::Math::Normalize(lateralVelocity + breakingVelocity) * agent.maxSpeed;
		return (desiredVelocity - agent.velocity)  * agent.mass / 1.0f;	// F = mass * (vel / time);
	}
}

NFGE::Math::Vector2 GetCloseIntersectPoint(NFGE::Math::Vector2 localPos, float radius)
{
	//--- circle equation: (x - localPos.x) ^ 2 + (y - localPos.y) ^ 2 = radius ^ 2;
	float y1 = (NFGE::Math::Sqrt(radius * radius - localPos.x * localPos.x) + localPos.y);
	float y2 = (-NFGE::Math::Sqrt(radius * radius - localPos.x * localPos.x) + localPos.y);

	return y1 < y2 ? NFGE::Math::Vector2(0.0f, y1) : NFGE::Math::Vector2(0.0f, y2);
}