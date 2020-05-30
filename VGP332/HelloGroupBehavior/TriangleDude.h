#ifndef HELLOGROUPBEHAVIOR_TRIANGLEDUDE_H
#define HELLOGROUPBEHAVIOR_TRIANGLEDUDE_H

#include <NFGE/Inc/NFGE.h>

struct BehaviorControl
{
	float mSeparationWeight = 1.0f;
	float mAlignmentWeight = 1.0f;
	float mCohesionWeight = 1.0f;

	float mWanderWeight = 1.0f;
	float mObsAvoidWeight = 1.0f;
	float mNeighborRadius = 100.0f;
};

struct TriangleDude : public NFGE::AI::Agent
{
	TriangleDude(NFGE::AI::AIWorld& world)
		:NFGE::AI::Agent(world)
	{

	}

	void Initialize()
	{
		maxSpeed = 300.0f;
		steeringModule = std::make_unique<NFGE::AI::SteeringModule>(*this);
		//---- Seek --------------------------------------------------------------------------------------------------------
		steeringModule->AddBehavior < NFGE::AI::SeekBehavior >("Seek")->SetActive(false);
		//------------------------------------------------------------------------------------------------------------------

		//---- Wander ------------------------------------------------------------------------------------------------------
		auto wanderBehavior = steeringModule->AddBehavior < NFGE::AI::WanderBehavior >("Wander");
		wanderBehavior->SetParama(20.0f, 20.0f, 10.0f);
		wanderBehavior->SetActive(false);
		//------------------------------------------------------------------------------------------------------------------

		//---- Arrive ------------------------------------------------------------------------------------------------------
		steeringModule->AddBehavior < NFGE::AI::ArriveBehavior >("Arrive")->SetActive(false);
		steeringModule->GetBehavior < NFGE::AI::ArriveBehavior >("Arrive")->SetWeight(1.0f);
		//------------------------------------------------------------------------------------------------------------------

		//---- Pursuit ------------------------------------------------------------------------------------------------------
		steeringModule->AddBehavior < NFGE::AI::PursuitBehavior>("Pursuit")->SetActive(false);
		//------------------------------------------------------------------------------------------------------------------

		//---- Evade -------------------------------------------------------------------------------------------------------
		steeringModule->AddBehavior<NFGE::AI::EvadeBehavior>("Evade")->SetActive(false);
		//------------------------------------------------------------------------------------------------------------------

		//---- OffSetPursuit -----------------------------------------------------------------------------------------------
		steeringModule->AddBehavior<NFGE::AI::OffsetPursuitBehavior<NFGE::AI::TriangleTeamFormContext>>("OffsetPursuit")->SetActive(false);
		//------------------------------------------------------------------------------------------------------------------

		//---- ObsAvoid      -----------------------------------------------------------------------------------------------
		auto obsAvioid = steeringModule->AddBehavior < NFGE::AI::ObstacleAvoidBehavior >("ObsAvoid");

		obsAvioid->SetActive(false);
		obsAvioid->SetWeight(10.0f);
		//------------------------------------------------------------------------------------------------------------------

		//---- GroupBehavior_Sepreation ------------------------------------------------------------------------------------
		auto GroupSeparation = steeringModule->AddBehavior < NFGE::AI::GroupSeparationBehavior>("GroupSeparation");
		GroupSeparation->SetActive(false);
		GroupSeparation->SetWeight(1.0f);
		//------------------------------------------------------------------------------------------------------------------

		//---- GroupBehavior_Alignment -------------------------------------------------------------------------------------
		auto GroupAlignment = steeringModule->AddBehavior < NFGE::AI::GroupAlignmentBehavior>("GroupAlignment");
		GroupAlignment->SetActive(false);
		GroupAlignment->SetWeight(1.0f);
		//------------------------------------------------------------------------------------------------------------------
		
		//---- GroupBehavior_Cohesion --------------------------------------------------------------------------------------
		auto GroupCohesion = steeringModule->AddBehavior < NFGE::AI::GroupCohesionBehavior>("GroupCohesion");
		GroupCohesion->SetActive(false);
		GroupCohesion->SetWeight(1.0f);
		//------------------------------------------------------------------------------------------------------------------

		neighbourRange = 100.0f;
		radius = 10.0f;

		screenWidth = (float)NFGE::sApp.GetScreenWidth();
		screenHeight = (float)NFGE::sApp.GetScreenHeight();
		position = NFGE::Math::Vector2(100.0f, 100.0f);
		velocity = NFGE::Math::Vector2(0.0f, 1.0f);
	}



	void Update(const BehaviorControl& behaviorControl, float deltaTime)
	{
		steeringModule->GetBehavior< NFGE::AI::WanderBehavior >("Wander")->SetWeight(behaviorControl.mWanderWeight);
		steeringModule->GetBehavior< NFGE::AI::GroupSeparationBehavior>("GroupSeparation")->SetWeight(behaviorControl.mSeparationWeight);
		steeringModule->GetBehavior< NFGE::AI::GroupCohesionBehavior>("GroupCohesion")->SetWeight(behaviorControl.mCohesionWeight);
		steeringModule->GetBehavior< NFGE::AI::GroupAlignmentBehavior>("GroupAlignment")->SetWeight(behaviorControl.mAlignmentWeight);
		steeringModule->GetBehavior< NFGE::AI::ObstacleAvoidBehavior>("ObsAvoid")->SetWeight(behaviorControl.mObsAvoidWeight);
		neighbourRange = behaviorControl.mNeighborRadius;

		UpdateNeighbor();
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

		if (mIsDebugDraw)
			DebugDraw(leftPoint, rightPoint);
	}

	void DebugDraw(const NFGE::Math::Vector2& leftPoint, const NFGE::Math::Vector2& rightPoint )
	{
		// Draw wander debugline
		auto wanderBehavior = steeringModule->GetBehavior<NFGE::AI::WanderBehavior>("Wander");
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

	std::unique_ptr<NFGE::AI::SteeringModule> steeringModule;
	NFGE::Graphics::Color mColor = NFGE::Graphics::Colors::Green;

	float screenWidth;
	float screenHeight;
	bool mIsDebugDraw = false;


};

#endif // !HELLOGROUPBEHAVIOR_TRIANGLEDUDE
