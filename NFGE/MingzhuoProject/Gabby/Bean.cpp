#include "Bean.h"
#include "GameState.h"

namespace
{
	const float RadiusMax = 5.0f;
	const float RadiusMin = 2.0f;
	const float RadiusChangeSpeed = 2.0f;

	const int Thickness = 3;
}

void Bean::Load(const NFGE::Math::Vector2 & position, float worldWidth, float worldHeight, const NFGE::Graphics::Color& color)
{
	Load(worldWidth, worldHeight, color);
	mCircle.center = position;
}

void Bean::Load(float worldWidth, float worldHeight, const NFGE::Graphics::Color & color)
{
	
	mCircle.radius = RadiusMin;
	mColor = color;
	mRadiusChangeSpeed = RadiusChangeSpeed;

	mWorldRect.left = -worldWidth * 0.5f;
	mWorldRect.right = worldWidth * 0.5f;
	mWorldRect.top = -worldHeight * 0.5f;
	mWorldRect.bottom = worldHeight * 0.5f;

	RandomGenreate();
}

void Bean::Update(GameState * theGame, const TriangleDude & Gabby, float deltaTime)
{
	mCircle.radius += mRadiusChangeSpeed * deltaTime;
	if (mCircle.radius > RadiusMax)
	{
		mCircle.radius = RadiusMax;
		mRadiusChangeSpeed *= -1.0f;
	}
	else if (mCircle.radius < RadiusMin)
	{
		mCircle.radius = RadiusMin;
		mRadiusChangeSpeed *= -1.0f;
	}

	if (Math::Intersect(mCircle, Math::Circle{ Gabby.position,Gabby.radius }))
	{
		++(theGame->mScore);
		theGame->mScoreDisappearCounter = 0.0f;
		theGame->GeneratePt();
		Boom(mCircle.center, theGame);
		RandomGenreate();
	}
}

void Bean::Render(const PTCamera & camera)
{
	auto renderPos = camera.ConvertToScreen(mCircle.center);
	auto renderRadius = mCircle.radius / camera.GetScale();
	for (int i = 0; i < Thickness; ++i)
	{
		SimpleDraw::AddScreenCircle(renderPos,renderRadius - i * 1.0f, mColor,32);
	}
}

void Bean::RandomGenreate()
{
	float randomX = NFGE::Math::RandomFloat(mWorldRect.left, mWorldRect.right);
	float randomY = NFGE::Math::RandomFloat(mWorldRect.top, mWorldRect.bottom);
	mCircle.center = NFGE::Math::Vector2{ randomX, randomY };
}

void Bean::Boom(const NFGE::Math::Vector2 & pos, GameState * theGame)
{
	for (int i = 0; i < 10; i++)
	{
		auto vel = NFGE::Math::RandomUnitSphere() * 1.5f;

		auto particle = new NFGE::Physics::Particle();
		particle->SetPosition({ pos.x,pos.y,0.0f });
		particle->SetVelocity(vel);
		particle->radius = 0.5f;
		particle->invMass = 1.0f;
		particle->gravityScale = 0.0f;
		theGame->mPhysicsWorld.AddParticle(particle);
		theGame->particleLifeTimeCounter.push_back(0.0f);
		theGame->mParticles.push_back(particle);
	}
}
