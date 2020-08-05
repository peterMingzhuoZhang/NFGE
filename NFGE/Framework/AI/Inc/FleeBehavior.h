#ifndef INCLUDE_NFGE_AI_FLEEBEHAVIOR_H
#define INCLUDE_NFGE_AI_FLEEBEHAVIOR_H

#include "SteeringBehavior.h"

namespace NFGE::AI
{
	class FleeBehavior : public SteeringBehavior
	{
	public:
		NFGE::Math::Vector2 Calulate(AI::Agent& agent) override;
		NFGE::Math::Vector2 panicPoint{ 0.0f,0.0f };
		float panicDistance = 500.0f;
	};

}

#endif // !INCLUDE_NFGE_AI_FLEEBEHAVIOR_H