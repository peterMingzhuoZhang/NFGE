#include "Precompiled.h"
#include "PathFollowingBehavior.h"

using namespace NFGE;

NFGE::Math::Vector2 AI::PathFollowingBehavior::Calulate(AI::Agent & agent)
{
	if (path.size() == 0)
		return NFGE::Math::Vector2();

	
	//agent.destination = path.back();

	NFGE::Math::Vector2 ret;
	if (path.size() == 1)
		ret = ArriveBehavior::Calulate(agent);
	else
	{
		auto desiredVelocity = NFGE::Math::Normalize(agent.destination - agent.position) * agent.maxSpeed;

		ret = (desiredVelocity - agent.velocity) * agent.mass / 1.0f;	// F = mass * (vel / time);
	}
	
	if (Distance(agent.position, path.back()) < switchPointDistance)
	{
		path.pop_back();
		if (path.size() == 0)
		{
			ret = NFGE::Math::Vector2();
		}
		else
		{
			agent.destination = path.back();
		}
	}
	
	return ret;
	
}