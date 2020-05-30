#include "Precompiled.h"
#include "GroupAlignmentBehavior3D.h"

using namespace NFGE;

NFGE::Math::Vector3 AI::GroupAlignmentBehavior3D::Calulate(AI::Agent3D & agent)
{
	NFGE::Math::Vector3 sumOfNeigborHeading = agent.heading;
	for (auto& neighbour : agent.neighborhood)
	{
		sumOfNeigborHeading += neighbour->heading;
	}

	NFGE::Math::Vector3 averageHeading = sumOfNeigborHeading / (float)(agent.neighborhood.size() + 1); // Dont forget agent itself

	NFGE::Math::Vector3 desiredVelocity = NFGE::Math::Normalize(averageHeading) * agent.maxSpeed;
	return (desiredVelocity - agent.velocity) * agent.mass / 1.0f;	// F = mass * (vel / time);
}
