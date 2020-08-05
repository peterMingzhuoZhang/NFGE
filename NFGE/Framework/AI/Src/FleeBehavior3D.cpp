#include "Precompiled.h"
#include "FleeBehavior3D.h"

using namespace NFGE::AI;
using namespace NFGE;

NFGE::Math::Vector3 AI::FleeBehavior3D::Calulate(AI::Agent3D & agent)
{
	const float panicDistanceSq = panicDistance * panicDistance;

	if (NFGE::Math::DistanceSqr(agent.position, panicPoint) > panicDistanceSq)
	{
		return NFGE::Math::Vector3(0.0f, 0.0f, 0.0f);
	}

	auto desiredVelocity = NFGE::Math::Normalize(agent.position - panicPoint) * agent.maxSpeed;

	return  (desiredVelocity - agent.velocity) * agent.mass / 1.0f;	// F = mass * (vel / time);
}