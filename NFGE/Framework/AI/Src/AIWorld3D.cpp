#include "Precompiled.h"
#include "AIWorld3D.h"

using namespace NFGE::AI;
using namespace NFGE;

namespace
{
	void AddCellAgentToNeighborhood(Agent3Ds& neigbor, const NFGE::Math::Sphere & range, const AIWorld3D::Grid3D& grid, int row, int col, int height)
	{
		if ((row < 0) || (row >= grid.mRow)) return;
		if ((col < 0) || (col >= grid.mCol)) return;
		if ((height < 0) || (height >= grid.mHeight)) return;

		int currentCellIndex = (col + row * grid.mCol) + (height * grid.mCol * grid.mRow);
		auto& newAgents = grid.mCells[currentCellIndex].mAgents;

		std::vector<AI::Agent3D*> addingAgents{};
		for (auto agent : newAgents)
		{
			if (NFGE::Math::Distance(agent->position, range.center) <= range.radius)
				addingAgents.push_back(agent);
		}
		neigbor.insert(neigbor.end(), addingAgents.begin(), addingAgents.end());
	}
}

void AIWorld3D::AddObstacle3D(const NFGE::Math::Sphere& obstacle)
{
	mObstacles.push_back(obstacle);
}

void AIWorld3D::AddWall3D(const NFGE::Math::Plane& wall)
{
	mWalls.push_back(wall);
}

void AI::AIWorld3D::RegisterAgent3D(Agent3D * agent)
{
	mAgents.push_back(agent);
}

void AI::AIWorld3D::UnregisterAgent3D(Agent3D * agent)
{
	auto iter = std::find(mAgents.begin(), mAgents.end(), agent);
	if (iter != mAgents.end())
	{
		std::iter_swap(iter, mAgents.end() - 1);
		mAgents.pop_back();
	}
}

void AI::AIWorld3D::Update(float deltaTime)
{
	ASSERT(mGrid.mHasInit == true, "[AIWorld] has to initialize the grid before update");
	mGrid.ClearGridCells();
	for (auto& agentPtr : mAgents)
	{
		mGrid.FenPeiAgent(agentPtr);
	}
}

void NFGE::AI::AIWorld3D::Update(float neighborRange, float deltaTime)
{
	float newCellSize = neighborRange * 2.0f;
	mGrid.mSize = newCellSize > 10.0f ? newCellSize : 10.0f;
	Update(deltaTime);																				// This is an extra function call, make it inline to improve perfomance
}

void AI::AIWorld3D::GetNeighborhood(Agent3Ds& neighbors, const NFGE::Math::Sphere & range) const
{
	
	neighbors.clear();
	int InCellIndex = mGrid.GetInCellIndex(range.center); 
	int InCellIndex_col = static_cast<int>(range.center.x / mGrid.mSize);
	int InCellIndex_row = static_cast<int>(range.center.z / mGrid.mSize);
	int InCellIndex_hei = static_cast<int>(range.center.y / mGrid.mSize);

	if ((InCellIndex < 0) || (InCellIndex >= static_cast<int>(mGrid.mCells.size())))
		return;


	int horizExpan = 1;
	int depthExpan = 1;
	int vertiExpan = 1;

	NFGE::Math::Vector3 currentCellCenter{ InCellIndex_col * mGrid.mSize + mGrid.mSize * 0.5f, InCellIndex_hei * mGrid.mSize + mGrid.mSize * 0.5f, InCellIndex_row * mGrid.mSize + mGrid.mSize * 0.5f };
	if (range.center.x < currentCellCenter.x) horizExpan = -1;
	if (range.center.y < currentCellCenter.y) vertiExpan = -1;
	if (range.center.z < currentCellCenter.z) depthExpan = -1;


	AddCellAgentToNeighborhood(neighbors, range, mGrid, InCellIndex_row,				InCellIndex_col,				InCellIndex_hei);
	AddCellAgentToNeighborhood(neighbors, range, mGrid, InCellIndex_row + depthExpan,	InCellIndex_col,				InCellIndex_hei);
	AddCellAgentToNeighborhood(neighbors, range, mGrid,	InCellIndex_row,				InCellIndex_col + horizExpan,	InCellIndex_hei);
	AddCellAgentToNeighborhood(neighbors, range, mGrid, InCellIndex_row + depthExpan,	InCellIndex_col + horizExpan,	InCellIndex_hei);
	AddCellAgentToNeighborhood(neighbors, range, mGrid, InCellIndex_row,				InCellIndex_col,				InCellIndex_hei + vertiExpan);
	AddCellAgentToNeighborhood(neighbors, range, mGrid, InCellIndex_row + depthExpan,	InCellIndex_col,				InCellIndex_hei + vertiExpan);
	AddCellAgentToNeighborhood(neighbors, range, mGrid, InCellIndex_row,				InCellIndex_col + horizExpan,	InCellIndex_hei + vertiExpan);
	AddCellAgentToNeighborhood(neighbors, range, mGrid, InCellIndex_row + depthExpan,	InCellIndex_col + horizExpan,	InCellIndex_hei + vertiExpan);
	//for (auto agent : mAgents)
	//{
	//	if (NFGE::Math::Distance(agent->position, range.center) <= range.radius)
	//		neighbors.push_back(agent);
	//}

	
}
