#include "Worker.h"
#include "LevelManager.h"
#include "LCTexture.h"
#include "Building.h"

using namespace NFGE::Input;
using namespace LCUnit;

extern BattleMap myBattleMap;

namespace
{
	std::string moneyTexture = "LaCelleCraft/Util/money.png";
	const Vector2 moneyTextureSize{ 20.0,20.0f };

	
	int GetCloseTile(Worker& agent, const std::vector<int>& tiles)
	{
		int workerTile = agent.mWorldMap->ConvertToMapIndex(agent.position);
		Vector2 tileIndexPos = agent.mWorldMap->GetRowAndCol(workerTile);

		float closeDist = FLT_MAX;
		int ret;
		for (auto& tile : tiles)
		{
			float currentDist = EngineWrapper::DistanceSqr(agent.mWorldMap->GetRowAndCol(tile), tileIndexPos);
			if (currentDist < closeDist)
			{
				closeDist = currentDist;
				ret = tile;
			}
		}
		return ret;
	}

	int GetCloseTile(Worker& agent, const NFGE::Math::Vector2& pos)
	{
		TextureGridMap& worldMap = *(LevelManager::Get()->myBattleMapPtr);
		return worldMap.ConvertToMapIndex(pos);
	}

	struct IdleState : public AI::State<Worker>
	{
		static std::string GetName() { return "Idle"; }

		void Enter(Worker& agent) override
		{
			agent.destination = agent.position;
			//agent.steeringModule->GetBehavior < AI::SeekBehavior >("Seek")->SetActive(false);
			position = agent.position;
			time = 0.0f;
		}
		void Update(Worker& agent, float deltaTime) override
		{
			time += deltaTime * 10.0f;
			agent.position.x = position.x + sin(time * 2.0f) * 5.0f;
			agent.position.y = position.y + cos(time * 2.0f) * 1.0f;
			if (NFGE::Math::MagnitudeSqr(agent.destination - position) > 5.0f)
			{
				agent.stateMachine->ChangeState("Walk");
			}

			if (agent.mIsGarthing && agent.mCurrentCarryMoney == 0)
			{
				agent.stateMachine->ChangeState("Gathring");
			}

		}
		void Exit(Worker& agent) override
		{
		}
	private:
		Vector2 position;
		float time;
	};

	struct GathringState : public AI::State<Worker>
	{
		static std::string GetName() { return "Gathring"; }

		void Enter(Worker& agent) override
		{
			switch (agent.mTowardStructure->mType )
			{
			case StructureType::Resource:
			{
				int choseTile = GetCloseTile(agent, agent.mTowardStructure->mGetInTile);
				agent.destination = agent.mWorldMap->gridGraph.GetNode(choseTile).position;
			}
				break;
			case StructureType::Building:																		//|
			{																									//|
				std::vector<int> allBaseTile;																	//|
				for (auto& building : LevelManager::Get()->mCommanders[agent.mCommanderIndex].myBuilding)		//|
				{																								//|
					if (building->mIsResourceCollectable)														//|
					{																							//|
						for (int i = 0; i < building->mGetInTile.size(); i++)									//|
						{																						//|--- Let worker choose the closest building to send back money
							allBaseTile.push_back(building->mGetInTile[i]);										//|
						}																						//|
					}																							//|
				}																								//|
																												//|
				int choseTile = GetCloseTile(agent, allBaseTile);												//|
				agent.destination = agent.mWorldMap->gridGraph.GetNode(choseTile).position;						//|
			}
				break;
			default:
				agent.destination = agent.position;
				break;
			}

			agent.stateMachine->ChangeState("Walk");
			
		}
		void Update(Worker& agent, float deltaTime) override
		{
			if (agent.mIsGarthing == false)
			{
				agent.stateMachine->ChangeState("Walk");
			}

		}
		void Exit(Worker& agent) override
		{
		}
	private:
		
	};

