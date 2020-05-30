#include "Precompiled.h"
#include "SeekBehavior3D.h"

using namespace NFGE::AI;
using namespace NFGE;

NFGE::Math::Vector3 AI::SeekBehavior3D::Calulate(AI::Agent3D & agent)
{
	auto desiredVelocity = NFGE::Math::Normalize(agent.destination - agent.position) * agent.maxSpeed;

	return (desiredVelocity - agent.velocity) * agent.mass / 1.0f;	// F = mass * (vel / time);
}