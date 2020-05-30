#include "TinyFish.h"
#include "PreditorFish.h"

using namespace NFGE::Math;
using namespace NFGE::Graphics;

extern BehaviorControl behaviorControl;
extern bool isDrawDebugLine;
extern int currentTinyFish;

PTCone TinyFish::mGeometry;
Graphics::Color TinyFish::mRegularColor = Graphics::Colors::Gray;
Graphics::Color TinyFish::mHappyColor= Graphics::Colors::LightGreen;
Graphics::Color TinyFish::mPanicColor = Graphics::Colors::OrangeRed;
Graphics::Color TinyFish::mDefaultAmbient = Graphics::Colors::DarkGray;

BehaviorControl FleeGrounpBehavior{1.5f, 1.5f, 1.0f, 1.0f, 5.0f, 200.0f};
BehaviorControl EatGrounpBehavior{ 2.5f, 2.5f, 3.5f, 1.0f, 5.0f, 200.0f };
BehaviorControl RegularGrounpBehavior{ 3.5f, 3.5f, 3.5f, 1.0f, 5.0f, 200.0f };

//BehaviorControl FleeGrounpBehavior{ 1.0f, 1.5f, 1.5f, 1.0f, 5.0f, 100.0f };
//BehaviorControl EatGrounpBehavior{ 1.0f, 1.5f, 1.5f, 1.0f, 5.0f, 100.0f };
//BehaviorControl RegularGrounpBehavior{ 1.0f, 1.5f, 1.5f, 1.0f, 5.0f, 100.0f };

namespace
{
	const int MeshRow = 5;
	const int MeshCol = 8;
	const float MeshRadius = 0.72f;
	const wchar_t* MeshShaderFileName = L"../../Assets/Shaders/Standard_Material.fx";
}

class TinyFish_VisualSensor : public AI::Sensor3D
{
public:
	bool CanSee(const Vector3& point, AI::Agent3D& agent)
	{
		// before was checking is vision block by any plane
		// In thid demo we dont have any plane
		return true;
	}

	template<class EntityType>
	void RefreshMemory(AI::MemoryRecord& memory, EntityType* entity)
	{
		auto success = memory.EditProporty<Vector3>("LastSeenPosition", entity->position);
		if (!success)  // Other sensor add the memory, so have to add LastSeenPosition
			memory.AddProperty<Vector3>("LastSeenPosition", entity->position);
		memory.lastRecordTime = 0.0f;
	}

	template<class EntityType>
	void AddMemory(AI::MemoryRecords& memories, EntityType* entity)
	{
		AI::MemoryRecord newMemory;
		newMemory.AddProperty<int>("EntityId", entity->mId);
		newMemory.AddProperty<Vector3>("LastSeenPosition", entity->position);
		newMemory.lastRecordTime = 0.0f;

		memories.push_back(newMemory);
	}

	template<class EntityType>
	void TryRefreshMemoryWithSeenEntities(AI::MemoryRecord& memory, EntityType* entityInMemory, std::vector<EntityType*>& seenEntities)
	{
		for (auto iter = seenEntities.begin(); iter != seenEntities.end(); ++iter)
		{
			auto currentSeen = *iter;
			if (currentSeen == entityInMemory)
			{
				RefreshMemory(memory, entityInMemory);
				seenEntities.erase(iter);
				break;
			}
		}
	}

	template<class EntityType>
	void RememberSeenEntities(AI::MemoryRecords& memories, const std::vector<EntityType*>& seenEntities)
	{
		for (auto& entity : seenEntities)
		{
			AddMemory(memories, entity);
		}
	}

	template<class EntityType>
	std::vector<EntityType*> ObserveEntity(float viewRangeSqr, float cosViewAngle, AI::Agent3D& agent)
	{
		std::vector<EntityType*> ret;
		const std::vector<EntityType*> Entities = agent.mWorld.GetEntity3Ds<EntityType>();
		if (!Entities.empty())
		{
			for (auto entity : Entities)
			{
				if (!entity->mIsActive)
					continue;
				if (DistanceSqr(agent.position, entity->position) > viewRangeSqr)
					continue;

				if (Dot(agent.heading, Normalize(entity->position - agent.position)) < cosViewAngle)
					continue;

				if (!CanSee({ entity->position.x, 0.0f,entity->position.y }, agent))
					continue;

				ret.push_back(entity);
			}
		}
		return ret;
	}

