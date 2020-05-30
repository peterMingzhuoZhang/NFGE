#include "Precompiled.h"
#include "FleeBehavior.h"

using namespace NFGE::AI;
using namespace NFGE;

NFGE::Math::Vector2 AI::FleeBehavior::Calulate(AI::Agent & agent)
{
	const float panicDistanceSq = panicDistance * panicDistance;

	if (NFGE::Math::DistanceSqr(agent.position, panicPoint) > panicDistanceSq)
	{
		return NFGE::Math::Vector2(0.0f,0.0f);
	}

	auto desiredVelocity = NFGE::Math::Normalize(agent.position - panicPoint) * agent.maxSpeed;

	return  (desiredVelocity - agent.velocity) * agent.mass / 1.0f;	// F = mass * (vel / time);
}