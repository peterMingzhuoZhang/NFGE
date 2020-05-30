#ifndef INCLUDE_NFGE_AI_GROUPCOHESIONBEHAVIOR3D_H
#define INCLUDE_NFGE_AI_GROUPCOHESIONBEHAVIOR3D_H

#include "SteeringBehavior3D.h"

namespace NFGE::AI
{
	class GroupCohesionBehavior3D : public SteeringBehavior3D
	{
	public:
		NFGE::Math::Vector3 Calulate(AI::Agent3D& agent) override;
	};

}

#endif // !INCLUDE_NFGE_AI_GROUPCOHESIONBEHAVIOR3D_H