	struct AttackState : public AI::State<Worker>
	{
		static std::string GetName() { return "Attack"; }

		void Enter(Worker& agent) override
		{
			ASSERT((agent.mTowardStructure != nullptr || agent.mTowardUnit!= nullptr), "[Worker] enter Attack state without have a target");

			if (agent.mTowardStructure != nullptr)
			{
				mIsTargetingStructure = true;
			}
			else
			{
				mIsTargetingStructure = false;
			}
			mTime = 0.0f;
			mAttackPosition = agent.position;
		}
		void Update(Worker& agent, float deltaTime) override
		{
			if (mIsTargetingStructure)
			{
				if (!agent.mTowardStructure->mIsActive)
				{
					agent.stateMachine->ChangeState("Idle");
					return;
				}
				
				auto building = static_cast<Building*>(agent.mTowardStructure);
				if(building)
					agent.Attack(*building, deltaTime);
			}
			else
			{
				if (!agent.mTowardUnit->mIsActive)
				{
					agent.stateMachine->ChangeState("Idle");
					return;
				}
				
				if (NFGE::Math::DistanceSqr(agent.position, agent.mTowardUnit->position) > agent.mStatus.mAttackRange * agent.mStatus.mAttackRange)
				{
					agent.stateMachine->ChangeState("Walk");
					return;
				}

				agent.Attack(*agent.mTowardUnit, deltaTime);
			}
			mTime += deltaTime * 10.0f;
			agent.position.x = mAttackPosition.x + sin(mTime * 2.0f) * 0.0f;
			agent.position.y = mAttackPosition.y + cos(mTime * 2.0f) * 1.0f;
		}
		void Exit(Worker& agent) override
		{
			
		}
	private:
		
		bool mIsTargetingStructure;
		float mTime;
		Vector2 mAttackPosition;
	};

	struct WalkState : public AI::State<Worker>
	{
		static std::string GetName() { return "Walk"; }

		void Enter(Worker& agent) override
		{
			TextureGridMap& worldMap = *(LevelManager::Get()->myBattleMapPtr);
			mPathFollowBehavior = agent.steeringModule->GetBehavior < AI::PathFollowingBehavior>("PathFollow");
												
			int start = worldMap.ConvertToMapIndex(agent.position);
			int end = worldMap.ConvertToMapIndex(agent.destination);
			worldMap.FindPath(start, end);
			if (worldMap.GraphContext.found)
			{
				LoadThePath(agent, worldMap.GraphContext);
				agent.destination = mPathFollowBehavior->path.back();
				agent.velocity = mLastVelocity;

				mPathFollowBehavior->SetActive(true);
				mPathFollowBehavior->arriveDistance = worldMap.textureSize.x * worldMap.xScale * 0.5f;				// Optimasieable by put into into init
				mPathFollowBehavior->switchPointDistance = worldMap.textureSize.x * worldMap.xScale * 0.5f;			// Optimasieable by put into into init
				mTargetBuildingPos = mPathFollowBehavior->path.front();
			}
			mHasChangedPath = true;
		}
		void Update(Worker& agent, float deltaTime) override
		{
			TextureGridMap& worldMap = *(LevelManager::Get()->myBattleMapPtr);

			auto building = dynamic_cast<Building*>(agent.mTowardStructure);
			auto unit = dynamic_cast<Unit*>(agent.mTowardUnit);

			if (mPathFollowBehavior->path.empty())
			{
				mHasChangedPath = false;
				agent.stateMachine->ChangeState("Idle");
			}
			else if ((mPathFollowBehavior->path.back().x != agent.destination.x) || (mPathFollowBehavior->path.back().y != agent.destination.y))
			{
				agent.stateMachine->ChangeState("Walk");
			}
			else if((building != nullptr) && (building->mCommanderIndex != agent.mCommanderIndex))
			{

				if (NFGE::Math::DistanceSqr(agent.position, mTargetBuildingPos) < agent.mStatus.mAttackRange * agent.mStatus.mAttackRange)
				{
					auto expectedDir = NFGE::Math::Normalize(mTargetBuildingPos - agent.position);

					agent.heading += (expectedDir - agent.heading) * 0.8f * deltaTime;

					if (NFGE::Math::Dot(agent.heading, expectedDir) > 0.9f)			//|
						agent.stateMachine->ChangeState("Attack");					//|--- For rotate the heading to aim the target
				}

			}
			else if ((unit != nullptr) && (unit->mCommanderIndex != agent.mCommanderIndex))
			{

				if (NFGE::Math::Distance(agent.mTowardUnit->position, agent.position) - agent.mStatus.mAttackRange > mChaseAccuracy)	//|
				{																														//|
					agent.stateMachine->ChangeState("Walk");																			//|--- Redo A* when ever unit move out of accuracy, EXPENSIVE
				}																														//|

				if (NFGE::Math::DistanceSqr(agent.position, unit->position) < agent.mStatus.mAttackRange * agent.mStatus.mAttackRange)
				{
					auto expectedDir = NFGE::Math::Normalize(unit->position - agent.position);

					agent.heading += (expectedDir - agent.heading) * 0.8f * deltaTime;

					if (NFGE::Math::Dot(agent.heading, expectedDir) > 0.9f)			//|
						agent.stateMachine->ChangeState("Attack");					//|--- For rotate the heading to aim the target
				}
			}
		}
		void Exit(Worker& agent) override
		{
			mPathFollowBehavior->path.clear();
			mLastVelocity = agent.velocity;
			agent.velocity = Vector2{};
			mPathFollowBehavior->ArriveBehavior::SetActive(false);

			if (agent.mIsGarthing && !mHasChangedPath)
			{
				switch (agent.mTowardStructure->mType)
				{
				case StructureType::Resource:
				{
					agent.GoInCity();
				}
					break;
				case StructureType::Building:
				{
					agent.HandInSalary();
				}
					break;
				default:
					break;
				}
			}

		}
	private:

