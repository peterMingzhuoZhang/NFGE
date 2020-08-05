#include "Precompiled.h"
#include "HideBehavior.h"

using namespace NFGE;

NFGE::Math::Vector2 AI::HideBehavior::Calulate(AI::Agent & agent)
{
	if (obstacles.size() == 0)
	{
		return NFGE::Math::Vector2();
	}

	float minDist = FLT_MAX;
	NFGE::Math::Circle* closeObs = nullptr;
	for (NFGE::Math::Circle& obs : obstacles)
	{
		
		float dist = NFGE::Math::Distance(obs.center, agent.position);
		if (dist < minDist)
		{
			closeObs = &obs;
		}
	}

	NFGE::Math::Vector2 toSafePoint = closeObs->center - targetAgent->position;
	NFGE::Math::Vector2 toSafePointDir = NFGE::Math::Normalize(toSafePoint);
	float length = NFGE::Math::Magnitude(toSafePoint);

	NFGE::Math::Vector2 destination = toSafePointDir * (length + closeObs->radius + stayDistance);

	return ArriveBehavior::Calulate(agent);
}