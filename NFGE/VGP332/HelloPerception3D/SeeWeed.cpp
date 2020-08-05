#include "SeeWeed.h"

PTSphere SeeWeed::mSphere;
extern bool isDrawDebugLine;
namespace
{
	const float SeeWeedRadius = 10.0f;
	const int MeshRow = 8;
	const int MeshCol = 8;
	const float MeshRadius = 1.0f;
	const wchar_t* MeshShaderFileName = L"../../Assets/Shaders/Standard_Material.fx";

	const int MaxSeeWeedInTree = 20;
}

void BuildSeeWeedTree(AI::Graph<SeeWeed*>& theGraph, int nodeCount, int maxRadius, int maxHeight, int maxNeighbor, Vector3 initPosition);

void SeeWeedTree::Initialize(float seeWeedEcoVal, AI::AIWorld3D& world, float worldWidth, float worldHeight, float worldDepth)
{
	mWorldWidth = worldWidth;
	mWorldHeight = worldHeight;
	mWorldDepth = worldDepth;
	for (int i = 0; i < MaxSeeWeedInTree; i++)
	{
		mAllSeeWeed.push_back(new SeeWeed(world));
		mAllSeeWeed.back()->Initialize(worldWidth, worldHeight, worldDepth);
		world.RegisterEntity3D<SeeWeed>(mAllSeeWeed.back());
	}
	ResetPosition();
	for (int i = 0; i < mSeeWeedGraph.GetNodeCount(); i++)
	{
		mSeeWeedGraph.GetNode(i)->mIsActive = true;
	}
}

void SeeWeedTree::Terminate()
{
	for (int i = 0; i < MaxSeeWeedInTree; i++)
	{
		delete mAllSeeWeed[i];
	}
}

void SeeWeedTree::Reset(const Vector3 & newPos, float seeWeedEcoVal)
{
	position = newPos;
	mTotalSeeWeed = (int)(seeWeedEcoVal * MaxSeeWeedInTree);
	mSeeWeedGraph.Initialize(mTotalSeeWeed);
	for (int i = 0; i < mTotalSeeWeed; i++)
	{
		SeeWeed*& theNode = mSeeWeedGraph.GetNode(i);
		theNode = mAllSeeWeed[i];
		theNode->Load();
	}
	BuildSeeWeedTree(mSeeWeedGraph, mTotalSeeWeed, 200, 100, maxNeighbor, newPos);
}

void SeeWeedTree::Update(float deltaTime)
{

	for (int i = 0; i < mSeeWeedGraph.GetNodeCount(); i++)
	{
		if (!mSeeWeedGraph.GetNode(i)->mIsActive)
			continue;
		mSeeWeedGraph.GetNode(i)->Update(deltaTime);
		mSeeWeedGraph.GetNode(i)->UpdateSprint(mSeeWeedGraph ,deltaTime);
	}

	int currentActiveNode = 0;
	for (int i = 0; i < mSeeWeedGraph.GetNodeCount(); i++)
	{
		//--------------------------------------------------------------------------------------------------//|
		if ((!mSeeWeedGraph.GetNode(i)->mIsActive) && (!mSeeWeedGraph.GetNode(i)->mSprings.empty()))		//|
		{																									//|
			for (auto neigobr : mSeeWeedGraph.GetNode(i)->mSprings)											//|
			{																								//|
				auto& nn = mSeeWeedGraph.GetNode(neigobr.neighbor)->mSprings;								//|--- Remove the none active node and the spring connection from current active node
				Spring a;																					//|
				a.neighbor = i;																				//|
				a.springPower = neigobr.springPower;														//|
				nn.erase(std::remove(nn.begin(), nn.end(), a));												//|
			}																								//|
			mSeeWeedGraph.GetNode(i)->mSprings.clear();														//|
		}																									//|
		//--------------------------------------------------------------------------------------------------//|
		if (mSeeWeedGraph.GetNode(i)->mIsActive)
			++currentActiveNode;
	}

	if (currentActiveNode < 1)
		ResetPosition();
}

void SeeWeedTree::Render()
{
	if (isDrawDebugLine)
		return;
	for (int i = 0; i < mSeeWeedGraph.GetNodeCount(); i++)
	{
		if (!mSeeWeedGraph.GetNode(i)->mIsActive)
			continue;
		mSeeWeedGraph.GetNode(i)->Render();
		mSeeWeedGraph.GetNode(i)->RenderSpringLine(mSeeWeedGraph);
	}
}