		void LoadThePath(Worker& agent, const AI::GraphSearch::Context& graphContext)																	  //|
		{																																														  //|
			ASSERT(mPathFollowBehavior != nullptr, "[Worker] Starting path follow behavior with null behaviorPtr");																						  //|
			auto next = graphContext.closedList.back();																																			  //|
			while (next != AI::InvalidNode)																																						  //|
			{																																													  //|
				mPathFollowBehavior->path.push_back(agent.mWorldMap->gridGraph.GetNode(next).position);																									  //|
				next = graphContext.parent[next];																																				  //|
			}																																													  //|
		}																																														  //|
		void StartPathFollowingBehavior(Worker& agent, const Vector2& lastVelocity, AI::PathFollowingBehavior*& behaviroPtr)																		  //|
		{																																														  //|
																																																  //|
		}
		int GetCloseTile(Worker& agent, const std::vector<int>& tiles)																															  //|
		{																																														  //|
			int workerTile = agent.mWorldMap->ConvertToMapIndex(agent.position);																												  //|
			Vector2 tileIndexPos = agent.mWorldMap->GetRowAndCol(workerTile);																													  //|
																																																  //|
			float closeDist = FLT_MAX;																																							  //|
			int ret;																																											  //|
			for (auto& tile : tiles)																																							  //|
			{																																													  //|
				float currentDist = EngineWrapper::DistanceSqr(agent.mWorldMap->GetRowAndCol(tile), tileIndexPos);																				  //|
				if (currentDist < closeDist)																																					  //|
				{																																												  //|
					closeDist = currentDist;																																					  //|
					ret = tile;																																									  //|
				}																																												  //|
			}																																													  //|
			return ret;																																											  //|
		}																																														  //|
																																																  //|
		

