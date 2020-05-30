#ifndef HELLOSTEERINGADVANCE_TRIANGLEDUDE_HEADER
#define HELLOSTEERINGADVANCE_TRIANGLEDUDE_HEADER

#include "NFGE/Inc/NFGE.h"

using namespace NFGE;
struct TriangleDude : public AI::Agent
{
	TriangleDude(AI::AIWorld& world)
		:AI::Agent(world)
	{

	}

	void Initialize()
	{
		maxSpeed = 300.0f;
		steeringModule = std::make_unique<AI::SteeringModule>(*this);
		//---- Seek --------------------------------------------------------------------------------------------------------
		steeringModule->AddBehavior < AI::SeekBehavior >("Seek")->SetActive(false);
		//------------------------------------------------------------------------------------------------------------------

		//---- Wander ------------------------------------------------------------------------------------------------------
		auto wanderBehavior = steeringModule->AddBehavior < AI::WanderBehavior >("Wander");
		wanderBehavior->SetParama(20.0f, 10.0f, 1.0f);
		wanderBehavior->SetActive(false);
		//------------------------------------------------------------------------------------------------------------------

		//---- Arrive ------------------------------------------------------------------------------------------------------
		steeringModule->AddBehavior < AI::ArriveBehavior >("Arrive")->SetActive(false);
		steeringModule->GetBehavior < AI::ArriveBehavior >("Arrive")->SetWeight(1.0f);
		//------------------------------------------------------------------------------------------------------------------

		//---- Pursuit ------------------------------------------------------------------------------------------------------
		steeringModule->AddBehavior < AI::PursuitBehavior>("Pursuit")->SetActive(false);
		//------------------------------------------------------------------------------------------------------------------

		//---- Evade -------------------------------------------------------------------------------------------------------
		steeringModule->AddBehavior<AI::EvadeBehavior>("Evade")->SetActive(false);
		//------------------------------------------------------------------------------------------------------------------

		//---- OffSetPursuit -----------------------------------------------------------------------------------------------
		steeringModule->AddBehavior<AI::OffsetPursuitBehavior<AI::TriangleTeamFormContext>>("OffsetPursuit")->SetActive(false);
		//------------------------------------------------------------------------------------------------------------------

		auto obsAvioid = steeringModule->AddBehavior < AI::ObstacleAvoidBehavior >("ObstacleAvoid");

		obsAvioid->SetActive(true);
		obsAvioid->SetWeight(10.0f);
		//------------------------------------------------------------------------------------------------------------------

		radius = 10.0f;

		screenWidth = (float)NFGE::sApp.GetScreenWidth();
		screenHeight = (float)NFGE::sApp.GetScreenHeight();
		position = NFGE::Math::Vector2(0.0f, 0.0f);
		velocity = NFGE::Math::Vector2(0.0f, 1.0f);
	}



	void Update(float deltaTime)
	{
		//UpdateNeighbor();
		auto force = steeringModule->Calculate();
		auto acceleration = force / mass;
		velocity += acceleration * deltaTime;
		// TODO add truncate method here
		position += velocity * deltaTime;

		if (NFGE::Math::MagnitudeSqr(velocity) > 1.0f)
			heading = NFGE::Math::Normalize(velocity);

		if ((position.x > screenWidth))
			position.x = static_cast<float>((int)position.x % (int)screenWidth);
		else if ((position.x < 0.0f))
			position.x = screenWidth;
		if ((position.y > screenHeight))
			position.y = static_cast<float>((int)position.y % (int)screenHeight);
		else if ((position.y < 0.0f))
			position.y = screenHeight;
	}

	void Render()
	{
		NFGE::Math::Vector2 forwardPoint = heading * radius * 2.0f;
		forwardPoint = forwardPoint + position;
		NFGE::Math::Vector2 leftPoint{ -heading.y * radius, heading.x * radius };
		leftPoint -= heading * radius * 1.0f;
		leftPoint = leftPoint + position;
		NFGE::Math::Vector2 rightPoint{ heading.y* radius, -heading.x * radius };
		rightPoint -= heading * radius * 1.0f;
		rightPoint = rightPoint + position;
		NFGE::sApp.DrawScreenLine(leftPoint, forwardPoint, mColor);
		NFGE::sApp.DrawScreenLine(rightPoint, forwardPoint, mColor);
		NFGE::sApp.DrawScreenLine(leftPoint, rightPoint, mColor);

		auto wanderBehavior = steeringModule->GetBehavior<AI::WanderBehavior>("Wander");
		if (wanderBehavior != nullptr)
		{
			NFGE::sApp.DrawScreenCircle(wanderBehavior->renderPoint, 5.0f, NFGE::Graphics::Colors::Red);
			NFGE::sApp.DrawScreenCircle(position + heading * wanderBehavior->wanderDistance, wanderBehavior->wanderRadius, NFGE::Graphics::Colors::Yellow);
		}

		//Draw obsAvoid debugline
		NFGE::sApp.DrawScreenLine(leftPoint, leftPoint + heading * NFGE::Math::Magnitude(velocity), NFGE::Graphics::Colors::AliceBlue);
		NFGE::sApp.DrawScreenLine(rightPoint, rightPoint + heading * NFGE::Math::Magnitude(velocity), NFGE::Graphics::Colors::AliceBlue);
		NFGE::sApp.DrawScreenLine(rightPoint + heading * NFGE::Math::Magnitude(velocity), leftPoint + heading * NFGE::Math::Magnitude(velocity), NFGE::Graphics::Colors::AliceBlue);
	}

	std::unique_ptr<AI::SteeringModule> steeringModule;
	NFGE::Graphics::Color mColor = NFGE::Graphics::Colors::Green;

	float screenWidth;
	float screenHeight;

};

#endif // !HELLOSTEERINGADVANCE_TRIANGLEDUDE_HEADER