void SeeWeedTree::DebugDraw()
{
	for (int i = 0; i < mSeeWeedGraph.GetNodeCount(); i++)
	{
		if (!mSeeWeedGraph.GetNode(i)->mIsActive)
			continue;
		mSeeWeedGraph.GetNode(i)->DebugDraw();
	}
}

void SeeWeedTree::ResetPosition()
{
	float positionX = NFGE::Math::RandomFloat(100.0f, mWorldWidth - 100.0f);
	float positionY = NFGE::Math::RandomFloat(100.0f, mWorldHeight - 100.0f);
	float positionZ = NFGE::Math::RandomFloat(100.0f, mWorldHeight - 100.0f);
	Reset({ positionX, positionY, positionZ }, NFGE::Math::RandomFloat(0.7f, 1.0f));
}

void BuildSeeWeedTree(AI::Graph<SeeWeed*>& theGraph, int nodeCount, int maxRadius, int maxHeight, int maxNeighbor, Vector3 initPosition)
{
	maxNeighbor = maxNeighbor > nodeCount ? nodeCount : maxNeighbor;
	maxNeighbor = maxNeighbor < 0 ? 1 : maxNeighbor;
	for (int i = 0; i < nodeCount; i++)											// Clear neighbor at begin, there for we can achieve two direction neighbor adding
	{
		SeeWeed* currentNode = theGraph.GetNode(i);
		currentNode->mSprings.clear();
	}

	float currentRadius = static_cast<float>(rand() % maxRadius);
	for (int i = 0; i < nodeCount; i++)
	{
		SeeWeed* currentNode = theGraph.GetNode(i);
		int totalNeighbor = 1;
		
		if (i == 0)
		{
			currentNode->position = initPosition;
		}
		else
		{
			int addingNeightbor = 0;
			SeeWeed* spring = theGraph.GetNode(addingNeightbor);
			currentNode->mSprings.push_back({ addingNeightbor, 0.0f });
			spring->mSprings.push_back({ i, 0.0f });
			currentNode->position = initPosition + NFGE::Math::RandomUnitSphere() * currentRadius;
		}

		
		currentNode->mHeight = currentNode->position.y;
	}
	for (int i = 0; i < nodeCount; i++)
	{
		SeeWeed* theNode = theGraph.GetNode(i);
		Vector3 offSet{};
		for (auto& spring : theNode->mSprings)
		{
			Vector3 vecDifference = theGraph.GetNode(spring.neighbor)->position - theNode->position;
			//float heightDifferece = theGraph.GetNode(spring.neighbor)->mHeight - theNode->mHeight;

			Vector3 vecDiff3D = { vecDifference.x, vecDifference.y, vecDifference.z };

			float length = Magnitude(vecDiff3D);
			spring.springPower = (length);

			Vector3 directionNormal = Normalize(vecDiff3D);

			offSet += directionNormal;
		}
		theNode->supposePosition = { theNode->position.x, theNode->position.y, theNode->position.z };
		theNode->mHeight = 1.0f + offSet.x;
		theNode->position = initPosition + Vector3{ offSet.x, offSet.y, offSet.z };
		theNode->mIsGetEatting = false;
		theNode->whoseEating = nullptr;
		theNode->mIsActive = true;
	}
}

SeeWeed::SeeWeed(AI::AIWorld3D & world)
	: PTEntity_Shell(world)
{
	if (!mSphere.mIsInitialize)
	{
		mSphere.Initialize(MeshRow, MeshCol, MeshRadius, MeshShaderFileName, MeshShaderFileName);
		mSphere.mMaterial.ambient = NFGE::Graphics::Colors::ForestGreen;
		mSphere.mMaterial.diffuse = NFGE::Graphics::Colors::DarkSeaGreen;
	}
}

void SeeWeed::Initialize(float width, float height, float depth)
{
	maxSpeed = 100.0f;
	steeringModule = std::make_unique<AI::SteeringModule3D>(*this);
	//---- ObsAvoid      -----------------------------------------------------------------------------------------------
	auto obsAvioid = steeringModule->AddBehavior < AI::ObstacleAvoidBehavior3D >("ObsAvoid");

	obsAvioid->SetActive(true);
	obsAvioid->SetWeight(10.0f);
	//------------------------------------------------------------------------------------------------------------------
	//---- Wander      -----------------------------------------------------------------------------------------------
	auto wander = steeringModule->AddBehavior < AI::WanderBehavior3D >("Wander");
	wander->SetParama(20.0f, 0.0f, 100.0f);
	wander->SetActive(true);
	wander->SetWeight(10.0f);
	//------------------------------------------------------------------------------------------------------------------
	worldWidth = width;
	worldHeight = height;
	worldDepth = depth;
	position = NFGE::Math::Vector3(100.0f, 100.0f, 100.0f);
	velocity = NFGE::Math::Vector3(0.0f, 0.0f, 0.0f);
	radius = SeeWeedRadius;
	mSprings.emplace_back();
}

