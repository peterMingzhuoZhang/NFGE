#include "Precompiled.h"
#include "SteeringModule.h"

using namespace NFGE::AI;
using namespace NFGE;

SteeringModule::SteeringModule(Agent& agent)
	:mAgent(agent)
{}

NFGE::Math::Vector2 SteeringModule::Calculate()
{
	NFGE::Math::Vector2 total;
	for (auto& [name, behavior] : mBehaviors)
	{
		if (behavior->IsActive())
		{
			total += behavior->Calulate(mAgent) * behavior->GetWeight();
		}
	}

	return total;
}