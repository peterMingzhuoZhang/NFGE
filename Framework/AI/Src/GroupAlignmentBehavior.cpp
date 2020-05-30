#include "Precompiled.h"
#include "GroupAlignmentBehavior.h"

using namespace NFGE;

NFGE::Math::Vector2 AI::GroupAlignmentBehavior::Calulate(AI::Agent & agent)
{
	NFGE::Math::Vector2 sumOfNeigborHeading = agent.heading;
	for (auto& neighbour : agent.neighborhood)
	{
		sumOfNeigborHeading += neighbour->heading;
	}

	NFGE::Math::Vector2 averageHeading = sumOfNeigborHeading / (float)(agent.neighborhood.size() + 1); // Dont forget agent itself

	NFGE::Math::Vector2 desiredVelocity = NFGE::Math::Normalize(averageHeading) * agent.maxSpeed;
	return (desiredVelocity - agent.velocity) * agent.mass / 1.0f;	// F = mass * (vel / time);
}
