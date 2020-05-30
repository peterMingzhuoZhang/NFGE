#include "Precompiled.h"
#include "InterposeBehavior.h"

using namespace NFGE;

NFGE::Math::Vector2 AI::InterposeBehavior::Calulate(AI::Agent & agent)
{
	agent.destination = target1->position + (target2->position - target1->position) * 0.5f;
	return ArriveBehavior::Calulate(agent);
}