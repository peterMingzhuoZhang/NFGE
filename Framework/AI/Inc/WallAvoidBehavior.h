#ifndef INCLUDE_NFGE_AI_WALLAVOIDBEHAVIOR_H
#define INCLUDE_NFGE_AI_WALLAVOIDBEHAVIOR_H

#include "SteeringBehavior.h"

namespace NFGE::AI
{
	class WallAvoidBehavior : public SteeringBehavior
	{
	public:
		WallAvoidBehavior()
			: mDetectors()
		{
			SetWeight(1.0f);
			mDetectors.emplace_back(3.0f, 18.0f, 0.0f);
			mDetectors.emplace_back(3.0f, 8.0f, 0.7f);
			mDetectors.emplace_back(3.0f, 3.0f, -0.7f);
			//mDetectors.emplace_back(3.0f, 30.0f, 0.8f);
			//mDetectors.emplace_back(3.0f, 30.0f, -0.8f);
			//mDetectors.emplace_back(3.0f, 30.0f, 1.0f);
			//mDetectors.emplace_back(3.0f, 30.0f, -1.0f);
		}

		struct Detector
		{
			Detector(float _near, float _far, float _tilt)
				: mNear(_near)
				, mFar(_far)
				, mTilt(_tilt)
			{}
			float mNear;
			float mFar;
			float mTilt;
		};
	public:
		NFGE::Math::Vector2 Calulate(AI::Agent& agent) override;

		std::vector<Detector> mDetectors;
	private:
		NFGE::Math::Vector2 ProduceForce(const Detector& detector, const NFGE::Math::LineSegment& wall, AI::Agent & agent);
		void RenderDetectors(AI::Agent & agent);
	};

}

#endif // !INCLUDE_NFGE_AI_WallAvoidBehavior_H