	void Update(AI::Agent3D& agent, AI::MemoryRecords& memories, float deltaTime) override
	{

		auto viewRangeSqr = NFGE::Math::Sqr(viewRange);
		auto cosViewAngle = cosf(viewAngle * 0.5f);

		std::vector<SeeWeed*> seenMinerals = ObserveEntity<SeeWeed>(viewRangeSqr, cosViewAngle, agent);
		std::vector<PreditorFish*> seenCarriers = ObserveEntity<PreditorFish>(viewRangeSqr, cosViewAngle, agent);

		for (auto& memory : memories)
		{
			auto entityInMemory = AI::Entity3D::mAllEntity[memory.GetProperty<int>("EntityId")];
			if (entityInMemory->GetTypeName() == SeeWeed::GET_TYPE_NAME())
			{
				TryRefreshMemoryWithSeenEntities(memory, reinterpret_cast<SeeWeed*>(entityInMemory), seenMinerals);
			}
			else if (entityInMemory->GetTypeName() == PreditorFish::GET_TYPE_NAME())
			{
				TryRefreshMemoryWithSeenEntities(memory, reinterpret_cast<PreditorFish*>(entityInMemory), seenCarriers);
			}
		}

		//Add new memory for left over seen entity 
		RememberSeenEntities(memories, seenMinerals);
		RememberSeenEntities(memories, seenCarriers);
	}

	float viewRange = 500.0f;
	float viewAngle = 6.0f;
};

struct TinyFish_SurviveState : public AI::State<TinyFish>
{
	static std::string GetName() { return "Survive"; }

	void Enter(TinyFish& agent) override
	{

	}
	void Update(TinyFish& agent, float deltaTime) override
	{
		auto& records = agent.mPerceptionModule->GetMemoryRecords();
		auto wander = agent.steeringModule->GetBehavior<AI::WanderBehavior3D>("Wander");
		auto flee = agent.steeringModule->GetBehavior<AI::FleeBehavior3D>("Flee");
		auto seek = agent.steeringModule->GetBehavior<AI::SeekBehavior3D>("Seek");
		auto arrive = agent.steeringModule->GetBehavior<AI::ArriveBehavior3D>("Arrive");
		if (!records.empty())
		{
			wander->SetActive(false);
			auto topMemoryRecord = records.front();

			int entityIndex = topMemoryRecord.GetProperty<int>("EntityId");
			auto& theEntityInMemory = AI::Entity3D::mAllEntity[entityIndex];

			
			if (theEntityInMemory->GetTypeName() == PreditorFish::GET_TYPE_NAME())
			{
				wander->SetActive(false);

				auto lastSeenPos = topMemoryRecord.GetProperty<Vector3>("LastSeenPosition");
				if (NFGE::Math::Distance(agent.position, lastSeenPos) < flee->panicDistance)
				{
					flee->SetActive(true);
					flee->SetWeight(5.0f);
					arrive->SetActive(false);
					seek->SetActive(false);
					flee->panicPoint = lastSeenPos;
					//flee->panicPoint = theEntityInMemory->position;

					if (mLastSeenType != theEntityInMemory->GetTypeName())
						agent.mDiffuseColorLerper.Start(agent.mDiffuseColor, TinyFish::mPanicColor, 0.5f);
					mLastSeenType = theEntityInMemory->GetTypeName();

					agent.UpdateBehaviorControl(FleeGrounpBehavior);
				}
				else
				{
					records.pop_front();
					flee->SetActive(false);
				}
				

				
			}
			else if (theEntityInMemory->GetTypeName() == SeeWeed::GET_TYPE_NAME())
			{
				Vector3 minaralPos = topMemoryRecord.GetProperty<Vector3>("LastSeenPosition");

				flee->SetActive(false);				
				wander->SetActive(false);
				arrive->SetActive(true);
				seek->SetActive(true);
				agent.destination = minaralPos;
				//SimpleDraw::AddLine({ agent.destination .x, agent.radius, agent.destination.y}, { agent.position.x, agent.radius, agent.position.y }, Colors::OrangeRed);

				AI::Entity3D* temp = theEntityInMemory;
				mTargetSeeWeed = reinterpret_cast<SeeWeed*>(temp);
				if ((agent.TryCollectSeeWeed(*mTargetSeeWeed)) || (mTargetSeeWeed->mIsGetEatting) )
				{
					arrive->SetActive(false);
					seek->SetActive(false);
					records.pop_front();
					agent.mDiffuseColorLerper.Start(agent.mDiffuseColor, TinyFish::mRegularColor, 0.5f);
					mLastSeenType = "";
					agent.UpdateBehaviorControl(RegularGrounpBehavior);
				}
				else
				{
					if (mLastSeenType != theEntityInMemory->GetTypeName())
						agent.mDiffuseColorLerper.Start(agent.mDiffuseColor, TinyFish::mHappyColor, 0.5f);
					mLastSeenType = theEntityInMemory->GetTypeName();
					agent.UpdateBehaviorControl(EatGrounpBehavior);
				}

			}
			
		}
		else
		{
			flee->SetActive(false);
			arrive->SetActive(false);
			seek->SetActive(false);
			wander->SetActive(true);
			
			if(mLastSeenType != "")
				agent.mDiffuseColorLerper.Start(agent.mDiffuseColor, TinyFish::mRegularColor, 0.5f);
			mLastSeenType = "";
			agent.UpdateBehaviorControl(RegularGrounpBehavior);
		}
		
	}
	void Exit(TinyFish& agent) override
	{
	}
private:
	SeeWeed * mTargetSeeWeed;
	std::string mLastSeenType;
};

