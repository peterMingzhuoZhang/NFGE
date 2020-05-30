#ifndef INCLUDE_NFGE_AI_INTERPOSEBEHAVIOR_H
#define INCLUDE_NFGE_AI_INTERPOSEBEHAVIOR_H

#include "ArriveBehavior.h"

namespace NFGE::AI
{
	class InterposeBehavior : public ArriveBehavior
	{
	public:
		NFGE::Math::Vector2 Calulate(AI::Agent& agent) override;
	private:
		Agent* target1;
		Agent* target2;
	};

}

#endif // !INCLUDE_NFGE_AI_INTERPOSEBEHAVIOR_H