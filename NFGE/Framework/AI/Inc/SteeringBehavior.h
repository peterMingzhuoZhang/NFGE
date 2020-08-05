#ifndef INCLUDE_NFGE_AI_STEERINGBEHAVIOR_H
#define INCLUDE_NFGE_AI_STEERINGBEHAVIOR_H

#include "Common.h"
#include "Agent.h"

namespace NFGE::AI
{

	class SteeringBehavior
	{
	public:
		SteeringBehavior() = default;
		virtual ~SteeringBehavior() = default;

		virtual NFGE::Math::Vector2 Calulate(Agent& agent) = 0;

		void SetActive(bool active) { mActive = active; }
		bool IsActive() const		{ return mActive; }

		void SetWeight(float weight) { mWeight = weight; }
		float GetWeight() const { return mWeight; }
	
	protected:
		bool IsDebugDraw2D = false;
	private:
		bool mActive = false;
		float mWeight = 1.0f;
	};

} // namespace NFGE::AI

#endif // !INCLUDE_NFGE_AI_STEERINGBEHAVIOR_H