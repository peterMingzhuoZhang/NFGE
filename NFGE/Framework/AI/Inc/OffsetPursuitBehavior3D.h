#ifndef INCLUDE_NFGE_AI_OFFSETPURSUITBEHAVIOR3D_H
#define INCLUDE_NFGE_AI_OFFSETPURSUITBEHAVIOR3D_H

#include "ArriveBehavior.h"

namespace NFGE::AI
{
	struct TeamForm3DContext
	{
		NFGE::Math::Vector3 position;
		virtual NFGE::Math::Vector3 GetPosition(int index) {
			return NFGE::Math::Vector3();
		}
	};

	struct ConeTeamFormContext : TeamForm3DContext
	{
		float yOffset = -50.0f;
		float radiusUnit = 200.0f;
		int incremnet = 9;
		
		NFGE::Math::Vector3 GetPosition(int index) override
		{
			

			if(index == 0)
				return NFGE::Math::Vector3();

			float angleUnit = NFGE::Math::Constants::TwoPi / incremnet;
			
			int col = (index) -1;

			float currentAngleRad = (float)(col)* angleUnit;
			float posX = cosf((float)(col)* angleUnit) * radiusUnit;
			float posZ = sinf((float)(col)* angleUnit) * radiusUnit;
			float posY = yOffset * 1;


			return NFGE::Math::Vector3(posX, posY, posZ);
		}
	};


	template <typename TeamForm3DPolicy>
	class OffsetPursuitBehavior3D : public ArriveBehavior3D
	{
	public:
		NFGE::Math::Vector3 Calulate(AI::Agent3D& agent) override
		{
			agent.destination = teamFormPolicy.GetPosition(agentIndex) * leader->LocalToWorldMatrix();
			NFGE::Math::Vector3 ret = ArriveBehavior3D::Calulate(agent);

			return ret + (leader->heading - agent.velocity);
		}

		TeamForm3DPolicy teamFormPolicy;
		Agent3D* leader;
		int agentIndex;
	};
}

#endif // !INCLUDE_NFGE_AI_OFFSETPURSUITBEHAVIOR_H