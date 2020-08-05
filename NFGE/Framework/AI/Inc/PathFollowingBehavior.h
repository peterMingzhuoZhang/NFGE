#ifndef INCLUDE_NFGE_AI_PATHFOLLOWINGBEHAVIOR_H
#define INCLUDE_NFGE_AI_PATHFOLLOWINGBEHAVIOR_H

#include "ArriveBehavior.h"
#include "SeekBehavior.h"
#include <queue>

namespace NFGE::AI
{
	class PathFollowingBehavior : public ArriveBehavior
	{
	public:
		NFGE::Math::Vector2 Calulate(AI::Agent& agent) override;

		std::list<NFGE::Math::Vector2> path;
		float switchPointDistance = 5.0f;
	};
}

#endif // !INCLUDE_NFGE_AI_PathFollowingBEHAVIOR_H
