#ifndef INCLUDE_NFGE_AI_OBSTACLEAVOIDBEHAVIOR_H
#define INCLUDE_NFGE_AI_OBSTACLEAVOIDBEHAVIOR_H

#include "SteeringBehavior.h"

namespace NFGE::AI
{
	class ObstacleAvoidBehavior : public SteeringBehavior
	{
	public:
		NFGE::Math::Vector2 Calulate(AI::Agent& agent) override;
		NFGE::Math::Vector2 mDebugDrawPosition;
	};

}

#endif // !INCLUDE_NFGE_AI_OBSTACLEAVOIDBEHAVIOR_H
