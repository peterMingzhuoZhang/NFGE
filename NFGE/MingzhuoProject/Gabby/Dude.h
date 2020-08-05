#pragma once

#include "NFGE/Inc/NFGE.h"
#include "PTCamera.h"
#include "PTLabel.h"

using namespace NFGE;

namespace
{
	const float ActionDisappearTime = 10.0f;
}

struct BehaviorControl
{
	float mSeparationWeight = 1.0f;
	float mAlignmentWeight = 4.0f;
	float mCohesionWeight = 3.0f;

	float mWanderWeight = 2.0f;
	float mChaseWeight = 10.0f;
	float mObsAvoidWeight = 10.0f;
	float mNeighborRadius = 100.0f;
	float mFleeWeight = 0.0f;

	
};

struct TriangleDude : public AI::Agent
{
	TriangleDude(AI::AIWorld& world)
		:AI::Agent(world)
	{

	}

	void Initialize(NFGE::Graphics::Color color, float worldWidth, float worldHeight, std::string name)
	{
		mNameLabel.Load(name, 20.0f, NFGE::Math::Vector2{ 0.0f,0.0f }, color);
		mColor = color;
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
		steeringModule->AddBehavior < AI::FleeBehavior >("Flee")->SetActive(false);
		steeringModule->GetBehavior < AI::FleeBehavior >("Flee")->SetWeight(0.0f);
		steeringModule->GetBehavior < AI::FleeBehavior >("Flee")->panicDistance = 400.0f;
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

		radius = 10.0f;

		screenWidth = worldWidth;
		screenHeight = worldHeight;
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


		float left = -screenWidth * 0.5f;
		float top = -screenHeight * 0.5f;
		float right = screenWidth * 0.5f;
		float bottom = screenHeight * 0.5f;

		if ((position.x > right))
			position.x = left;
		else if ((position.x < left))
			position.x = right;
		if ((position.y > bottom))
			position.y = top;
		else if ((position.y < top))
			position.y = bottom;

		mActionDisapperaTimer += deltaTime;
	}

	void Update(const BehaviorControl& behaviorControl, float deltaTime)
	{
		steeringModule->GetBehavior< NFGE::AI::WanderBehavior >("Wander")->SetWeight(behaviorControl.mWanderWeight);
		steeringModule->GetBehavior< NFGE::AI::GroupSeparationBehavior>("GroupSeparation")->SetWeight(behaviorControl.mSeparationWeight);
		steeringModule->GetBehavior< NFGE::AI::GroupCohesionBehavior>("GroupCohesion")->SetWeight(behaviorControl.mCohesionWeight);
		steeringModule->GetBehavior< NFGE::AI::GroupAlignmentBehavior>("GroupAlignment")->SetWeight(behaviorControl.mAlignmentWeight);
		steeringModule->GetBehavior< NFGE::AI::ObstacleAvoidBehavior>("ObstacleAvoid")->SetWeight(behaviorControl.mObsAvoidWeight);
		steeringModule->GetBehavior< NFGE::AI::PursuitBehavior>("Pursuit")->SetWeight(behaviorControl.mChaseWeight);
		steeringModule->GetBehavior< NFGE::AI::SeekBehavior>("Seek")->SetWeight(behaviorControl.mChaseWeight);
		steeringModule->GetBehavior< NFGE::AI::FleeBehavior>("Flee")->SetWeight(behaviorControl.mFleeWeight);
		neighbourRange = behaviorControl.mNeighborRadius;

		UpdateNeighbor();
		Update(deltaTime);
	}

	void Render(const PTCamera& camera)
	{
		float radiusMutiplyer = NFGE::Math::Magnitude(velocity) / 300.0f;
		if (radiusMutiplyer < 0.75f) radiusMutiplyer = 0.75f;
		if (radiusMutiplyer > 1.00f) radiusMutiplyer = 1.00f;
		auto useRadius = radius * radiusMutiplyer;
		useRadius = useRadius / camera.GetScale();
		auto usePosition = camera.ConvertToScreen(position);

		NFGE::Math::Vector2 forwardPoint = heading * useRadius * 2.0f;
		forwardPoint = forwardPoint + usePosition;
		NFGE::Math::Vector2 leftPoint{ -heading.y * useRadius, heading.x * useRadius };
		leftPoint -= heading * useRadius * 1.0f;
		leftPoint = leftPoint + usePosition;
		NFGE::Math::Vector2 rightPoint{ heading.y* useRadius, -heading.x * useRadius };
		rightPoint -= heading * useRadius * 1.0f;
		rightPoint = rightPoint + usePosition;
		NFGE::sApp.DrawScreenLine(leftPoint, forwardPoint, mColor);
		NFGE::sApp.DrawScreenLine(rightPoint, forwardPoint, mColor);
		NFGE::sApp.DrawScreenLine(leftPoint, rightPoint, mColor);


		mNameLabel.SetPos(position + NFGE::Math::Vector2(0.0f, -radius * 2.0f));
		float alpha = (ActionDisappearTime - mActionDisapperaTimer) / ActionDisappearTime;
		if (alpha < 0.0f) alpha = 0.0f;
		mNameLabel.Render(camera, alpha);
	}

	void ActionShowName()
	{
		mActionDisapperaTimer = 0.0f;
	}

	std::unique_ptr<AI::SteeringModule> steeringModule;
	NFGE::Graphics::Color mColor = NFGE::Graphics::Colors::Green;

	float screenWidth;
	float screenHeight;

	PTLabel mNameLabel;
	float mActionDisapperaTimer = 0.0f;
};