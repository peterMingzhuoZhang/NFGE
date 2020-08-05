#ifndef INCLUDE_NFGE_AI_HIDEBEHAVIOR_H
#define INCLUDE_NFGE_AI_HIDEBEHAVIOR_H

#include "ArriveBehavior.h"

namespace NFGE::AI
{
	class HideBehavior : public ArriveBehavior
	{
	public:
		NFGE::Math::Vector2 Calulate(AI::Agent& agent) override;
	private:
		Agent * targetAgent;
		std::list<NFGE::Math::Circle> obstacles;
		float stayDistance;
	};

}

#endif // !INCLUDE_NFGE_AI_HIDEPOSEBEHAVIOR_H