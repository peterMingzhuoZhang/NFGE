#include "SCV.h"
#include "Carrier.h"
#include "PolygonGridGraphMap.h"

class VisualSensor : public AI::Sensor
{
public:
	bool CanSee(const Vector2& point, AI::Agent& agent)
	{
		for (auto& wall : agent.mWorld.GetWalls())
		{
			if (EngineWrapper::Intersect({ point, agent.position }, wall))
			{
				DrawScreenLine(point, agent.position, Colors::AliceBlue);
				return false;
			}
		}

		DrawScreenLine(point, agent.position, Colors::OrangeRed);
		return true;
	}

	template<class EntityType>
	void RefreshMemory(AI::MemoryRecord& memory, EntityType* entity)
	{
		auto success = memory.EditProporty<Vector2>("LastSeenPosition", entity->position);
		if (!success)  // Other sensor add the memory, so have to add LastSeenPosition
			memory.AddProperty<Vector2>("LastSeenPosition", entity->position);
		memory.lastRecordTime = 0.0f;
	}

	template<class EntityType>
	void AddMemory(AI::MemoryRecords& memories, EntityType* entity)
	{
		AI::MemoryRecord newMemory;
		newMemory.AddProperty<int>("EntityId", entity->mId);
		newMemory.AddProperty<Vector2>("LastSeenPosition", entity->position);
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
		for (auto entity : seenEntities)
		{
			AddMemory(memories, entity);
		}
	}

	template<class EntityType>
	std::vector<EntityType*> ObserveEntity(float viewRangeSqr, float cosViewAngle, AI::Agent& agent)
	{
		std::vector<EntityType*> ret;
		const std::vector<EntityType*> Entities = agent.mWorld.GetEntities<EntityType>();
		if (!Entities.empty())
		{
			for (auto entity : Entities)
			{
				if (Math::DistanceSqr(agent.position, entity->position) > viewRangeSqr)
					continue;

				if (Math::Dot(agent.heading, Math::Normalize(entity->position - agent.position)) < cosViewAngle)
					continue;

				if (!CanSee(entity->position, agent))
					continue;
				
				ret.push_back(entity);
			}
		}
		return ret;
	}

	void Update(AI::Agent& agent, AI::MemoryRecords& memories, float deltaTime) override
	{

		auto viewRangeSqr = Math::Sqr(viewRange);
		auto cosViewAngle = cosf(viewAngle * 0.5f);

		//-----------------------------------------------------------------------------------------------------------------------------------------//|
		DrawScreenLine(agent.position, agent.position + Math::Rotate(agent.heading, viewAngle * 0.5f) * viewRange, Colors::White);		   //|
		DrawScreenLine(agent.position, agent.position + Math::Rotate(agent.heading, -viewAngle * 0.5f)* viewRange, Colors::White);		   //|--- Draw Debug View Triangle
		DrawScreenLine(agent.position + Math::Rotate(agent.heading, -viewAngle * 0.5f)* viewRange, agent.position + Math::Rotate(agent.heading, viewAngle * 0.5f)* viewRange, Colors::White);
		//-----------------------------------------------------------------------------------------------------------------------------------------//|

		std::vector<Mineral*> seenMinerals = ObserveEntity<Mineral>(viewRangeSqr, cosViewAngle, agent);
		std::vector<Carrier*> seenCarriers = ObserveEntity<Carrier>(viewRangeSqr, cosViewAngle, agent);
		
		for (auto& memory : memories)
		{
			auto entityInMemory = AI::Entity::mAllEntity[memory.GetProperty<int>("EntityId")];
			if (entityInMemory->GetTypeName() == Mineral::GET_TYPE_NAME())
			{
				TryRefreshMemoryWithSeenEntities(memory, reinterpret_cast<Mineral*>(entityInMemory), seenMinerals);
			}
			else if (entityInMemory->GetTypeName() == Carrier::GET_TYPE_NAME())
			{
				TryRefreshMemoryWithSeenEntities(memory, reinterpret_cast<Carrier*>(entityInMemory), seenCarriers);
			}
		}

		//Add new memory for left over seen entity 
		RememberSeenEntities(memories, seenMinerals);
		RememberSeenEntities(memories, seenCarriers);
	}

	float viewRange = 200.0f;
	float viewAngle = 1.0f;
};

struct SurviveState : public AI::State<SCV>
{
	static std::string GetName() { return "Survive"; }

