#include "Precompiled.h"
#include "GroupSeparationBehavior.h"

using namespace NFGE;

NFGE::Math::Vector2 AI::GroupSeparationBehavior::Calulate(AI::Agent & agent)
{
	NFGE::Math::Vector2 desiredForce{0.0f,0.0f};

	for (auto& neighbour : agent.neighborhood)
	{
		NFGE::Math::Vector2 currentDiffVector = agent.position - neighbour->position;
		float currentDist = NFGE::Math::Magnitude(currentDiffVector);
		if (currentDist != 0.0f)
		{
			NFGE::Math::Vector2 currentForce = NFGE::Math::Normalize(currentDiffVector) / currentDist;
			desiredForce += currentForce;
		}
	}

	if ((NFGE::Math::IsZero(desiredForce.x)) && (NFGE::Math::IsZero(desiredForce.y )))
		return NFGE::Math::Vector2{ 0.0f,0.0f };

	NFGE::Math::Vector2 desiredVelocity = NFGE::Math::Normalize(desiredForce) * agent.maxSpeed;
	return (desiredVelocity - agent.velocity) * agent.mass / 1.0f;	// F = mass * (vel / time);
}
