#ifndef INCLUDE_NFGE_AI_WALLAVOIDBEHAVIOR3D_H
#define INCLUDE_NFGE_AI_WALLAVOIDBEHAVIOR3D_H

#include "SteeringBehavior3D.h"

namespace NFGE::AI
{
	class WallAvoidBehavior3D : public SteeringBehavior3D
	{
	public:
		WallAvoidBehavior3D()
			: mDetectors()
		{
			SetWeight(1.0f);
			mDetectors.emplace_back(NFGE::Math::Vector3::Zero(), NFGE::Math::Vector3{0.0f,1.0f,0.0f}, 100.0f);
		}

		struct Detector
		{
			Detector(NFGE::Math::Vector3 pos, NFGE::Math::Vector3 dir, float length)
				: mAntuna(pos, dir)
				, mLength(length)
			{}
			NFGE::Math::Ray mAntuna;
			float mLength;
		};
	public:
		NFGE::Math::Vector3 Calulate(AI::Agent3D& agent) override;

		std::vector<Detector> mDetectors;
	private:
		NFGE::Math::Vector3 ProduceForce(const Detector& detector, const NFGE::Math::Plane& wall, AI::Agent3D & agent);
		void RenderDetectors(AI::Agent3D & agent);
	};

}

#endif // !INCLUDE_NFGE_AI_WallAvoidBehavior3D_H
