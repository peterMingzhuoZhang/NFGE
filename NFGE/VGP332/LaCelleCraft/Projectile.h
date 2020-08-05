#ifndef NFGE_LACELLECRAFT_PROJECTILE_H
#define NFGE_LACELLECRAFT_PROJECTILE_H

#include "Animation.h"

struct Building;
struct Unit;
namespace LC
{
	enum ProjectileState
	{
		Idle = 0,
		InTravel,
		Destroyed,
		TotalStateCount
	};

	struct Projectile : public NFGE::AI::Agent
	{
		static const int directionCount = 1;  //For high quality look projectile increase the number

		Projectile(NFGE::AI::AIWorld& world);
		
		// Machanic Logic
		int mCommanderIndex;
		bool mIsActive = false;
		bool mIsAOE;

		// Render 
		int mCurrentDirection;				//For high quality look projectile
		LCAnimator mAnimator;
		float mDefaultWidth;
		float mDefaultHeight;
		NFGE::Math::Vector2 mDrawScale;
		NFGE::Math::Vector2 mSize;
		float mAltitude = -10.0f;
		float mTurningSpeed = 2.0f;

		Unit* mTargetUnit;
		Building* mTargetBuilding;

		Projectile** mInLevelRegistry = nullptr;
		// AI Logic
		std::unique_ptr<NFGE::AI::SteeringModule> steeringModule;
		std::unique_ptr<NFGE::AI::StateMachine<Projectile>> stateMachine;
		
		// Combat Logic
		float mDmg;

		void Load();
		virtual void Update(float deltaTime);
		virtual void Fire(const Vector2& initPos, const Vector2& size, int commanderIndex, Building& structure) = 0;
		virtual void Fire(const Vector2& initPos, const Vector2& size, int commanderIndex, Unit& structure) = 0;
		virtual void Render(const Camera2D& camera) = 0;
		void Die();
	
	private:
		void UpdatePhysics(float deltaTime)
		{
			auto force = steeringModule->Calculate() * mTurningSpeed;
			auto acceleration = force / mass;
			velocity += acceleration * deltaTime;
			// TODO add truncate method here
			position += velocity * deltaTime;

			if (NFGE::Math::MagnitudeSqr(velocity) > 1.0f)
				heading = NFGE::Math::Normalize(velocity);
		}
	};

	namespace Projectile_State
	{
		struct IdleState : public NFGE::AI::State<Projectile>
		{
			static std::string GetName() { return "Idle"; }

			void Enter(Projectile& agent) override;
			void Update(Projectile& agent, float deltaTime) override;
			void Exit(Projectile& agent) override;
		private:
		};

		struct InTravelState : public NFGE::AI::State<Projectile>
		{
			static std::string GetName() { return "InTravel"; }

			void Enter(Projectile& agent) override;
			void Update(Projectile& agent, float deltaTime) override;
			void Exit(Projectile& agent) override;
		private:
			float hitRange = 5.0f;
		};

		struct DestoryedState : public NFGE::AI::State<Projectile>
		{
			static std::string GetName() { return "Destoryed"; }

			void Enter(Projectile& agent) override;
		

			void Update(Projectile& agent, float deltaTime) override;

			void Exit(Projectile& agent) override;
			
		private:
			
		};
	}

}


#endif // !NFGE_LACELLECRAFT_PROJECTILE_H