		Vector2 RandomCirclePosInTile(size_t tileIndex)
		{
			TextureGridMap& worldMap = *(LevelManager::Get()->myBattleMapPtr);
			return worldMap.gridGraph.GetNode(tileIndex).position + NFGE::Math::RandomUnitCircle() * 0.5f;
		}
		AI::PathFollowingBehavior* mPathFollowBehavior;
		Vector2 mTargetBuildingPos;
		Vector2 mLastVelocity{};
		bool mHasChangedPath = false;
		float mChaseAccuracy = 10.0f;
	};
}


int Worker::mHealthBarSliceCount = 5;
LCTexture Worker::mMoneyTexture = LCTexture();

Worker::Worker()
	:Unit(myBattleMap)
{	

	if(mMoneyTexture.mTextureId == 0)
		mMoneyTexture.Load(moneyTextureSize, { 0.5f,0.5f }, moneyTexture);
}

void Worker::Load(TextureGridMap& worldMap, const Vector2& initPos, int commanderIndex, const Vector2& size, float maxHealth)
{
	Unit::Load(worldMap,initPos,size, maxHealth,commanderIndex);
	
	mHealthBar.Load(mHealthBarSliceCount);
	stateMachine = std::make_unique<AI::StateMachine<Worker>>(*this);

	mCurrentHealth = 0;
	mCurrentDirection = 0;

	destination = Vector2(initPos.x + 1.0f, initPos.y + 1.0f);

	//Add steering behaviro
	//steeringModule->AddBehavior < AI::SeekBehavior >("Seek")->SetActive(true);
	steeringModule->AddBehavior < AI::PathFollowingBehavior >("PathFollow")->ArriveBehavior::SetActive(true);
	//Add state for statmachine
	stateMachine->AddState<IdleState>();
	stateMachine->AddState<WalkState>();
	stateMachine->AddState<GathringState>();
	stateMachine->AddState<AttackState>();
	//Add clip into animator
}

void Worker::Update(float deltaTime)
{
	if (mIsBuilding || mIsWorking)
		return;
	Unit::Update(deltaTime);

	mHealthBar.Update(mCurrentHealth / mMaxHealth);

	stateMachine->Update(deltaTime);
}

void Worker::ControlUpdate(const Vector2& currentMousePos, float deltaTime)
{
	//Unit::ControlUpdate(currentMousePos, deltaTime);
	if (mIsBuilding || mIsWorking)
		return;
	if (IsMousePressed(MouseButton::RBUTTON))
	{
		mTowardUnit = false;
		mTowardStructure = false;

		if (LevelManager::Get()->mPlayerController.mCurrentHighLightResource != nullptr)
		{
			// to do cai kuang
			mIsGarthing = true;
			mTowardStructure = LevelManager::Get()->mPlayerController.mCurrentHighLightResource;
			stateMachine->ChangeState("Gathring");
		}
		else if (LevelManager::Get()->mPlayerController.mCurrentHighLightStructure != nullptr)
		{
			mIsGarthing = false;
			auto building = reinterpret_cast<Building*>(LevelManager::Get()->mPlayerController.mCurrentHighLightStructure);
			mTowardStructure = building;
			if ((building->mCommanderIndex == mCommanderIndex) && (mCurrentCarryMoney > 0))
			{
				mIsGarthing = true;
				stateMachine->ChangeState("Gathring");
			}
			else
			{
				int choseTile = GetCloseTile(*this, mTowardStructure->mGetInTile);
				destination = mWorldMap->gridGraph.GetNode(choseTile).position;
				stateMachine->ChangeState("Walk");
			}
			// to do go to 
		}
		else if (LevelManager::Get()->mPlayerController.mCurrentHighLightUnit != nullptr)
		{
			mIsGarthing = false;
			auto unit = reinterpret_cast<Unit*>(LevelManager::Get()->mPlayerController.mCurrentHighLightUnit);
			mTowardUnit = unit;			//Turn goto Unit off
			if (unit->mCommanderIndex != mCommanderIndex)
			{
				int choseTile = GetCloseTile(*this, mTowardUnit->position);
				destination = mWorldMap->gridGraph.GetNode(choseTile).position;
				stateMachine->ChangeState("Walk");
			}
		}
		else
		{
			mIsGarthing = false;
			destination = LevelManager::Get()->mWorldCamera->ConvertToWorld(currentMousePos);
			stateMachine->ChangeState("Walk");
		}

	}
}

