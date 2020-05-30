#include "Precompiled.h"
#include "ArriveBehavior3D.h"

using namespace NFGE::AI;
using namespace NFGE;

NFGE::Math::Vector3 AI::ArriveBehavior3D::Calulate(AI::Agent3D & agent)
{
	NFGE::Math::Vector3 toDestination = agent.destination - agent.position;
	float dist = NFGE::Math::Distance(agent.position, agent.destination);

	float speed = NFGE::Math::Sqrt(2.0f * (agent.maxSpeed) * dist);
	
	if (dist <= 1.0f)
	{
		agent.velocity = NFGE::Math::Vector3(0.0f, 0.0f, 0.0f);
		return agent.velocity;
	}

	NFGE::Math::Vector3 desiredVelocity = NFGE::Math::Normalize(toDestination) * speed;// agent.maxSpeed;

	return (desiredVelocity - agent.velocity) * (arriveDistance / dist)  * agent.mass / 1.0f;	// F = mass * (vel / time);
}
