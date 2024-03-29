#ifndef INCLUDE_NFGE_AI_FLEEBEHAVIOR3D_H
#define INCLUDE_NFGE_AI_FLEEBEHAVIOR3D_H

#include "SteeringBehavior3D.h"

namespace NFGE::AI
{
	class FleeBehavior3D : public SteeringBehavior3D
	{
	public:
		NFGE::Math::Vector3 Calulate(AI::Agent3D& agent) override;
		NFGE::Math::Vector3 panicPoint{ 0.0f, 0.0f, 0.0f };
		float panicDistance = 500.0f;
	};

}

#endif // !INCLUDE_NFGE_AI_FLEEBEHAVIOR_H