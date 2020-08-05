#include "GameState.h"

#include "TextureGridMap.h"
#include "RandomSpringMap.h"
//-------------------------------------------------------------------------------------------------------------------------------------//|
TextureGridMap myGridMap;
RandomSpringMap myRandomMap;
struct GridMap_CanOpenNode_Djika																									   //|
{																																	   //|
	bool operator()(AI::GraphSearch::Context& context, size_t n)																	   //|
	{																																   //|
		if (context.closed[n])																										   //|
			return false;																											   //|
		if (myGridMap.IsWall(n))																									   //|
			return false;																											   //|
		return true;																												   //|
	}																																   //|
};																																	   //|
struct GridMap_CanOpenNode_DFSnBFS																									   //|
{																																	   //|
	bool operator()(AI::GraphSearch::Context& context, size_t n)																	   //|
	{																																   //|
		if (context.open[n])																										   //|
			return false;																											   //|
		if (myGridMap.IsWall(n))																									   //|--- Global GridMap and its policies
			return false;																											   //|		
		return true;																												   //|		Because the policies need to 
	}																																   //|		know the instance of the 
};																																	   //|		map, So I have to put
struct GridMap_GetGCost																												   //|		them here
{																																	   //|
	float operator()(AI::GraphSearch::Context& context, size_t parentIndex, size_t nodeIndex)										   //|
	{																																   //|
		if (parentIndex == AI::InvalidNode)																							   //|
			return 0.0f;																											   //|
																																	   //|
		size_t fromColumn = parentIndex % myGridMap.col;																			   //|
		size_t fromRow = parentIndex / myGridMap.col;																				   //|
		size_t toColume = nodeIndex % myGridMap.col;																				   //|
		size_t toRow = nodeIndex / myGridMap.col;																					   //|
		float currentGCost = myGridMap.allTiles[myGridMap.gridGraph.GetNode(nodeIndex).tileType].walkThroughCost;
		return context.g[parentIndex] + ((fromColumn != toColume && fromRow != toRow) ? 1.414f*currentGCost : 1.0f*currentGCost);	   //|
																																	   //|
	}																																   //|
};																																	   //|
struct GridMap_GetHCost																												   //|
{																																	   //|
	float operator()(AI::GraphSearch::Context& context, size_t nodeIndex)															   //|
	{																																   //|
																																	   //|
		size_t fromColumn = nodeIndex % myGridMap.col;																				   //|
		size_t fromRow = nodeIndex / myGridMap.col;																					   //|
		size_t toColume = context.end % myGridMap.col;																				   //|
		size_t toRow = context.end / myGridMap.col;																					   //|
																																	   //|
		return  sqrtf(static_cast<float>((toColume - fromColumn) * (toColume - fromColumn) + (toRow - fromRow) *(toRow - fromRow)));   //|
	}																																   //|
};																																	   //|
																																	   //|
//-------------------------------------------------------------------------------------------------------------------------------------//|

//-------------------------------------------------------------------------------------------------------------------------------------//|
struct RandomMap_CanOpenNode_Djika																									   //|
{																																	   //|
	bool operator()(AI::GraphSearch::Context& context, size_t n)																	   //|
	{																																   //|
		if (context.closed[n])																										   //|
			return false;																											   //|
		return true;																												   //|
	}																																   //|
};																																	   //|
struct RandomMap_CanOpenNode_DFSnBFS																								   //|
{																																	   //|
	bool operator()(AI::GraphSearch::Context& context, size_t n)																	   //|
	{																																   //|
		if (context.open[n])																										   //|
			return false;																											   //|
		return true;																												   //|--- Random Map and its policies
	}																																   //|
};																																	   //|		Because the policies need to
struct RandomMap_GetGCost																											   //|		know the instance of the 
{																																	   //|		map, So I have to put
	float operator()(AI::GraphSearch::Context& context, size_t parentIndex, size_t nodeIndex)										   //|		them here
	{																																   //|
		if (parentIndex == AI::InvalidNode)																							   //|
			return 0.0f;																											   //|
																																	   //|
		auto& currentNode = myRandomMap.randomGraph.GetNode(nodeIndex);																   //|
		auto& parentNode = myRandomMap.randomGraph.GetNode(parentIndex);															   //|
																																	   //|
		Vector2 direction = currentNode.position - parentNode.position;																   //|
		float LengthSqure = sqrtf(direction.x * direction.x + direction.y * direction.y);											   //|
																																	   //|
		return context.g[parentIndex] + (LengthSqure);																				   //|
																																	   //|
	}																																   //|
};																																	   //|
struct RandomMap_GetHCost																											   //|
{																																	   //|
	float operator()(AI::GraphSearch::Context& context, size_t nodeIndex)															   //|
	{																																   //|
																																	   //|
		auto& currentNode = myRandomMap.randomGraph.GetNode(nodeIndex);																   //|
		auto& endNode = myRandomMap.randomGraph.GetNode(context.end);																   //|
																																	   //|
		Vector2 direction = currentNode.position - endNode.position;																   //|
		float LengthSqure = sqrtf(direction.x * direction.x + direction.y * direction.y);											   //|
																																	   //|
		return LengthSqure;																											   //|
																																	   //|
	}																																   //|
};																																	   //|
//-------------------------------------------------------------------------------------------------------------------------------------//|

void GameState::Initialize()
{
	myCamera2D = Camera2D(static_cast<float>(NFGE::sApp.GetScreenWidth()), static_cast<float>(NFGE::sApp.GetScreenWidth()));
	myGridMap.Load();
	myRandomMap.Load();
}

void GameState::Terminate()
{
	myGridMap.Unload();
	myRandomMap.Unload();
}

void GameState::Update(float deltaTime)
{
	// GridGraph
	
	
	myRandomMap.Update(deltaTime);

	myCamera2D.Update(deltaTime);
}

void GameState::Render()
{
	myGridMap.Render(myCamera2D);

	myRandomMap.Render(myCamera2D);
}

void GameState::DebugUI()
{
	myGridMap.DebugUI<GridMap_CanOpenNode_Djika, GridMap_CanOpenNode_DFSnBFS, GridMap_GetGCost, GridMap_GetHCost>();
	myRandomMap.DebugUI<RandomMap_CanOpenNode_Djika, RandomMap_CanOpenNode_DFSnBFS, RandomMap_GetGCost, RandomMap_GetHCost>();
}

