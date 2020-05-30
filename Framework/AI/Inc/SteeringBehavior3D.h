#ifndef INCLUDE_NFGE_AI_STEERINGBEHAVIOR3D_H
#define INCLUDE_NFGE_AI_STEERINGBEHAVIOR3D_H

#include "Common.h"
#include "Agent3D.h"

namespace NFGE::AI
{

	class SteeringBehavior3D
	{
	public:
		SteeringBehavior3D() = default;
		virtual ~SteeringBehavior3D() = default;

		virtual NFGE::Math::Vector3 Calulate(Agent3D& agent) = 0;

		void SetActive(bool active) { mActive = active; }
		bool IsActive() const { return mActive; }

		void SetWeight(float weight) { mWeight = weight; }
		float GetWeight() const { return mWeight; }

		void SetDebugDraw(bool isDebugDraw) { IsDebugDraw3D = isDebugDraw; }
		bool IsDebugDraw() const { return IsDebugDraw3D; }

	protected:
		bool IsDebugDraw3D = false;
	private:
		bool mActive = false;
		float mWeight = 1.0f;
	};

} // namespace NFGE::AI

#endif // !INCLUDE_NFGE_AI_STEERINGBEHAVIOR_H