void SeeWeed::Terminate()
{
}

void SeeWeed::Load()
{
	mIsActive = true;
}

void SeeWeed::OnUpdate(float deltaTime)
{
	UpdatePhysics(deltaTime);
	ClampInToWorld();
}

void SeeWeed::Render()
{
	auto& camera = NFGE::sApp.GetMainCamera();

	auto sphereRadius = radius * 0.5f;
	mSphere.Render({ position.x, position.y, position.z }, heading, { sphereRadius, sphereRadius, sphereRadius }, camera);

	
}

void SeeWeed::DebugDraw()
{
}

void SeeWeed::UpdatePhysics(float deltaTime)
{
	auto force = steeringModule->Calculate();

	if (mIsGetEatting)
	{
		position = whoseEating->position;
		mEatTimmer -= deltaTime;

		if (mEatTimmer < 0.0f)
		{
			mIsActive = false;
			return;
		}
	}

	auto acceleration = force / mass;
	velocity += acceleration * deltaTime;
	// TODO add truncate method here
	position += velocity * deltaTime;

	if (NFGE::Math::MagnitudeSqr(velocity) > 1.0f)
		heading = NFGE::Math::Normalize(velocity);

	//if (mIsLowestNode)
	//{
	//	if (mHeight > 0.0f)
	//	{
	//		velocity_Y += -9.8f * deltaTime;
	//		mHeight += velocity_Y * deltaTime;
	//	}
	//	else
	//	{
	//		//GetEat();
	//		mIsOnGround = true;
	//		velocity_Y = 0.0f;
	//		mHeight = 0.0f;
	//	}
	//}
	
	// TODO:: Do some random movement if its the center node
}

void SeeWeed::UpdateSprint(const AI::Graph<SeeWeed*>& graph, float deltaTime)
{

	Vector3 position3D = Vector3(position.x, position.y, position.z);
	Vector3 mainDirection = supposePosition - position3D;
	float mainForceLength = Magnitude(mainDirection);

	Vector3 normalMainDirection = Normalize(mainDirection);
	position3D += normalMainDirection * (mainForceLength * deltaTime);

	for (int i = 0; i < mSprings.size(); ++i)
	{
		if (mSprings[i].springPower == 0.0f)
		{
			continue;
		}
		auto& neighbor = graph.GetNode(mSprings[i].neighbor);
		auto neighborPos3D = Vector3(neighbor->position.x, neighbor->position.y, neighbor->position.z);

		Vector3 direction = position3D - neighborPos3D;
		if (NFGE::Math::IsZero(direction))
			direction = Vector3{ 1.0f,1.0f,1.0f };

		float currentLength = Magnitude(direction);
		float difference = (mSprings[i].springPower - currentLength);

		if (difference < 0.0f)
		{
			difference = 0.0f;
		}

		float percentage = difference / mSprings[i].springPower;

		Vector3 normalDirection = Normalize(direction);
		position3D += normalDirection * (difference * percentage * deltaTime);

		position = position3D;
		//mHeight = position3D.y;
	}
}

void SeeWeed::RenderSpringLine(const AI::Graph<SeeWeed*>& graph)
{
	for (auto& spring : mSprings)
	{
		auto& currentNeigbor = graph.GetNode(spring.neighbor);
		auto point1 = Vector3{ currentNeigbor->position.x ,currentNeigbor->position.y, currentNeigbor->position.z };
		auto point2 = Vector3{ position.x ,position.y, position.z };
		NFGE::Graphics::SimpleDraw::AddLine(point1, point2, NFGE::Graphics::Colors::DarkGreen);
	}
}

void SeeWeed::GetEat(Agent3D* entityPtr)
{
	mIsGetEatting = true;
	mEatTimmer = mEatTakeTime;
	whoseEating = entityPtr;
	//mIsActive = false;

}

void SeeWeed::ClampInToWorld()
{

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
