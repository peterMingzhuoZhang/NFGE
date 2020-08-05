#ifndef INCLUDE_NFGE_AI_OFFSETPURSUITBEHAVIOR_H
#define INCLUDE_NFGE_AI_OFFSETPURSUITBEHAVIOR_H

#include "ArriveBehavior.h"

namespace NFGE::AI
{
	struct TeamFormContext
	{
		NFGE::Math::Vector2 position;
		virtual NFGE::Math::Vector2 GetPosition(int index) {
			return NFGE::Math::Vector2();
		}
	};

	struct TriangleTeamFormContext : TeamFormContext
	{
		float yOffset = 50.0f;
		float xOffset = 50.0f;
		int GetRow(size_t index)
		{
			int ret = 0;
			while ((static_cast<int>(index) >= ((ret + 1) * (ret + 2)) >> 1) )
			{
				ret++;
			}
			return ret;
		}
		NFGE::Math::Vector2 GetPosition(int index) override
		{
			
			int row = GetRow(index);
			int col = (index)- ((row * (row + 1)) >> 1);
			float posX = (float)(col) * xOffset - (float)row * (xOffset * 0.5f);
			float posY = -yOffset * row;
			return NFGE::Math::Vector2(posX, posY);
		}
	};


	template <typename TeamFormPolicy>
	class OffsetPursuitBehavior : public ArriveBehavior
	{
	public:
		NFGE::Math::Vector2 Calulate(AI::Agent& agent) override
		{
			agent.destination = NFGE::Math::TransformCoord(teamFormPolicy.GetPosition(agentIndex), leader->LocalToWorld());
			NFGE::Math::Vector2 ret = ArriveBehavior::Calulate(agent);

			return ret + (leader->heading - agent.velocity);
		}

		TeamFormPolicy teamFormPolicy;
		Agent* leader;
		int agentIndex;
	};
}

#endif // !INCLUDE_NFGE_AI_OFFSETPURSUITBEHAVIOR_H