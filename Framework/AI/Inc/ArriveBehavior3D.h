#ifndef INCLUDE_NFGE_AI_ARRIVEBEHAVIOR3D_H
#define INCLUDE_NFGE_AI_ARRIVEBEHAVIOR3D_H

#include "SteeringBehavior3D.h"

namespace NFGE::AI
{
	class ArriveBehavior3D : public SteeringBehavior3D
	{
	public:
		NFGE::Math::Vector3 Calulate(AI::Agent3D& agent) override;
		float arriveDistance = 500.0f;
	};

}

#endif // !INCLUDE_NFGE_AI_ARRIVEBEHAVIOR_H