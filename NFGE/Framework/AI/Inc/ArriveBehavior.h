#ifndef INCLUDE_NFGE_AI_ARRIVEBEHAVIOR_H
#define INCLUDE_NFGE_AI_ARRIVEBEHAVIOR_H

#include "SteeringBehavior.h"

namespace NFGE::AI
{
	class ArriveBehavior : public SteeringBehavior
	{
	public:
		NFGE::Math::Vector2 Calulate(AI::Agent& agent) override;
		float arriveDistance = 500.0f;
	};

}

#endif // !INCLUDE_NFGE_AI_ARRIVEBEHAVIOR_H