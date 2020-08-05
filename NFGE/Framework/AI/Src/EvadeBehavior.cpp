#include "Precompiled.h"
#include "EvadeBehavior.h"
using namespace NFGE;

NFGE::Math::Vector2 AI::EvadeBehavior::Calulate(AI::Agent & agent)
{
	if (targetAgent == nullptr)
	{
		return NFGE::Math::Vector2();
	}
	float distToTarget = NFGE::Math::Distance(agent.position, targetAgent->position);

	float periSpeed = NFGE::Math::Magnitude(agent.velocity);
	float timeToTarget = periSpeed == 0.0f ? 0.0f : (distToTarget * predictFactor) / periSpeed;

	NFGE::Math::Vector2 evadetDestination = targetAgent->position + targetAgent->velocity * timeToTarget ;

	agent.destination = evadetDestination;

	return FleeBehavior::Calulate(agent);
}