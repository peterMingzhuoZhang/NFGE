#ifndef INCLUDE_NFGE_AI_EVADEBEHAVIOR_H
#define INCLUDE_NFGE_AI_EVADEBEHAVIOR_H

#include "FleeBehavior.h"

namespace NFGE::AI
{
	class EvadeBehavior : public FleeBehavior
	{
	public:
		NFGE::Math::Vector2 Calulate(AI::Agent& agent) override;
	//private:
		Agent * targetAgent;
		float predictFactor = 0.5f; //Big the number predict more, less the number predict less
	};

}

#endif // !INCLUDE_NFGE_AI_EVADEBEHAVIOR_H
