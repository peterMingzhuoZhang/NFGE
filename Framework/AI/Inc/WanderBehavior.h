#ifndef INCLUDE_NFGE_AI_WANDERBEHAVIOR_H
#define INCLUDE_NFGE_AI_WANDERBEHAVIOR_H

#include "SeekBehavior.h"

namespace NFGE::AI
{
	class WanderBehavior : public SeekBehavior
	{
	public:
		NFGE::Math::Vector2 Calulate(AI::Agent& agent) override;
		void SetParama(float radius, float distance, float jitter);

	//private:
		float wanderRadius = 20.0f;
		float wanderDistance = 50.0f;
		float wanderJitter = 10.0f;

		NFGE::Math::Vector2 wanderPoint = {0.0f,0.0f};
		NFGE::Math::Vector2 renderPoint; // render purpose
	};

}

#endif // !INCLUDE_NFGE_AI_WANDERBEHAVIOR_H
