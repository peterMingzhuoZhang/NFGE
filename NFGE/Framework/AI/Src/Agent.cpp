#include "Precompiled.h"
#include  "Agent.h"

#include "AIWorld.h"

using namespace NFGE::AI;
using namespace NFGE;

Agent::Agent(AIWorld& world)
	:mWorld(world)
{
	world.RegisterAgent(this);
}

Agent::~Agent()
{
	mWorld.UnregisterAgent(this);
}

NFGE::Math::Matrix3 AI::Agent::LocalToWorld()
{
	NFGE::Math::Vector2 y = heading;
	NFGE::Math::Vector2 x = { heading.y, -heading.x };
	NFGE::Math::Vector2 p = position;
	return
	{
		x.x, x.y, 0.0f,
		y.x, y.y, 0.0f,
		p.x, p.y, 1.0f,
	};
}

void AI::Agent::UpdateNeighbor()
{
	//neighborhood.clear();
	mWorld.GetNeighborhood(neighborhood, NFGE::Math::Circle(position, neighbourRange));
}
