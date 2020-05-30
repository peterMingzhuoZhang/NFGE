#ifndef INCLUDE_NFGE_AI_WANDERBEHAVIOR3D_H
#define INCLUDE_NFGE_AI_WANDERBEHAVIOR3D_H

#include "SeekBehavior3D.h"

namespace NFGE::AI
{
	class WanderBehavior3D : public SeekBehavior3D
	{
	public:
		NFGE::Math::Vector3 Calulate(AI::Agent3D& agent) override;
		void SetParama(float radius, float distance, float jitter);

		//private:
		float wanderRadius = 20.0f;
		float wanderDistance = 50.0f;
		float wanderJitter = 10.0f;

		NFGE::Math::Vector3 wanderPoint{ 0.0f,0.0f,0.0f };
		NFGE::Math::Vector3 renderPoint{ 0.0f,0.0f,0.0f }; // render purpose
	};

}

#endif // !INCLUDE_NFGE_AI_WANDERBEHAVIOR_H
