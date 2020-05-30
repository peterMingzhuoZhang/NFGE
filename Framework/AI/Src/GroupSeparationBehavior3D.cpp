#include "Precompiled.h"
#include "GroupSeparationBehavior3D.h"

using namespace NFGE;

NFGE::Math::Vector3 AI::GroupSeparationBehavior3D::Calulate(AI::Agent3D & agent)
{
	NFGE::Math::Vector3 desiredForce{0.0f, 0.0f, 0.0f};

	for (auto& neighbour : agent.neighborhood)
	{
		NFGE::Math::Vector3 currentDiffVector = agent.position - neighbour->position;
		float currentDist = NFGE::Math::Magnitude(currentDiffVector);
		if (currentDist != 0.0f)
		{
			NFGE::Math::Vector3 currentForce = NFGE::Math::Normalize(currentDiffVector) / currentDist;
			desiredForce += currentForce;
		}
	}

	if ((NFGE::Math::IsZero(desiredForce.x)) && (NFGE::Math::IsZero(desiredForce.y )))
		return NFGE::Math::Vector3{ 0.0f, 0.0f, 0.0f };

	NFGE::Math::Vector3 desiredVelocity = NFGE::Math::Normalize(desiredForce) * agent.maxSpeed;
	return (desiredVelocity - agent.velocity) * agent.mass / 1.0f;	// F = mass * (vel / time);
}
