#include "SphereDude.h"

using namespace NFGE::Graphics;
using namespace NFGE::Math;
using namespace NFGE;

PTSphere SphereDude::mSphere;
namespace
{
	const int MeshRow = 8;
	const int MeshCol = 8;
	const float MeshRadius = 1.0f;
	const wchar_t* MeshShaderFileName = L"../../Assets/Shaders/Standard_Material.fx";
}

SphereDude::SphereDude(AI::AIWorld & world)
	:AI::Agent(world) 
{
	if (!mSphere.mIsInitialize)
		mSphere.Initialize(MeshRow, MeshCol, MeshRadius, MeshShaderFileName, MeshShaderFileName);
}

void SphereDude::Initialize(float width, float height)
{
	maxSpeed = 300.0f;
	steeringModule = std::make_unique<AI::SteeringModule>(*this);
	//---- Seek --------------------------------------------------------------------------------------------------------
	steeringModule->AddBehavior < AI::SeekBehavior >("Seek")->SetActive(false);
	//------------------------------------------------------------------------------------------------------------------

	//---- Wander ------------------------------------------------------------------------------------------------------
	auto wanderBehavior = steeringModule->AddBehavior < AI::WanderBehavior >("Wander");
	wanderBehavior->SetParama(20.0f, 20.0f, 10.0f);
	wanderBehavior->SetActive(false);
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

	//---- ObsAvoid      -----------------------------------------------------------------------------------------------
	auto obsAvioid = steeringModule->AddBehavior < AI::ObstacleAvoidBehavior >("ObsAvoid");

	obsAvioid->SetActive(false);
	obsAvioid->SetWeight(10.0f);
	//------------------------------------------------------------------------------------------------------------------

	//---- GroupBehavior_Sepreation ------------------------------------------------------------------------------------
	auto GroupSeparation = steeringModule->AddBehavior < AI::GroupSeparationBehavior>("GroupSeparation");
	GroupSeparation->SetActive(false);
	GroupSeparation->SetWeight(1.0f);
	//------------------------------------------------------------------------------------------------------------------

	//---- GroupBehavior_Alignment -------------------------------------------------------------------------------------
	auto GroupAlignment = steeringModule->AddBehavior < AI::GroupAlignmentBehavior>("GroupAlignment");
	GroupAlignment->SetActive(false);
	GroupAlignment->SetWeight(1.0f);
	//------------------------------------------------------------------------------------------------------------------

	//---- GroupBehavior_Cohesion --------------------------------------------------------------------------------------
	auto GroupCohesion = steeringModule->AddBehavior < AI::GroupCohesionBehavior>("GroupCohesion");
	GroupCohesion->SetActive(false);
	GroupCohesion->SetWeight(1.0f);
	//------------------------------------------------------------------------------------------------------------------

	neighbourRange = 100.0f;
	radius = 10.0f;

	worldWidth = width;
	worldHeight = height;
	position = NFGE::Math::Vector2(100.0f, 100.0f);
	velocity = NFGE::Math::Vector2(0.0f, 1.0f);
}

void SphereDude::Terminate()
{
}

void SphereDude::Update(const BehaviorControl & behaviorControl, float deltaTime)
{
	steeringModule->GetBehavior< AI::WanderBehavior >("Wander")->SetWeight(behaviorControl.mWanderWeight);
	steeringModule->GetBehavior< AI::GroupSeparationBehavior>("GroupSeparation")->SetWeight(behaviorControl.mSeparationWeight);
	steeringModule->GetBehavior< AI::GroupCohesionBehavior>("GroupCohesion")->SetWeight(behaviorControl.mCohesionWeight);
	steeringModule->GetBehavior< AI::GroupAlignmentBehavior>("GroupAlignment")->SetWeight(behaviorControl.mAlignmentWeight);
	steeringModule->GetBehavior< AI::ObstacleAvoidBehavior>("ObsAvoid")->SetWeight(behaviorControl.mObsAvoidWeight);
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
}

void SphereDude::Render()
{
	auto& camera = NFGE::sApp.GetMainCamera();
	Render_2D();
	
	auto sphereRadius = radius * 0.5f;
	mSphere.Render({ position.x, radius, position.y }, { 0.0f,0.0f,0.0f }, { sphereRadius, sphereRadius, sphereRadius }, camera);
}

void SphereDude::DebugDraw(const NFGE::Math::Vector2 & leftPoint, const NFGE::Math::Vector2 & rightPoint)
{
	// Draw wander debugline
	auto wanderBehavior = steeringModule->GetBehavior<AI::WanderBehavior>("Wander");
	if (wanderBehavior != nullptr)
	{
		Vector2 tempPoint = wanderBehavior->renderPoint;
		SimpleDraw::AddCircle_FaceY(tempPoint.x, mDebugLineHeight, tempPoint.y, 5.0f, Colors::Red);
		tempPoint = position + heading * wanderBehavior->wanderDistance;
		SimpleDraw::AddCircle_FaceY(tempPoint.x, mDebugLineHeight, tempPoint.y, wanderBehavior->wanderRadius, Colors::Red);
	}

	//Draw obsAvoid debugline
	Vector2 forwardVector = heading * NFGE::Math::Magnitude(velocity);
	Vector2 leftPoint_forward = leftPoint + forwardVector;
	Vector2 rightPoint_forward = rightPoint + forwardVector;

	Vector3 leftPoint_3d(leftPoint.x, mDebugLineHeight, leftPoint.y);
	Vector3 leftPoint_forward_3d(leftPoint_forward.x, mDebugLineHeight, leftPoint_forward.y);
	Vector3 rightPoint_3d(rightPoint.x, mDebugLineHeight, rightPoint.y);
	Vector3 rightPoint_forward_3d(rightPoint_forward.x, mDebugLineHeight, rightPoint_forward.y);

	NFGE::sApp.DrawLine(leftPoint_3d, leftPoint_forward_3d, NFGE::Graphics::Colors::AliceBlue);
	NFGE::sApp.DrawLine(rightPoint_3d, rightPoint_forward_3d, NFGE::Graphics::Colors::AliceBlue);
	NFGE::sApp.DrawLine(rightPoint_forward_3d, leftPoint_forward_3d, NFGE::Graphics::Colors::AliceBlue);
}

void SphereDude::Render_2D()
{
	NFGE::Math::Vector2 forwardPoint = heading * radius * 2.0f;
	forwardPoint = forwardPoint + position;
	NFGE::Math::Vector2 leftPoint{ -heading.y * radius, heading.x * radius };
	leftPoint -= heading * radius * 1.0f;
	leftPoint = leftPoint + position;
	NFGE::Math::Vector2 rightPoint{ heading.y* radius, -heading.x * radius };
	rightPoint -= heading * radius * 1.0f;
	rightPoint = rightPoint + position;

	Vector3  leftPoint_3d = Vector3(leftPoint.x, mDebugLineHeight, leftPoint.y);
	Vector3  rightPoint_3d = Vector3(rightPoint.x, mDebugLineHeight, rightPoint.y);
	Vector3  forwardPoint_3d = Vector3(forwardPoint.x, mDebugLineHeight, forwardPoint.y);

	NFGE::sApp.DrawLine(leftPoint_3d, forwardPoint_3d, mColor);
	NFGE::sApp.DrawLine(rightPoint_3d, forwardPoint_3d, mColor);
	NFGE::sApp.DrawLine(leftPoint_3d, rightPoint_3d, mColor);

	if (mIsDebugDraw)
		DebugDraw(leftPoint, rightPoint);
}
