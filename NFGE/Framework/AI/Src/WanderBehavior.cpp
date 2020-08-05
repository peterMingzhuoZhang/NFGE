#include "Precompiled.h"
#include "WanderBehavior.h"

using namespace NFGE;

NFGE::Math::Vector2 AI::WanderBehavior::Calulate(AI::Agent & agent)
{
	wanderPoint = wanderPoint + NFGE::Math::RandomUnitCircle(true) * wanderJitter;
	wanderPoint = NFGE::Math::Normalize(wanderPoint) * wanderRadius;
	//wanderPoint.y += wanderDistance;
	auto wanderTarget = wanderPoint + NFGE::Math::Vector2 {0.0f, wanderDistance};
	auto localToWorld = agent.LocalToWorld();
	NFGE::Math::Vector2 destination = NFGE::Math::TransformCoord(wanderTarget, localToWorld);
	renderPoint = destination; // render purpose
	agent.destination = destination;


	//X::DrawScreenCircle(X::Math::TransformCoord(X::Math::Vector2{ 0.0f, wanderDistance }, agent.LocalToWorld()), wanderRadius, X::Colors::LightBlue);
	//X::DrawScreenCircle(agent.destination, 5.0f, X::Colors::Green);
	//X::DrawScreenLine(agent.position, agent.destination, X::Colors::Cyan);

	return SeekBehavior::Calulate(agent);
}

void AI::WanderBehavior::SetParama(float radius, float distance, float jitter)
{
	wanderRadius = radius;
	wanderDistance = distance;
	wanderJitter = jitter;
}
