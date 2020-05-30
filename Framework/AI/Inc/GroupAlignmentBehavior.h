#ifndef INCLUDE_NFGE_AI_GROUPALIGNMENTBEHAVIOR_H
#define INCLUDE_NFGE_AI_GROUPALIGNMENTBEHAVIOR_H

#include "SteeringBehavior.h"

namespace NFGE::AI
{
	class GroupAlignmentBehavior : public SteeringBehavior
	{
	public:
		NFGE::Math::Vector2 Calulate(AI::Agent& agent) override;
	};

}

#endif // !INCLUDE_NFGE_AI_GROUPALIGNMENTBEHAVIOR_H