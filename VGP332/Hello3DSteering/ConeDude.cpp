#include "ConeDude.h"

PTCone ConeDude::mCone;

namespace
{
	const int MeshRow = 8;
	const int MeshCol = 8;
	const float MeshRadius = 1.0f;
	const wchar_t* MeshShaderFileName = L"../../Assets/Shaders/Standard_Material.fx";
}

ConeDude::ConeDude(AI::AIWorld3D & world)
	:AI::Agent3D(world)
{
	if (!mCone.mIsInitialize)
	{
		mCone.Initialize(MeshRow, MeshCol, MeshRadius, MeshShaderFileName, MeshShaderFileName);
	}
}

void ConeDude::Initialize(float width, float height, float depth)
{
	steeringModule = std::make_unique<AI::SteeringModule3D>(*this);
	worldWidth = width;
	worldHeight = height;
	worldDepth = depth;

	maxSpeed = 300.0f;
	//---- Seek --------------------------------------------------------------------------------------------------------
	steeringModule->AddBehavior < AI::SeekBehavior3D >("Seek")->SetActive(false);
	//------------------------------------------------------------------------------------------------------------------

	//---- Wander ------------------------------------------------------------------------------------------------------
	auto wanderBehavior = steeringModule->AddBehavior < AI::WanderBehavior3D >("Wander");
	wanderBehavior->SetParama(20.0f, 0.0f, 100.0f);
	wanderBehavior->SetActive(false);
	//------------------------------------------------------------------------------------------------------------------

	//---- Arrive ------------------------------------------------------------------------------------------------------
	steeringModule->AddBehavior < AI::ArriveBehavior3D >("Arrive")->SetActive(false);
	steeringModule->GetBehavior < AI::ArriveBehavior3D >("Arrive")->SetWeight(1.0f);
	//------------------------------------------------------------------------------------------------------------------

	//---- Pursuit ------------------------------------------------------------------------------------------------------
	//steeringModule->AddBehavior < AI::PursuitBehavior>("Pursuit")->SetActive(false);
	//------------------------------------------------------------------------------------------------------------------

	//---- Evade -------------------------------------------------------------------------------------------------------
	//steeringModule->AddBehavior<AI::EvadeBehavior>("Evade")->SetActive(false);
	//------------------------------------------------------------------------------------------------------------------

	//---- OffSetPursuit -----------------------------------------------------------------------------------------------
	//steeringModule->AddBehavior<AI::OffsetPursuitBehavior<AI::TriangleTeamFormContext>>("OffsetPursuit")->SetActive(false);
	//------------------------------------------------------------------------------------------------------------------

	//---- ObsAvoid      -----------------------------------------------------------------------------------------------
	auto obsAvioid = steeringModule->AddBehavior < AI::ObstacleAvoidBehavior3D >("ObsAvoid");

	obsAvioid->SetActive(false);
	obsAvioid->SetWeight(10.0f);
	//------------------------------------------------------------------------------------------------------------------

	//---- GroupBehavior_Sepreation ------------------------------------------------------------------------------------
	auto GroupSeparation = steeringModule->AddBehavior < AI::GroupSeparationBehavior3D>("GroupSeparation");
	GroupSeparation->SetActive(false);
	GroupSeparation->SetWeight(1.0f);
	//------------------------------------------------------------------------------------------------------------------

	//---- GroupBehavior_Alignment -------------------------------------------------------------------------------------
	auto GroupAlignment = steeringModule->AddBehavior < AI::GroupAlignmentBehavior3D>("GroupAlignment");
	GroupAlignment->SetActive(false);
	GroupAlignment->SetWeight(1.0f);
	//------------------------------------------------------------------------------------------------------------------

	//---- GroupBehavior_Cohesion --------------------------------------------------------------------------------------
	auto GroupCohesion = steeringModule->AddBehavior < AI::GroupCohesionBehavior3D>("GroupCohesion");
	GroupCohesion->SetActive(false);
	GroupCohesion->SetWeight(1.0f);
	//------------------------------------------------------------------------------------------------------------------

	neighbourRange = 100.0f;
	radius = 10.0f;


	position = NFGE::Math::Vector3(100.0f, 100.0f, 100.0f);
	velocity = NFGE::Math::Vector3(0.0f, 1.0f,0.0f);
}

