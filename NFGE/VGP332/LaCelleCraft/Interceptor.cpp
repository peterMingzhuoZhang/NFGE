#include "Interceptor.h"
#include "LevelManager.h"
#include "Carrier.h"

extern BattleMap myBattleMap;

namespace
{
	std::string defaultTexturePrefix = "LaCelleCraft/PeterChanLegend/Unit/Interceptor/interceptor_";
	std::string defaultTextureSubfix = ".png";
	const Vector2 size{ 32.0f,32.0f };
	const float maxHealth = 20.0f;
	const float Mass = 1.0f;
	const float MaxSpeed = 500.0f;
	const float turningSpeed = 2.0f;
	const float Altitude = -100.0f;

	struct InDockState : public AI::State<Interceptor>
	{
		static std::string GetName() { return "InDock"; }

		void Enter(Interceptor& agent) override
		{
			agent.mIsInDock = true;
			agent.destination = agent.position;
			agent.steeringModule->GetBehavior < AI::SeekBehavior >("Seek")->SetActive(false);
			position = agent.position;
			agent.steeringModule->GetBehavior < AI::SeekBehavior >("Seek")->SetActive(false);
		}
		void Update(Interceptor& agent, float deltaTime) override
		{
			
			if (!agent.mTrivalPoints.empty())
			{
				agent.stateMachine->ChangeState("GoTo");
			}

		}
		void Exit(Interceptor& agent) override
		{
			agent.mIsInDock = false;
			agent.steeringModule->GetBehavior < AI::SeekBehavior >("Seek")->SetActive(true);
			agent.position = agent.mMotherShip->position;
		}
	private:
		Vector2 position;
	};

	struct GoToState : public AI::State<Interceptor>
	{
		static std::string GetName() { return "GoTo"; }

		void Enter(Interceptor& agent) override
		{
			TextureGridMap& worldMap = *(LevelManager::Get()->myBattleMapPtr);
			
			mNextSubDestination = agent.mTrivalPoints.front();
			
			agent.velocity = mLastVelocity;
			agent.destination = mNextSubDestination;
			
			agent.mIsInDock = false;

			agent.steeringModule->GetBehavior < AI::SeekBehavior >("Seek")->SetActive(true);
		}
		void Update(Interceptor& agent, float deltaTime) override
		{
			TextureGridMap& worldMap = *(LevelManager::Get()->myBattleMapPtr);

			
			if (EngineWrapper::DistanceSqr(agent.position, mNextSubDestination) < worldMap.textureSize.x * worldMap.xScale * 0.5f)
			{
				agent.mTrivalPoints.pop_front();
				if (!agent.mTrivalPoints.empty())
				{
					mNextSubDestination = agent.mTrivalPoints.front();
					agent.destination = mNextSubDestination;
				}
				else
				{
					agent.stateMachine->ChangeState("FanHang");
				}
			}
			else
			{
				if ((mNextSubDestination.x != agent.mTrivalPoints.front().x) || (mNextSubDestination.y != agent.mTrivalPoints.front().y))
				{
					agent.stateMachine->ChangeState("GoTo");
					agent.mTrivalPoints.pop_back();
				}
			}


		}
		void Exit(Interceptor& agent) override
		{
			
			mLastVelocity = agent.velocity;
			
		}
	private:

		Vector2 RandomCirclePosInTile(int tileIndex)
		{
			TextureGridMap& worldMap = *(LevelManager::Get()->myBattleMapPtr);
			return worldMap.gridGraph.GetNode(tileIndex).position + NFGE::Math::RandomUnitCircle() * 0.5f;
		}
		Vector2 mNextSubDestination;
		Vector2 mLastVelocity{};
	};

	struct FanHangState : public AI::State<Interceptor>
	{
		static std::string GetName() { return "FanHang"; }

		void Enter(Interceptor& agent) override
		{
			agent.destination = agent.mMotherShip->position;
		}
		void Update(Interceptor& agent, float deltaTime) override
		{
			TextureGridMap& worldMap = *(LevelManager::Get()->myBattleMapPtr);

			agent.destination = agent.mMotherShip->position;

			if (EngineWrapper::DistanceSqr(agent.position, agent.mMotherShip->position) < worldMap.textureSize.x * worldMap.xScale * 0.2f)
			{
				agent.stateMachine->ChangeState("InDock");
			}
			else
			{
				if (!agent.mTrivalPoints.empty())
				{
					agent.stateMachine->ChangeState("GoTo");
				}
			}


		}
		void Exit(Interceptor& agent) override
		{
			agent.velocity = Vector2{};
			agent.steeringModule->GetBehavior < AI::SeekBehavior >("Seek")->SetActive(false);
		}
	private:

	};
}



