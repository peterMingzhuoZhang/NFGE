#include "Precompiled.h"
#include "Constraints.h"

using namespace NFGE;
using namespace NFGE::Physics;
using namespace NFGE::Graphics;
using namespace NFGE::Math;

Fixed::Fixed(Particle* p)
	: Fixed(p, p->position)
{
}

Fixed::Fixed(Particle* p, const Math::Vector3& position)
	: mParticle(p)
	, mPosition(position)
{
}

void Fixed::Apply() const
{
	mParticle->SetPosition(mPosition);
}

void Fixed::DebugDraw() const
{
	Graphics::SimpleDraw::AddAABB(mPosition, mParticle->radius, Colors::Green);
}

Spring::Spring(Particle* a, Particle* b, float restLength)
	: mParticleA(a)
	, mParticleB(b)
	, mRestLength(restLength)
{
	if (mRestLength <= 0.0f)
		mRestLength = Math::Distance(a->position, b->position);
}

void Spring::Apply() const
{
	Math::Vector3 delta = mParticleB->position - mParticleA->position;
	float dist = Math::Magnitude(delta);
	float diff = (dist - mRestLength) / (dist * (mParticleA->invMass + mParticleB->invMass));
	mParticleA->position += delta * diff * mParticleA->invMass;
	mParticleB->position -= delta * diff * mParticleB->invMass;
}

void Spring::DebugDraw() const
{
	Graphics::SimpleDraw::AddLine(mParticleA->position, mParticleB->position, Colors::Green);
}

NFGE::Physics::Arriver::Arriver(Particle * a, Particle * b, float resetLength)
	: mParticleA(a)
	, mParticleB(b)
	, mRestLength(resetLength)
{
	ASSERT(!NFGE::Math::IsZero(resetLength), "[BFGE::Physics::Constraints::Arriver]Can not have resetLength 0");
}

void NFGE::Physics::Arriver::Apply() const
{
	Math::Vector3 delta = mParticleA->position - mParticleB->position;
	float dist = Math::Magnitude(delta);
	float diff = dist - mRestLength;
	float percentage;
	if (diff <= 0.0f)
	{
		diff = 0.0f;
		percentage = 0.0f;
	}
	else
	{
		percentage = 1.0f - mRestLength / diff;
	}

	Vector3 travelDist = delta * (percentage/* * mSpeedModifier*/);
	mParticleB->SetPosition(mParticleB->position + (travelDist));
}

void NFGE::Physics::Arriver::DebugDraw() const
{
	Graphics::SimpleDraw::AddLine(mParticleA->position, mParticleB->position, Colors::Chocolate);
}
