#include "Precompiled.h"
#include "GroupCohesionBehavior.h"

using namespace NFGE;

NFGE::Math::Vector2 AI::GroupCohesionBehavior::Calulate(AI::Agent & agent)
{
	NFGE::Math::Vector2 sumOfNeigborsPos = agent.position;
	for (auto& neighbour : agent.neighborhood)
	{
		sumOfNeigborsPos += neighbour->position;
	}

	NFGE::Math::Vector2 centerOfMass = sumOfNeigborsPos / (float)(agent.neighborhood.size() + 1); // Dont forget agent itself

	NFGE::Math::Vector2 toDestination = centerOfMass - agent.position;

	if ((NFGE::Math::IsZero( toDestination.x)) && (NFGE::Math::IsZero( toDestination.y)))
		return NFGE::Math::Vector2(0.0f, 0.0f);

	NFGE::Math::Vector2 desiredVelocity = NFGE::Math::Normalize(toDestination) * agent.maxSpeed;
	return (desiredVelocity - agent.velocity) * agent.mass / 1.0f;	// F = mass * (vel / time);
}