TinyFish::TinyFish(AI::AIWorld3D & world)
	:PTEntity_Shell(world)
{
	if (!mGeometry.mIsInitialize)
	{
		mGeometry.Initialize(MeshRow, MeshCol, MeshRadius, MeshShaderFileName, MeshShaderFileName);
	}
}

void TinyFish::Initialize(float width, float height, float depth)
{
	maxSpeed = 300.0f;
	steeringModule = std::make_unique<AI::SteeringModule3D>(*this);
	//---- Seek --------------------------------------------------------------------------------------------------------
	steeringModule->AddBehavior < AI::SeekBehavior3D >("Seek")->SetActive(false);
	steeringModule->AddBehavior < AI::FleeBehavior3D >("Flee")->SetActive(false);
	steeringModule->GetBehavior < AI::FleeBehavior3D >("Flee")->panicDistance = 350.0f;
	//------------------------------------------------------------------------------------------------------------------

	//---- Wander ------------------------------------------------------------------------------------------------------
	auto wanderBehavior = steeringModule->AddBehavior < AI::WanderBehavior3D >("Wander");
	wanderBehavior->SetParama(80.0f, 20.0f, 1000.0f);
	wanderBehavior->SetActive(true);
	//------------------------------------------------------------------------------------------------------------------

	//---- Arrive ------------------------------------------------------------------------------------------------------
	steeringModule->AddBehavior < AI::ArriveBehavior3D >("Arrive")->SetActive(false);
	steeringModule->GetBehavior < AI::ArriveBehavior3D >("Arrive")->SetWeight(1.0f);
	//------------------------------------------------------------------------------------------------------------------

	//---- ObsAvoid      -----------------------------------------------------------------------------------------------
	auto obsAvioid = steeringModule->AddBehavior < AI::ObstacleAvoidBehavior3D >("ObsAvoid");

	obsAvioid->SetActive(true);
	obsAvioid->SetWeight(10.0f);
	//------------------------------------------------------------------------------------------------------------------
	
	//---- WallAvoid      -----------------------------------------------------------------------------------------------
	auto wallAvioid = steeringModule->AddBehavior < AI::WallAvoidBehavior3D>("wallAvoid");
	wallAvioid->SetDebugDraw(true);
	wallAvioid->SetActive(true);
	wallAvioid->SetWeight(10.0f);
	//------------------------------------------------------------------------------------------------------------------

	//---- GroupBehavior_Sepreation ------------------------------------------------------------------------------------
	auto GroupSeparation = steeringModule->AddBehavior < AI::GroupSeparationBehavior3D>("GroupSeparation");
	GroupSeparation->SetActive(true);
	GroupSeparation->SetWeight(1.0f);
	//------------------------------------------------------------------------------------------------------------------

	//---- GroupBehavior_Alignment -------------------------------------------------------------------------------------
	auto GroupAlignment = steeringModule->AddBehavior < AI::GroupAlignmentBehavior3D>("GroupAlignment");
	GroupAlignment->SetActive(true);
	GroupAlignment->SetWeight(1.0f);
	//------------------------------------------------------------------------------------------------------------------

	//---- GroupBehavior_Cohesion --------------------------------------------------------------------------------------
	auto GroupCohesion = steeringModule->AddBehavior < AI::GroupCohesionBehavior3D>("GroupCohesion");
	GroupCohesion->SetActive(true);
	GroupCohesion->SetWeight(1.0f);
	//------------------------------------------------------------------------------------------------------------------

	mPerceptionModule = std::make_unique<AI::PerceptionModule3D>(*this, [this](AI::MemoryRecord& memory) {
		auto entityInMemory = AI::Entity3D::mAllEntity[memory.GetProperty<int>("EntityId")];
		if (entityInMemory->GetTypeName() == PreditorFish::GET_TYPE_NAME())
		{
			if(NFGE::Math::Distance(entityInMemory->position, position) < 400.0f)
				memory.importance = 3.0f;
			else
				memory.importance = 0.0f;
		}
		else if (entityInMemory->GetTypeName() == SeeWeed::GET_TYPE_NAME())
		{
			auto theSeeWeed = reinterpret_cast<SeeWeed*>(entityInMemory);
			memory.importance = 1.0f - NFGE::Math::Distance(entityInMemory->position, position) / 500.0f;
		}
	});
	mPerceptionModule->AddSensor<TinyFish_VisualSensor>("Vision");
	mPerceptionModule->SetMemorySpan(2.0f);

	mStateMachine = std::make_unique<AI::StateMachine<TinyFish>>(*this);
	mStateMachine->AddState<TinyFish_SurviveState>();

	neighbourRange = 100.0f;
	radius = 10.0f;

	worldWidth = width;
	worldHeight = height;
	worldDepth = depth;
	position = NFGE::Math::Vector3(200.0f, 200.0f, 200.0f);
	velocity = NFGE::Math::Vector3(0.0f, 1.0f, 0.0f);
	mDiffuseColor = mRegularColor;
	mAmbientColor = mDefaultAmbient;
}

