#include "Precompiled.h"
#include "ArriveBehavior.h"

using namespace NFGE::AI;
using namespace NFGE;

NFGE::Math::Vector2 AI::ArriveBehavior::Calulate(AI::Agent & agent)
{
	NFGE::Math::Vector2 toDestination = agent.destination - agent.position;
	float dist = NFGE::Math::Distance(agent.position, agent.destination);

	float speed = NFGE::Math::Sqrt(2.0f * (agent.maxSpeed) * dist);

	if (dist <= 1.0f)
	{
		agent.velocity = NFGE::Math::Vector2(0.0f,0.0f);
		return agent.velocity;
	}

	NFGE::Math::Vector2 desiredVelocity = NFGE::Math::Normalize(toDestination) * speed;

	return (desiredVelocity - agent.velocity) * (arriveDistance / dist )  * agent.mass / 1.0f;	// F = mass * (vel / time);
}
