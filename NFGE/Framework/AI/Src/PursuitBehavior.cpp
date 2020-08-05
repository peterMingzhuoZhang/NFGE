#include "Precompiled.h"
#include "PursuitBehavior.h"

using namespace NFGE;

NFGE::Math::Vector2 AI::PursuitBehavior::Calulate(AI::Agent & agent)
{
	if (targetAgent == nullptr)
	{
		return NFGE::Math::Vector2();
	}
	float distToTarget = NFGE::Math::Distance(agent.position, targetAgent->position);
 
	float periSpeed = NFGE::Math::Magnitude(agent.velocity);
	float timeToTarget = periSpeed == 0.0f ? 0.0f : (distToTarget * predictFactor) / periSpeed;

	NFGE::Math::Vector2 pursuitDestination = targetAgent->position + targetAgent->velocity * timeToTarget ;

	agent.destination = pursuitDestination;

	return SeekBehavior::Calulate(agent);
}