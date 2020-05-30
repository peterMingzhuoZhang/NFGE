#include "Precompiled.h"
#include "SeekBehavior.h"

using namespace NFGE::AI;
using namespace NFGE;

NFGE::Math::Vector2 AI::SeekBehavior::Calulate(AI::Agent & agent)
{
	auto desiredVelocity = NFGE::Math::Normalize(agent.destination - agent.position) * agent.maxSpeed;

	return (desiredVelocity - agent.velocity) * agent.mass / 1.0f;	// F = mass * (vel / time);
}