void TinyFish::Terminate()
{
}

void TinyFish::OnUpdate( float deltaTime)
{
	mPerceptionModule->Update(deltaTime);
	//if(mPerceptionModule->GetMemoryRecords().empty())
		//mDiffuseColorLerper.Start(mDiffuseColor, TinyFish::mRegularColor, 0.5f);

	mStateMachine->Update(deltaTime);
	
	//UpdateBehaviorControl(behaviorControl);

	steeringModule->GetBehavior< AI::WanderBehavior3D >("Wander")->SetWeight(mBehaviorControl.mWanderWeight);
	steeringModule->GetBehavior< AI::GroupSeparationBehavior3D>("GroupSeparation")->SetWeight(mBehaviorControl.mSeparationWeight);
	steeringModule->GetBehavior< AI::GroupCohesionBehavior3D>("GroupCohesion")->SetWeight(mBehaviorControl.mCohesionWeight);
	steeringModule->GetBehavior< AI::GroupAlignmentBehavior3D>("GroupAlignment")->SetWeight(mBehaviorControl.mAlignmentWeight);
	steeringModule->GetBehavior< AI::ObstacleAvoidBehavior3D>("ObsAvoid")->SetWeight(mBehaviorControl.mObsAvoidWeight);
	neighbourRange = mBehaviorControl.mNeighborRadius;

	UpdateNeighbor();
	auto force = steeringModule->Calculate();
	auto acceleration = force / mass;
	velocity += acceleration * deltaTime;
	// TODO add truncate method here
	position += velocity * deltaTime;

	if (NFGE::Math::MagnitudeSqr(velocity) > 1.0f)
		heading = NFGE::Math::Normalize(velocity);

	ClampInScene();

	mIsDebugDraw = isDrawDebugLine;
}

void TinyFish::Render()
{
	auto& camera = NFGE::sApp.GetMainCamera();
	Render_2D();

	auto sphereRadius = radius * 0.5f;
	mGeometry.Render({ position.x, position.y, position.z }, heading, { sphereRadius, sphereRadius, sphereRadius }, camera, mDiffuseColor, mAmbientColor);
}

void TinyFish::DebugDraw()
{
	
}

void TinyFish::ResetPosition(const Vector3 & pos)
{
	position = pos;
}

bool TinyFish::TryCollectSeeWeed(SeeWeed & SeeWeed)
{
	if (NFGE::Math::PointInSphere(SeeWeed.position, { position, radius + 5.0f }) && SeeWeed.mIsGetEatting == false)
	{
		for (int i = 0; i < 10; i++)
		{
			BornAChild();
		}
		SeeWeed.GetEat(this);
		return true;
	}
	return false;
}

void TinyFish::BornAChild()
{
	auto newFish = PTEntityManager<TinyFish>::Get()->GetNext();
	if (newFish == nullptr)
		return;
	newFish->ResetPosition(position);
	newFish->mIsActive = true;
	++currentTinyFish;
}
