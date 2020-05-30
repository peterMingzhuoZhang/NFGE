#ifndef INCLUDE_NFGE_AI_GROUPSEPARATIONBEHAVIOR_H
#define INCLUDE_NFGE_AI_GROUPSEPARATIONBEHAVIOR_H

#include "SteeringBehavior.h"

namespace NFGE::AI
{
	class GroupSeparationBehavior : public SteeringBehavior
	{
	public:
		NFGE::Math::Vector2 Calulate(AI::Agent& agent) override;
	};

}

#endif // !INCLUDE_NFGE_AI_GROUPSEPARATIONBEHAVIOR_H