void Worker::Render(const Camera2D & camera)
{
	if (mIsBuilding || mIsWorking)
		return;

	Vector2 positionWithAltitude = { position.x, position.y + mAltitude };

	Vector2 drawPos = camera.ConvertToScreen(positionWithAltitude);
	if (!camera.IsOutOfView(drawPos, { mSize.x * camera.GetScale(), mSize.y * camera.GetScale() }))
	{
		if (mAnimator.mClipCount != 0)
		{
			//DrawSprite(mAnimator.GetCurrentTextureWithDirection(mCurrentDirection), drawPos, 0.0f, 1.0f, 0.5f, 0.5f, mDrawScale.x * camera.GetScale(), mDrawScale.y * camera.GetScale());
			mAnimator.Render(mCurrentDirection, drawPos, 0.0f,1.0f,0.5f,0.5f, mDrawScale.x, mDrawScale.y, camera);
			//DrawScreenCircle(drawPos, 5.0f, Red);
		}
		else
		{
			DrawSprite(GetDefaultTexture(mCurrentDirection), drawPos, 0.0f, 1.0f, 0.5f, 0.5f, mDrawScale.x * camera.GetScale(), mDrawScale.y * camera.GetScale());
		}
	}


	if (mIsSelected || mIsMouseOnTop)
	{
		float healthBarLength = mDefaultWidth * mDrawScale.x * camera.GetScale();
		Vector2 toBottomLeft = { -mDefaultWidth * mDrawScale.x  * 0.5f, mDefaultHeight * mDrawScale.y * 0.5f };
		Vector2 toTopLeft = { 0.0f, -mDefaultHeight * mDrawScale.y};
		mHealthBar.Render(camera.ConvertToScreen(positionWithAltitude + toBottomLeft + toTopLeft), healthBarLength, camera.GetScale());
	}

	if (mCurrentCarryMoney > 0)
	{
		Vector2 moneyDrawPos = { positionWithAltitude.x - mSize.x * 0.5f ,positionWithAltitude.y - mSize.y * 0.5f };
		mMoneyTexture.Render(moneyDrawPos, camera, false);
	}
}

void Worker::DebugUIUpdate(float deltaTime, const Camera2D & camera)
{
	Unit::DebugUIUpdate(deltaTime, camera);
}

void Worker::Unload()
{
}

bool Worker::Work(float deltaTime)
{
	if (mWorkTimeCounter <= 0.0f)
	{
		mWorkTimeCounter = mTotalWorkTime;
		return true;
	}
	mWorkTimeCounter -= deltaTime;
	return false;
	
}

void Worker::GoInCity()
{
	Resource* theCity = reinterpret_cast<Resource*>(mTowardStructure);
	theCity->mWorkers.push_back(this);
	mIsWorking = true;
	mLastWorkedCity = mTowardStructure;
}

void Worker::GetOutCity()
{
	mIsWorking = false;
	
	auto& homeStructure = LevelManager::Get()->mCommanders[mCommanderIndex].myBuilding;
	if (!homeStructure.empty())
	{
		mTowardStructure = homeStructure.front();
	}
	else
	{
		mTowardStructure = nullptr;
	}
	stateMachine->ChangeState("Gathring");
}

void Worker::HandInSalary()
{
	LevelManager::Get()->mCommanders[mCommanderIndex].mCurrnetMoney += mCurrentCarryMoney;
	mCurrentCarryMoney = 0;

	ASSERT(mLastWorkedCity != nullptr, "[Worker] try to go to lastworkedcity when its nullptr");
	mTowardStructure = mLastWorkedCity;

}

