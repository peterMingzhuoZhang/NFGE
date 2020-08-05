
#include "Carrier.h"
#include "LevelManager.h"
#include "ObjectPoolManager.h"
#include "Interceptor.h"

extern BattleMap myBattleMap;

namespace
{
	std::string defaultTexturePrefix = "LaCelleCraft/PeterChanLegend/Unit/Carrier/carrier_";
	std::string defaultTextureSubfix = ".png";
	const Vector2 size{ 128.0f,128.0f };
	const float maxHealth = 10.0f;
	const float Mass = 1.0f;
	const float MaxSpeed = 70.0f;
	const float turningSpeed = 20.0f;
	const float Altitude = -100.0f;

	const int interceptorCount = 8;

	struct IdleState : public AI::State<Carrier>
	{
		static std::string GetName() { return "Idle"; }

		void Enter(Carrier& agent) override
		{
			agent.destination = agent.position;
			agent.steeringModule->GetBehavior < AI::SeekBehavior >("Seek")->SetActive(false);
			position = agent.position;
			time = 0.0f;
		}
		void Update(Carrier& agent, float deltaTime) override
		{
			time += deltaTime * 10.0f;
			//agent.position.x = position.x + sin(time * 2.0f) * 5.0f;
			agent.position.y = position.y + cos(time * 2.0f) * 1.0f;
			if (NFGE::Math::MagnitudeSqr(agent.destination - position) > 5.0f)
			{
				agent.stateMachine->ChangeState("Walk");
			}

		}
		void Exit(Carrier& agent) override
		{
		}
	private:
		Vector2 position;
		float time;
	};

	struct WalkState : public AI::State<Carrier>
	{
		static std::string GetName() { return "Walk"; }

		void Enter(Carrier& agent) override
		{
			TextureGridMap& worldMap = *(LevelManager::Get()->myBattleMapPtr);
			//destination = worldMap.initPosition + RandomUnitCircle() * worldMap.graphMaxArea.x * 0.4f + Vector2{ worldMap.graphMaxArea.x * 0.5f, worldMap.graphMaxArea.y * 0.5f };
			
			int end = worldMap.ConvertToMapIndex(agent.destination);
			path.push_front(end);
			mNextSubDestination = RandomCirclePosInTile(path.front());
			agent.velocity = mLastVelocity;
			agent.destination = mNextSubDestination;
			agent.steeringModule->GetBehavior < AI::SeekBehavior >("Seek")->SetActive(true);
			ShootOutInterceptor(agent);
		}
		void Update(Carrier& agent, float deltaTime) override
		{
			TextureGridMap& worldMap = *(LevelManager::Get()->myBattleMapPtr);

			if (path.empty())
			{
				agent.stateMachine->ChangeState("Idle");
			}
			else if (EngineWrapper::DistanceSqr(agent.position, mNextSubDestination) < worldMap.textureSize.x * worldMap.xScale * 0.5f)
			{
				path.pop_front();
				if (!path.empty())
				{
					mNextSubDestination = RandomCirclePosInTile(path.front());
					agent.destination = mNextSubDestination;
				}
			}
			else
			{
				if ((mNextSubDestination.x != agent.destination.x) || (mNextSubDestination.y != agent.destination.y))
				{
					agent.stateMachine->ChangeState("Walk");
				}
			}


		}
		void Exit(Carrier& agent) override
		{
			path.clear();
			mLastVelocity = agent.velocity;
			agent.velocity = Vector2{};
			agent.steeringModule->GetBehavior < AI::SeekBehavior >("Seek")->SetActive(false);
		}
	private:

		Vector2 RandomCirclePosInTile(size_t tileIndex)
		{
			TextureGridMap& worldMap = *(LevelManager::Get()->myBattleMapPtr);
			return worldMap.gridGraph.GetNode(tileIndex).position + RandomUnitCircle() * 0.5f;
		}

		void ShootOutInterceptor(Carrier& agent)
		{
			for (auto& interceptor: agent.mInterceptors)
			{
				if (interceptor->mTrivalPoints.empty())
				{
					interceptor->mTrivalPoints.push_back(agent.position + RandomUnitCircle() * 100.0f);
					interceptor->mTrivalPoints.push_back(mNextSubDestination);
					interceptor->mIsInDock = false;
				}
				else
				{
					interceptor->mTrivalPoints.push_front(mNextSubDestination);
				}
			}
		}

		std::list<size_t> path;
		Vector2 mNextSubDestination;
		Vector2 mLastVelocity{};
	};
}

TextureId Carrier::mDefaultTexture[Carrier::UnitDefaultDirectionCount];
int Carrier::mHealthBarSliceCount = 8;

Carrier::Carrier()
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

void Carrier::Load(TextureGridMap& worldMap, const Vector2& initPos, int commanderIndex)
{
	Unit::Load(worldMap, initPos, size, maxHealth, commanderIndex);

	mHealthBar.Load(mHealthBarSliceCount);
	stateMachine = std::make_unique<AI::StateMachine<Carrier>>(*this);

	mCurrentHealth = 0;
	mCurrentDirection = 0;
	mass = Mass;
	maxSpeed = MaxSpeed;
	mTurningSpeed = turningSpeed;
	mAltitude = Altitude;
	destination = Vector2(initPos.x + 1.0f, initPos.y + mAltitude * worldMap.yScale * LevelManager::Get()->mWorldCamera->GetScale());

	//Add steering behaviro
	steeringModule->AddBehavior < AI::SeekBehavior >("Seek")->SetActive(true);
	//Add state for statmachine
	stateMachine->AddState<IdleState>();
	stateMachine->AddState<WalkState>();
	//Add clip into animator

	//Load 8 interceptor
	for (int i = 0; i < interceptorCount; i++)
	{
		Interceptor* newInterceptor = static_cast<Interceptor*>(ObjectPoolManager::Get()->GetUnitFromPool("Interceptor"));
		newInterceptor->Load(*mWorldMap, position, this, mCommanderIndex);
		newInterceptor->mIsActive = true;
		mInterceptors.push_back(newInterceptor);
		//LevelManager::Get()->mCommanders[mCommanderIndex].RegisterNewUnit(newInterceptor);
	}
}

void Carrier::Update(float deltaTime)
{
	if (mIsBuilding)
		return;
	Unit::Update(deltaTime);

	mHealthBar.Update(mCurrentHealth / mMaxHealth);

	stateMachine->Update(deltaTime);

	//for (auto& interceptor : mInterceptors)
	//{
	//	interceptor->Update(deltaTime);
	//}
}

void Carrier::ControlUpdate(const Vector2& currentMousePos, float deltaTime)
{
	Unit::ControlUpdate(currentMousePos, deltaTime);
}

void Carrier::Render(const Camera2D & camera)
{
	if (mIsBuilding)
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
		Vector2 toTopLeft = { 0.0f, -mDefaultHeight * mDrawScale.y};
		mHealthBar.Render(camera.ConvertToScreen(positionWithAltitude + toBottomLeft + toTopLeft), healthBarLength, camera.GetScale());
	}

	//for (auto& interceptor : mInterceptors)
	//{
	//	interceptor->Render(camera); 
	//}
}

void Carrier::DebugUIUpdate(float deltaTime, const Camera2D & camera)
{
	Unit::DebugUIUpdate(deltaTime, camera);
}

void Carrier::Unload()
{
}

void Carrier::Attack( Unit & unit, float deltaTime)
{
}

void Carrier::Attack( Building & strcuture, float deltaTime)
{
}

void Carrier::TakeDamage(LC::Projectile & bullet)
{
}

void Carrier::Die()
{
	Unit::Die();
}