	void Enter(SCV& agent) override
	{
		
	}
	void Update(SCV& agent, float deltaTime) override
	{
		auto& records = agent.mPerceptionModule->GetMemoryRecords();
		auto wander = agent.mSteeringModule->GetBehavior<AI::WanderBehavior>("Wander");
		if (!records.empty())
		{
			wander->SetActive(false);
			auto topMemoryRecord = records.front();

			int entityIndex = topMemoryRecord.GetProperty<int>("EntityId");
			auto& theEntityInMemory = AI::Entity::mAllEntity[entityIndex];
			if (theEntityInMemory->GetTypeName() == Carrier::GET_TYPE_NAME())
			{
				wander->SetActive(true);
				auto pathfind = agent.mSteeringModule->GetBehavior<AI::PathFollowingBehavior>("PathFollow");
				pathfind->SetActive(false);
				
				auto flee = agent.mSteeringModule->GetBehavior<AI::FleeBehavior>("Flee");
				flee->SetActive(true);
				flee->SetWeight(5.0f);

				auto wallAvoid = agent.mSteeringModule->GetBehavior<AI::WallAvoidBehavior>("WallAvoid");
				wallAvoid->SetWeight(5.0f);

				flee->panicPoint = topMemoryRecord.GetProperty<EngineWrapper::Vector2>("LastSeenPosition");

				DrawScreenCircle(agent.position, 10.0f, Colors::Orange);
			}
			else if (theEntityInMemory->GetTypeName() == Mineral::GET_TYPE_NAME())
			{
				Vector2 minaralPos = topMemoryRecord.GetProperty<EngineWrapper::Vector2>("LastSeenPosition");

				auto flee = agent.mSteeringModule->GetBehavior<AI::FleeBehavior>("Flee");
				flee->SetActive(false);

				auto pathfind = agent.mSteeringModule->GetBehavior<AI::PathFollowingBehavior>("PathFollow");
				agent.mPolyMap->FindPathInPoly(agent.position, minaralPos);
				pathfind->path = agent.mPolyMap->GetPosPath();
				pathfind->SetActive(true);
				auto wallAvoid = agent.mSteeringModule->GetBehavior<AI::WallAvoidBehavior>("WallAvoid");
				wallAvoid->SetWeight(0.0f);

				AI::Entity* temp = theEntityInMemory;
				mTargetMineral = reinterpret_cast<Mineral*>(temp);
				if (agent.TryCollectMinaral(*mTargetMineral))
					records.pop_front();
				DrawScreenCircle(agent.position, 10.0f, Colors::Purple);
			}
		}
		else
		{
			wander->SetActive(true);
			auto wallAvoid = agent.mSteeringModule->GetBehavior<AI::WallAvoidBehavior>("WallAvoid");
			wallAvoid->SetWeight(5.0f);
		}
	}
	void Exit(SCV& agent) override
	{
	}
private:
	Mineral* mTargetMineral;
};

SCV::SCV(AI::AIWorld & world)
	:AI::Agent(world)
{
}

void SCV::Initialize(PolygonGridGraphMap* thePolygonMap)
{
	mPolyMap = thePolygonMap;
	
	maxSpeed = 100.0f;
	mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, [this](AI::MemoryRecord& memory) {
		auto entityInMemory = AI::Entity::mAllEntity[memory.GetProperty<int>("EntityId")];
		if (entityInMemory->GetTypeName() == Carrier::GET_TYPE_NAME())
		{
			memory.importance = 500.0f - Math::Distance(entityInMemory->position, position) / 30.0f;
		}
		else if (entityInMemory->GetTypeName() == Mineral::GET_TYPE_NAME())
		{
			memory.importance = 10.0f - Math::Distance(entityInMemory->position, position) / 100.0f;
		}

	});
	mPerceptionModule->AddSensor<VisualSensor>("Vision");

	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	auto wander = mSteeringModule->AddBehavior<AI::WanderBehavior>("Wander");
	wander->SetParama(50.0f, 100.0f, 270.0f);
	wander->SetActive(true);
	auto seek = mSteeringModule->AddBehavior<AI::PathFollowingBehavior>("PathFollow");
	seek->switchPointDistance = 10.0f;
	seek->SetWeight(1.0f);
	auto flee = mSteeringModule->AddBehavior<AI::FleeBehavior>("Flee");
	flee->SetWeight(1.0f);
	auto obsAvoid = mSteeringModule->AddBehavior<AI::ObstacleAvoidBehavior>("ObstacleAvoid");
	obsAvoid->SetActive(true);
	auto wallAvoid = mSteeringModule->AddBehavior<AI::WallAvoidBehavior>("WallAvoid");
	wallAvoid->SetWeight(5.0f);
	wallAvoid->SetActive(true);

	mStateMachine = std::make_unique<AI::StateMachine<SCV>>(*this);
	mStateMachine->AddState<SurviveState>();

	for (size_t i = 0; i < std::size(mTextureIds); i++)
	{
		char name[256];
		sprintf_s(name, "scv_%02zu.png", i + 1);
		mTextureIds[i] = LoadTexture(name);
	}

	ResetPosition(mPolyMap->mPolygonGridGraph);
}

void SCV::Update(float deltaTime)
{
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	mPerceptionModule->Update(deltaTime);

	mStateMachine->Update(deltaTime);

	//--------------------------------------------------------------------------------------//|
	auto force = mSteeringModule->Calculate();												//|
	auto acceleration = force / mass;														//|
	velocity += acceleration * deltaTime;													//|
	// TODO add truncate method here														//|
	position += velocity * deltaTime;														//| ---- update the steering physics
																							//|
	if (Math::MagnitudeSqr(velocity) > 1.0f)												//|
		heading = Math::Normalize(velocity);												//|
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

void SCV::Render()
{
	float angle = atan2(-heading.x, heading.y) + Math::Constants::Pi;
	int currentDirection = (int)(angle / Math::Constants::TwoPi * 16.0f) % 16;
	NFGE::sApp.DrawSprite(mTextureIds[currentDirection], position);
}

void SCV::ResetPosition(const AI::Graph<PolygoneMapNode>& theGraphMap)
{
	int nextNodeIndex = NFGE::Math::Random() % theGraphMap.GetNodeCount();
	position = theGraphMap.GetNode(nextNodeIndex).position;
}

bool SCV::TryCollectMinaral(Mineral & theMineral)
{
	if (EngineWrapper::PointInCircle(theMineral.position, {position, 20.0f}))
	{
		mValue += theMineral.value;
		theMineral.ResetPosition(mPolyMap->mPolygonGridGraph);
		return true;
	}
	return false;
}
