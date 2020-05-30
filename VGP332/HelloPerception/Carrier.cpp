#include "Carrier.h"
#include "PolygonGridGraphMap.h"

Carrier::Carrier(AI::AIWorld & world)
	:AI::Agent(world)
{
}

void Carrier::Initialize(PolygonGridGraphMap* thePolygonMap)
{
	maxSpeed = 100.0f;

	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	auto wander = mSteeringModule->AddBehavior<AI::WanderBehavior>("Wander");
	wander->SetParama(50.0f, 100.0f, 20.0f);
	wander->SetActive(true);
	auto obsAvoid = mSteeringModule->AddBehavior<AI::ObstacleAvoidBehavior>("ObstacleAvoid");
	auto wallAvoid = mSteeringModule->AddBehavior<AI::WallAvoidBehavior>("WallAvoid");

	for (size_t i = 0; i < std::size(mTextureIds); i++)
	{
		char name[256];
		sprintf_s(name, "carrier_%02zu.png", i + 1);
		mTextureIds[i] = LoadTexture(name);
	}


	ResetPosition(thePolygonMap->mPolygonGridGraph);
}

void Carrier::Update(float deltaTime)
{
	float screenWidth = (float)NFGE::sApp.GetScreenWidth();
	float screenHeight = (float)NFGE::sApp.GetScreenHeight();
	//--------------------------------------------------------------------------------------//|
	auto force = mSteeringModule->Calculate();												//|
	auto acceleration = force / mass;														//|
	velocity += acceleration * deltaTime;													//|
																							// TODO add truncate method here														//|
	position += velocity * deltaTime;														//| ---- update the steering physics
																							//|
	if (NFGE::Math::MagnitudeSqr(velocity) > 1.0f)												//|
		heading = NFGE::Math::Normalize(velocity);												//|
																							//--------------------------------------------------------------------------------------//|

																							//-----------------------------------------------------------------------------------//|
	if ((position.x > screenWidth))														 //|
		position.x = static_cast<float>((int)position.x % (int)screenWidth);			 //|
	else if ((position.x < 0.0f))														 //|
		position.x = screenWidth;														 //|
	if ((position.y > screenHeight))													 //| ---- clamp the SCV position
		position.y = static_cast<float>((int)position.y % (int)screenHeight);			 //|
	else if ((position.y < 0.0f))														 //|
		position.y = screenHeight;														 //|
																						 //-----------------------------------------------------------------------------------//|
}

void Carrier::Render()
{
	float angle = atan2(-heading.x, heading.y) + NFGE::Math::Constants::Pi;
	int currentDirection = (int)(angle / NFGE::Math::Constants::TwoPi* 32.0f) % 32;
	NFGE::sApp.DrawSprite(mTextureIds[currentDirection], position);
}

void Carrier::ResetPosition(const AI::Graph<PolygoneMapNode>& theGraphMap)
{
	int nextNodeIndex = NFGE::Math::Random() % theGraphMap.GetNodeCount();
	position = theGraphMap.GetNode(nextNodeIndex).position;
}
