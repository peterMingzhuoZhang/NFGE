#include "Precompiled.h"
#include "GroupCohesionBehavior3D.h"

using namespace NFGE;

NFGE::Math::Vector3 AI::GroupCohesionBehavior3D::Calulate(AI::Agent3D & agent)
{
	NFGE::Math::Vector3 sumOfNeigborsPos = agent.position;
	for (auto& neighbour : agent.neighborhood)
	{
		sumOfNeigborsPos += neighbour->position;
	}

	NFGE::Math::Vector3 centerOfMass = sumOfNeigborsPos / (float)(agent.neighborhood.size() + 1); // Dont forget agent itself

	NFGE::Math::Vector3 toDestination = centerOfMass - agent.position;

	if ((NFGE::Math::IsZero( toDestination.x)) && (NFGE::Math::IsZero( toDestination.y)))
		return NFGE::Math::Vector3(0.0f, 0.0f, 0.0f);

	NFGE::Math::Vector3 desiredVelocity = NFGE::Math::Normalize(toDestination) * agent.maxSpeed;
	return (desiredVelocity - agent.velocity) * agent.mass / 1.0f;	// F = mass * (vel / time);
}
