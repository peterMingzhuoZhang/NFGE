#include "Precompiled.h"
#include "WanderBehavior3D.h"

using namespace NFGE;

NFGE::Math::Vector3 AI::WanderBehavior3D::Calulate(AI::Agent3D & agent)
{
	wanderPoint = wanderPoint + NFGE::Math::RandomUnitSphere() * wanderJitter;
	wanderPoint = NFGE::Math::Normalize(wanderPoint) * wanderRadius;
	//wanderPoint.y += wanderDistance;
	auto wanderTarget = wanderPoint + NFGE::Math::Vector3{ 0.0f, wanderDistance, 0.0f };
	auto localToWorld = agent.LocalToWorldMatrix();
	NFGE::Math::Vector3 destination = wanderTarget * localToWorld;
	renderPoint = destination; // render purpose
	agent.destination = destination;

	if (IsDebugDraw3D)
	{
		NFGE::Graphics::SimpleDraw::AddSphere(NFGE::Math::Vector3{ 0.0f, wanderDistance ,0.0f}* agent.LocalToWorld(), wanderRadius, NFGE::Graphics::Colors::LightBlue,20,20);
		NFGE::Graphics::SimpleDraw::AddSphere(agent.destination, 5.0f, NFGE::Graphics::Colors::Green);
		NFGE::Graphics::SimpleDraw::AddLine(agent.position, agent.destination, NFGE::Graphics::Colors::Cyan);
	}

	return SeekBehavior3D::Calulate(agent);
}

void AI::WanderBehavior3D::SetParama(float radius, float distance, float jitter)
{
	wanderRadius = radius;
	wanderDistance = distance;
	wanderJitter = jitter;
}
