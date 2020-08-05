#ifndef INCLUDE_NFGE_AI_SEEKBEHAVIOR_H
#define INCLUDE_NFGE_AI_SEEKBEHAVIOR_H

#include "SteeringBehavior.h"

namespace NFGE::AI
{
	class SeekBehavior : public SteeringBehavior
	{
	public:
		virtual NFGE::Math::Vector2 Calulate(AI::Agent& agent) override;
		
	};

}

#endif // !INCLUDE_NFGE_AI_SEEKBEHAVIOR_H
