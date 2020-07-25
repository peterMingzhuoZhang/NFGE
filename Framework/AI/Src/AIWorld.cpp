#include "Precompiled.h"
#include "AIWorld.h"

using namespace NFGE::AI;
using namespace NFGE;

namespace
{
	void AddCellAgentToNeighborhood(Agents& neigbor, const NFGE::Math::Circle & range, const AIWorld::Grid& grid, int row, int col)
	{
		if ((row < 0) || (row >= grid.mRow)) return;
		if ((col < 0) || (col>= grid.mCol)) return;
		
		int currentCellIndex = col + row * grid.mCol;
		auto& newAgents = grid.mCells[currentCellIndex].mAgents;

		std::vector<AI::Agent*> addingAgents{};
		for (auto agent : newAgents)
		{
			if (NFGE::Math::Distance(agent->position, range.center) <= range.radius)
				addingAgents.push_back(agent);
		}
		neigbor.insert(neigbor.end(), addingAgents.begin(), addingAgents.end());
	}
}

void AIWorld::AddObstacles(const NFGE::Math::Circle& obstacle)
{
	mObstacles.push_back(obstacle);
}

void AIWorld::AddWall(const NFGE::Math::LineSegment& wall)
{
	mWalls.push_back(wall);
}

void AI::AIWorld::RegisterAgent(Agent * agent)
{
	mAgents.push_back(agent);
}

void AI::AIWorld::UnregisterAgent(Agent * agent)
{
	auto iter = std::find(mAgents.begin(), mAgents.end(), agent);
	if (iter != mAgents.end())
	{
		std::iter_swap(iter, mAgents.end() - 1);
		mAgents.pop_back();
	}
}

void AI::AIWorld::Update()
{
	ASSERT(mGrid.mHasInit == true, "[AIWorld] has to initialize the grid before update");
	mGrid.ClearGridCells();
	for (auto& agentPtr : mAgents)
	{
		mGrid.FenPeiAgent(agentPtr);
	}
	
}

void NFGE::AI::AIWorld::Update(float neiborRange)
{
	float newCellSize = neiborRange * 2.0f;
	mGrid.mSize = newCellSize > 10.0f ? newCellSize : 10.0f;
	Update();																				// This is an extra function call, make it inline to improve perfomance
}

void NFGE::AI::AIWorld::DebugUI()
{
	//------------------------------------------------------------------------------------
	//-|||||||||||-- Debug Function ------------------------------------------------------
	//-|||||||||||------------------------------------------------------------------------
	//-vvvvvvvvvvv------------------------------------------------------------------------
	mGrid.DebugDraw();
	//-^^^^^^^^^^^------------------------------------------------------------------------
	//-|||||||||||------------------------------------------------------------------------
	//-|||||||||||------------------------------------------------------------------------
	//------------------------------------------------------------------------------------
}

void AI::AIWorld::GetNeighborhood(Agents& neigbor, const NFGE::Math::Circle & range) const
{
	neigbor.clear();
	//Agents neighborhood;
	// TODO:: get the neighborhood yourself Peter!!
	int InCellIndex = mGrid.GetInCellIndex(range.center);
	int InCellIndex_col = InCellIndex % mGrid.mCol;
	int InCellIndex_row = InCellIndex / mGrid.mCol;

	if ((InCellIndex < 0) || (InCellIndex >= static_cast<int>(mGrid.mCells.size())))
		return;

	int vertiExpan = 1;
	int horizExpan = 1;

	NFGE::Math::Vector2 currentCellCenter{ InCellIndex_col * mGrid.mSize + mGrid.mSize * 0.5f, InCellIndex_row * mGrid.mSize + mGrid.mSize * 0.5f };
	if (range.center.x < currentCellCenter.x) horizExpan = -1;
	if (range.center.y < currentCellCenter.y) vertiExpan = -1;

	AddCellAgentToNeighborhood(neigbor, range, mGrid, InCellIndex_row, InCellIndex_col);
	AddCellAgentToNeighborhood(neigbor, range, mGrid, InCellIndex_row + vertiExpan, InCellIndex_col);
	AddCellAgentToNeighborhood(neigbor, range, mGrid, InCellIndex_row, InCellIndex_col + horizExpan);
	AddCellAgentToNeighborhood(neigbor, range, mGrid, InCellIndex_row + vertiExpan, InCellIndex_col + horizExpan);

	/*for (auto& agent : mAgents)
	{
		if (NFGE::Math::Distance(agent->position, range.center) <= range.radius)
			neigbor.push_back(agent);
	}*/

}