TextureId Interceptor::mDefaultTexture[Interceptor::UnitDefaultDirectionCount];
int Interceptor::mHealthBarSliceCount = 8;

Interceptor::Interceptor()
	:Unit(myBattleMap)
{
	for (int i = 0; i < UnitDefaultDirectionCount; i++)
	{
		if (i < 10)
		{
			mDefaultTexture[i] = LoadTexture((defaultTexturePrefix + "0" + std::to_string(i) + defaultTextureSubfix).c_str());
		}
		else
		{
			mDefaultTexture[i] = LoadTexture((defaultTexturePrefix + std::to_string(i) + defaultTextureSubfix).c_str());
		}
	}


	mDefaultWidth = GetSpriteWidth(mDefaultTexture[0]);
	mDefaultHeight = GetSpriteHeight(mDefaultTexture[0]);
}

void Interceptor::Load(TextureGridMap& worldMap, const Vector2& initPos, Carrier* motherShip, int commanderIndex)
{
	Unit::Load(worldMap, initPos, size, maxHealth, commanderIndex);

	mHealthBar.Load(mHealthBarSliceCount);
	stateMachine = std::make_unique<AI::StateMachine<Interceptor>>(*this);

	mCurrentHealth = maxHealth;
	mCurrentDirection = 0;
	mass = Mass;
	maxSpeed = MaxSpeed;
	mTurningSpeed = turningSpeed;
	mAltitude = Altitude;
	mIsInDock = true;
	mMotherShip = motherShip;
	destination = Vector2(initPos.x + 1.0f, initPos.y + mAltitude * worldMap.yScale * LevelManager::Get()->mWorldCamera->GetScale());

	//Add steering behaviro
	steeringModule->AddBehavior < AI::SeekBehavior >("Seek")->SetActive(true);
	//Add state for statmachine
	stateMachine->AddState<InDockState>();
	stateMachine->AddState<GoToState>();
	stateMachine->AddState<FanHangState>();
	//Add clip into animator

	//Load 8 interceptor
}

void Interceptor::Update(float deltaTime)
{
	if (mIsBuilding || mIsInDock)
		return;
	Unit::Update(deltaTime);

	mHealthBar.Update(mCurrentHealth / mMaxHealth);

	stateMachine->Update(deltaTime);
}

void Interceptor::ControlUpdate(const Vector2& currentMousePos, float deltaTime)
{
	Unit::ControlUpdate(currentMousePos, deltaTime);
}

void Interceptor::Render(const Camera2D & camera)
{
	if (mIsBuilding || mIsInDock)
		return;

	Vector2 positionWithAltitude = { position.x, position.y + mAltitude };

	Vector2 drawPos = camera.ConvertToScreen(positionWithAltitude);
	if (!camera.IsOutOfView(drawPos, { mSize.x * camera.GetScale(), mSize.y * camera.GetScale() }))
	{
		if (mAnimator.mClipCount != 0)
		{
			DrawSprite(mAnimator.GetCurrentTexture(), drawPos, 0.0f, 1.0f, 0.5f, 0.5f, mDrawScale.x * camera.GetScale(), mDrawScale.y * camera.GetScale());
			//DrawScreenCircle(drawPos, 5.0f, Red);
		}
		else
		{
			DrawSprite(mDefaultTexture[mCurrentDirection], drawPos, 0.0f, 1.0f, 0.5f, 0.5f, mDrawScale.x * camera.GetScale(), mDrawScale.y * camera.GetScale());
		}
	}

	if (mIsSelected || mIsMouseOnTop)
	{
		float healthBarLength = mDefaultWidth * mDrawScale.x * camera.GetScale();
		Vector2 toBottomLeft = { -mDefaultWidth * mDrawScale.x  * 0.5f, mDefaultHeight * mDrawScale.y * 0.5f };
		Vector2 toTopLeft = { 0.0f, -mDefaultHeight * mDrawScale.y };
		mHealthBar.Render(camera.ConvertToScreen(positionWithAltitude + toBottomLeft + toTopLeft), healthBarLength, camera.GetScale());
	}
}

void Interceptor::DebugUIUpdate(float deltaTime, const Camera2D & camera)
{
	Unit::DebugUIUpdate(deltaTime, camera);
}

void Interceptor::Unload()
{
}

void Interceptor::Attack( Unit & unit, float deltaTime)
{
}

void Interceptor::Attack( Building & strcuture, float deltaTime)
{
}

void Interceptor::TakeDamage(Projectile & bullet)
{
}

void Interceptor::Die()
{
	Unit::Die();
}
