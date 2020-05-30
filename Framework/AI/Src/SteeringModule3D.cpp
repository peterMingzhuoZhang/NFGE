#include "Precompiled.h"
#include "SteeringModule3D.h"

using namespace NFGE::AI;
using namespace NFGE;

SteeringModule3D::SteeringModule3D(Agent3D& agent)
	:mAgent(agent)
{}

NFGE::Math::Vector3 SteeringModule3D::Calculate()
{
	NFGE::Math::Vector3 total;
	for (auto&[name, behavior] : mBehaviors)
	{
		if (behavior->IsActive())
		{
			total += behavior->Calulate(mAgent) * behavior->GetWeight();
		}
	}

	return total;
}