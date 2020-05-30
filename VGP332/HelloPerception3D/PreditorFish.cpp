#include "PreditorFish.h"
#include "TinyFish.h"

using namespace NFGE::Graphics;
using namespace NFGE::Math;

extern bool isDrawDebugLine;
extern int currentTinyFish;

PTDynamicCone PreditorFish::mGeometry;
namespace
{
	const int MeshRow = 32;
	const int MeshCol = 32;
	const float MeshHeight = 4.0f;
	const float MeshRadius = 3.0f;
	const wchar_t* MeshShaderFileName = L"../../Assets/Shaders/Dynamic_Wave_Geometry.fx";
	const float PreditorRadius = 30.0f;
}

struct PredictorFish_SurviveState : public AI::State<PreditorFish>
{
	static std::string GetName() { return "Survive"; }

	void Enter(PreditorFish& agent) override
	{

	}
	void Update(PreditorFish& agent, float deltaTime) override
	{
		Agent3Ds tinyFishes;
		agent.mWorld.GetNeighborhood(tinyFishes, {agent.position, agent.mDetectedRange});
		for (auto& tinyFish : tinyFishes)
		{
			auto currentFish = dynamic_cast<TinyFish*>(tinyFish);
			if (currentFish == nullptr)
				continue;
			if (!currentFish->mIsActive)
				continue;

			SimpleDraw::AddLine({ currentFish->position.x, currentFish->position.y, currentFish->position.z}, { agent.position.x, agent.position.y, agent.position.z }, Colors::HotPink);

			if (DistanceSqr(currentFish->position, agent.position) < Sqr(agent.mKillRange))
			{
				SimpleDraw::AddLine({ currentFish->position.x, currentFish->position.y, currentFish->position.z }, { agent.position.x, agent.position.y, agent.position.z }, Colors::PaleVioletRed);
				currentFish->mIsActive = false;
				--currentTinyFish;
			}
		}
	}
	void Exit(PreditorFish& agent) override
	{
	}
private:
};

PreditorFish::PreditorFish(AI::AIWorld3D & world)
	:PTEntity_Shell(world)
{
	if (!mGeometry.mIsInitialize)
		mGeometry.Initialize(MeshRow, MeshCol, MeshRadius, MeshHeight, 2.0f, MeshShaderFileName, MeshShaderFileName);
}

void PreditorFish::Initialize(float width, float height, float depth)
{
	maxSpeed = 150.0f;
	steeringModule = std::make_unique<AI::SteeringModule3D>(*this);
	//---- Wander ------------------------------------------------------------------------------------------------------
	auto wanderBehavior = steeringModule->AddBehavior < AI::WanderBehavior3D >("Wander");
	wanderBehavior->SetParama(200.0f, 50.0f, 100.0f);
	wanderBehavior->SetActive(false);
	//------------------------------------------------------------------------------------------------------------------

	//---- OffSetPursuit -----------------------------------------------------------------------------------------------
	auto offSetPursuit = steeringModule->AddBehavior<AI::OffsetPursuitBehavior3D<AI::ConeTeamFormContext>>("OffsetPursuit");
	offSetPursuit->SetActive(false);
	offSetPursuit->SetWeight(1.25f);
	//------------------------------------------------------------------------------------------------------------------
	
	mStateMachine = std::make_unique<AI::StateMachine<PreditorFish>>(*this);
	mStateMachine->AddState<PredictorFish_SurviveState>();

	worldWidth = width;
	worldHeight = height;
	worldDepth = depth;
	position = NFGE::Math::Vector3(500.0f, 500.0f, 500.0f);
	velocity = NFGE::Math::Vector3(0.0f, 1.0f, 0.0f);
	mDiffuseColor = NFGE::Graphics::Colors::Yellow;

	radius = PreditorRadius;
}

void PreditorFish::Terminate()
{

}

void PreditorFish::OnUpdate(float deltaTime)
{
	maxSpeed = 150.0f + currentTinyFish * 2.0f;
	//mDetectedRange = currentTinyFish / 4.0f + 50.0f;
	//mKillRange = currentTinyFish / 5.0f + 40.0f;

	mStateMachine->Update(deltaTime);

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

void PreditorFish::Render()
{
	auto& camera = NFGE::sApp.GetMainCamera();
	Render_2D();

	auto sphereRadius = radius * 0.5f;
	mGeometry.Render({ position.x,  position.y, position.z }, heading, { sphereRadius, sphereRadius, sphereRadius }, camera);

}

void PreditorFish::DebugDraw()
{
}


void PreditorFish::ResetPosition(const NFGE::Math::Vector3 & pos)
{
	position = pos;
}
