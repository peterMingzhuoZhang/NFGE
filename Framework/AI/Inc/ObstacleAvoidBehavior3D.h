#ifndef INCLUDE_NFGE_AI_OBSTACLEAVOIDBEHAVIOR3D_H
#define INCLUDE_NFGE_AI_OBSTACLEAVOIDBEHAVIOR3D_H

#include "SteeringBehavior3D.h"

namespace NFGE::AI
{
	class ObstacleAvoidBehavior3D : public SteeringBehavior3D
	{
	public:
		NFGE::Math::Vector3 Calulate(AI::Agent3D& agent) override;
		NFGE::Math::Vector3 mDebugDrawPosition;
	};

}

#endif // !INCLUDE_NFGE_AI_OBSTACLEAVOIDBEHAVIOR3D_H