void ConeDude::Terminate()
{
}

void ConeDude::Update(const BehaviorControl & behaviorControl, float deltaTime)
{
	steeringModule->GetBehavior< AI::WanderBehavior3D >("Wander")->SetWeight(behaviorControl.mWanderWeight);
	steeringModule->GetBehavior< AI::GroupSeparationBehavior3D>("GroupSeparation")->SetWeight(behaviorControl.mSeparationWeight);
	steeringModule->GetBehavior< AI::GroupCohesionBehavior3D>("GroupCohesion")->SetWeight(behaviorControl.mCohesionWeight);
	steeringModule->GetBehavior< AI::GroupAlignmentBehavior3D>("GroupAlignment")->SetWeight(behaviorControl.mAlignmentWeight);
	steeringModule->GetBehavior< AI::ObstacleAvoidBehavior3D>("ObsAvoid")->SetWeight(behaviorControl.mObsAvoidWeight);

	steeringModule->GetBehavior< AI::WanderBehavior3D >("Wander")->SetDebugDraw(mIsDebugDraw);
	steeringModule->GetBehavior< AI::GroupSeparationBehavior3D>("GroupSeparation")->SetDebugDraw(mIsDebugDraw);
	steeringModule->GetBehavior< AI::GroupCohesionBehavior3D>("GroupCohesion")->SetDebugDraw(mIsDebugDraw);
	steeringModule->GetBehavior< AI::GroupAlignmentBehavior3D>("GroupAlignment")->SetDebugDraw(mIsDebugDraw);
	steeringModule->GetBehavior< AI::ObstacleAvoidBehavior3D>("ObsAvoid")->SetDebugDraw(mIsDebugDraw);

	neighbourRange = behaviorControl.mNeighborRadius;

	UpdateNeighbor();
	auto force = steeringModule->Calculate();
	auto acceleration = force / mass;
	velocity += acceleration * deltaTime;
	// TODO add truncate method here
	position += velocity * deltaTime;

	if (NFGE::Math::MagnitudeSqr(velocity) > 1.0f)
		heading = NFGE::Math::Normalize(velocity);

	if ((position.x > worldWidth))
		position.x = static_cast<float>((int)position.x % (int)worldWidth);
	else if ((position.x < 0.0f))
		position.x = worldWidth;
	if ((position.y > worldHeight))
		position.y = static_cast<float>((int)position.y % (int)worldHeight);
	else if ((position.y < 0.0f))
		position.y = worldHeight;
	if ((position.z > worldDepth))
		position.z = static_cast<float>((int)position.z % (int)worldDepth);
	else if ((position.z < 0.0f))
		position.z = worldDepth;
}

void ConeDude::Render()
{
	auto& camera = NFGE::sApp.GetMainCamera();
	auto coneRadius = radius * 0.25f;

	float adj = NFGE::Math::Dot(heading, { 0.0f,1.0f,0.0f });
	float hyp = 1.0f;
	float radAngle = acosf(adj / hyp);
	NFGE::Math::Matrix4 rotMatix;
	//radAngle = adj >= 0.0f ? radAngle : radAngle + HalfPI;

	if (radAngle == 0.0f)
		rotMatix.Identity();
	else
		rotMatix = NFGE::Math::MatrixRotationAxis(NFGE::Math::Cross({ 0.0f,1.0f,0.0f }, heading), radAngle);

	mCone.Render({ position.x, position.y, position.z }, rotMatix, { coneRadius, coneRadius, coneRadius }, camera);
}

void ConeDude::DebugDraw(const NFGE::Math::Vector2 & leftPoint, const NFGE::Math::Vector2 & rightPoint)
{
}
