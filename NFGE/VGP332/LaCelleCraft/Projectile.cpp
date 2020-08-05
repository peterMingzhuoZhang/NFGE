#include "Projectile.h"
#include "Building.h"
#include "LevelManager.h"

using namespace LC;

LC::Projectile::Projectile(NFGE::AI::AIWorld & world)
	:NFGE::AI::Agent(world)
{
	steeringModule = std::make_unique<NFGE::AI::SteeringModule>(*this);
	steeringModule->AddBehavior < NFGE::AI::SeekBehavior >("Seek")->SetActive(false);
	steeringModule->AddBehavior < NFGE::AI::ArriveBehavior >("Arrive")->SetActive(false);

	stateMachine = std::make_unique<NFGE::AI::StateMachine<Projectile>>(*this);
	// TODO add state
	//stateMachine->add
}

void LC::Projectile::Load()
{
	ASSERT(mInLevelRegistry == nullptr, "[Building] Try Load Building twice, NOT ALLOWED!");
	if (mIsAOE)
		mInLevelRegistry = LevelManager::Get()->RegisterNewAoeProjectile(this);
	else
		mInLevelRegistry = LevelManager::Get()->RegisterNewProjectile(this);
}

void LC::Projectile::Update(float deltaTime)
{
	//---------------//Agent physics Update
	UpdatePhysics(deltaTime);
	//---------------

	float angle = atan2(-heading.x, heading.y) + NFGE::Math::Constants::Pi;
	mCurrentDirection = (int)(angle / NFGE::Math::Constants::TwoPi * 32.0f) % 32;

	if (mAnimator.mClipCount != 0)
	{
		mAnimator.Update(deltaTime);
	}
	
	stateMachine->Update(deltaTime);
}

void LC::Projectile::Die()
{
	mIsActive = false;
	*mInLevelRegistry = nullptr;
	mInLevelRegistry = nullptr;
}

void LC::Projectile_State::IdleState::Enter(Projectile & agent)
{
	agent.steeringModule->GetBehavior < NFGE::AI::SeekBehavior >("Seek")->SetActive(false);
	agent.steeringModule->GetBehavior < NFGE::AI::ArriveBehavior >("Arrive")->SetActive(false);
}

void LC::Projectile_State::IdleState::Update(Projectile & agent, float deltaTime)
{
	// Just Chill
}

void LC::Projectile_State::IdleState::Exit(Projectile & agent)
{
	// Nothing to do
}

void LC::Projectile_State::InTravelState::Enter(Projectile & agent)
{
	agent.steeringModule->GetBehavior < NFGE::AI::ArriveBehavior >("Arrive")->SetActive(true);
	agent.steeringModule->GetBehavior < NFGE::AI::SeekBehavior >("Seek")->SetActive(false);
}

void LC::Projectile_State::InTravelState::Update(Projectile & agent, float deltaTime)
{
	if (NFGE::Math::DistanceSqr(agent.position, agent.destination) < hitRange * hitRange)
	{
		agent.stateMachine->ChangeState("Destoryed");
	}
}

void LC::Projectile_State::InTravelState::Exit(Projectile & agent)
{
	agent.steeringModule->GetBehavior < NFGE::AI::ArriveBehavior >("Arrive")->SetActive(false);
}

void LC::Projectile_State::DestoryedState::Enter(Projectile & agent)
{
	agent.steeringModule->GetBehavior < NFGE::AI::ArriveBehavior >("Arrive")->SetActive(true);
	agent.steeringModule->GetBehavior < NFGE::AI::SeekBehavior >("Seek")->SetActive(false);
	agent.mAnimator.ChangeToState(ProjectileState::Destroyed, false);

	if (agent.mTargetBuilding != nullptr)
	{
		agent.mTargetBuilding->TakeDamage(agent);
	}
	else
	{
		agent.mTargetUnit->TakeDamage(agent);
	}
}

void LC::Projectile_State::DestoryedState::Update(Projectile & agent, float deltaTime)
{
	if (agent.mAnimator.mState == ProjectileState::Idle) // Means the projectile already finishe the destroyed animation
	{
		agent.Die();
		agent.stateMachine->ChangeState("Idle");
	}
}

void LC::Projectile_State::DestoryedState::Exit(Projectile & agent)
{
	agent.steeringModule->GetBehavior < NFGE::AI::ArriveBehavior >("Arrive")->SetActive(false);
}