//====================================================================================================
// Filename:	GraphUtil.h
// Created by:	Mingzhuo Zhang
// Description: It contains all the structure & helper function for Creating & Editing All my 
//				PathingMap: [Baisc Node Structure for Each PathFindMap]
//							[BuildGraph() for each PathFindMap]
//							[ShowGraph() for each PathFindMap]
//							[ShowPath() for each PathFindMap]
//							[ShowCloseList() for each PathFindMap]
//							[PathFinding() for each PathFindMap]
//====================================================================================================


#ifndef HELLOPATHFINDING_GRAPHUTIL_HEADER
#define HELLOPATHFINDING_GRAPHUTIL_HEADER

#include "EngineWrapper.h"
#include "Tile.h"
#include <chrono>

using namespace EngineWrapper;

enum SearchMethod
{
	BreathFirst,
	DepthFirst,
	Djika,
	A_Star
};

struct MapNode : public AI::Node
{
	Vector2 position;
	Rect rect;
	int tileType;
};

struct MapInteractNode : public MapNode
{
	float radius;

	void ShowActive()
	{
		DrawScreenCircle(position, radius, Graphics::Colors::Yellow);
	}
	void ShowActive(const Camera2D& camera )
	{
		DrawScreenCircle(camera.ConvertToScreen(position), radius* camera.GetScale(), Graphics::Colors::Yellow);
	}
};

struct MapSpringNode : public MapInteractNode
{
	std::vector<size_t> springLengths;
	Vector2 supposePosition;

	void Update(const AI::Graph<MapSpringNode>& graph, Vector2 center, float deltaTime)
	{
		Vector2 mainDirection = supposePosition - position;
		float mainForceLength = sqrtf(mainDirection.x * mainDirection.x + mainDirection.y * mainDirection.y);

		Vector2 normalMainDirection = Vector2{ mainDirection.x / mainForceLength, mainDirection.y / mainForceLength };
		position += normalMainDirection * (mainForceLength * deltaTime);

		for (int i = 0; i < springLengths.size(); ++i)
		{
			if (springLengths[i] == 0)
			{
				continue;
			}
			auto& neighbor = graph.GetNode(neighbors[i]);

			Vector2 direction = position - neighbor.position;
			if ((direction.x == 0.0f)&&(direction.y == 0.0f))
				direction = Vector2{ 1.0f,1.0f};

			float currentLength = sqrtf(direction.x * direction.x + direction.y * direction.y);
			float difference = (springLengths[i] - currentLength);

			if (difference < 0.0f)
			{
				difference =  0.0f;
			}

			float percentage = difference / springLengths[i];

			Vector2 normalDirection = Vector2{ direction.x / currentLength, direction.y / currentLength};
			position += normalDirection * (difference * percentage * deltaTime);
		}
	}
};

using NavGraph = AI::Graph<MapNode>;
using NavIneractGraph = AI::Graph<MapInteractNode>;
using NavSpringGraph = AI::Graph<MapSpringNode>;

// Graph biuld function
void BuildGridBaseGraph(NavGraph& theGraph, int col, int row, float xGap, float yGap, Vector2 initPosition);
void BuildGridBaseGraphWithInSize(NavGraph& theGraph, int col, int row, float maxWidth, float maxHeight, Vector2 initPosition);
void BuildRandomGraph(NavGraph& theGraph, int nodeCound,int maxWidth, int maxHeight, int maxNeighbor, Vector2 initPosition);
void BuildRandomGraph(NavIneractGraph& theGraph, int nodeCound, float radius, int maxWidth, int maxHeight, int maxNeighbor,Vector2 initPosition);
void BuildRandomGraph(NavSpringGraph& theGraph, int nodeCound, float radius, int maxWidth, int maxHeight, int maxNeighbor, Vector2 initPosition);

// Graph display function
void ShowGraph(NavGraph& graph,float nodeRadius);
void ShowGraph(NavGraph& graph, const std::vector<Tile>& allTile, float nodeRadius);
void ShowGraph(NavGraph& graph, const std::vector<Tile>& allTile, float nodeRadius, const Camera2D& camera );
void ShowGraph(NavIneractGraph& graph);
void ShowGraph(NavSpringGraph& graph);
void ShowGraph(NavSpringGraph& graph, const Camera2D& camera);

// Graph search util function


// Template Implemention -----------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
inline void ShowPath(std::list<size_t>& path, T& graph, float nodeRadius, Color color);
template<typename T>
inline void ShowPath(std::list<size_t>& path, T& graph, float nodeRadius, Color color, const Camera2D& camera);


template<typename T>
void ShowPath(AI::GraphSearch::Context& context, T& graph, float nodeRadius, Color color);
template<typename T>
void ShowPath(AI::GraphSearch::Context& context, T& graph, float nodeRadius, Color color, const Camera2D& camera);

template<typename T>
void ShowClosedList(AI::GraphSearch::Context& context, T& graph, float nodeRadius, Color color);
template<typename T>
void ShowClosedList(AI::GraphSearch::Context& context, T& graph, float nodeRadius, Color color, const Camera2D& camera);

template<typename T, typename U, typename V, typename W>
void PathFinding(AI::GraphSearch::Context & context, T& graph, int start, int end, SearchMethod method);

// Template function implemention----------------------------------------------------------------------------------------

template<typename T>
inline void ShowPath(std::list<size_t>& path, T & graph, float nodeRadius, Color color)
{
	// Draw end point
	DrawScreenCircle(graph.GetNode(path.back()).position, nodeRadius + 2.0f, color);

	while (!(path.size() <= 1))
	{
		MapNode& nodeA = graph.GetNode(path.back());
		path.pop_back();
		MapNode& nodeB = graph.GetNode(path.back());

		DrawScreenLine(nodeA.position, nodeB.position, color);
		DrawScreenCircle(nodeB.position, nodeRadius, color);
	}

	// Draw start point
	DrawScreenCircle(graph.GetNode(path.front()).position, nodeRadius + 2.0f, color);
}

template<typename T>
inline void ShowPath(std::list<size_t>& path, T & graph, float nodeRadius, Color color, const Camera2D & camera)
{
	// Draw end point
	DrawScreenCircle(camera.ConvertToScreen(graph.GetNode(path.back()).position), (nodeRadius + 2.0f) * camera.GetScale(), color);

	while (!(path.size() <= 1))
	{
		MapNode& nodeA = graph.GetNode(path.back());
		path.pop_back();
		MapNode& nodeB = graph.GetNode(path.back());

		DrawScreenLine(camera.ConvertToScreen(nodeA.position), camera.ConvertToScreen(nodeB.position), color);
		DrawScreenCircle(camera.ConvertToScreen(nodeB.position), nodeRadius * camera.GetScale(), color);
	}

	// Draw start point
	DrawScreenCircle(camera.ConvertToScreen(graph.GetNode(path.front()).position), (nodeRadius + 2.0f) * camera.GetScale(), color);
}

template<typename T>
void ShowPath(AI::GraphSearch::Context & context, T & graph, float nodeRadius, Color color)
{
	if (context.found)
	{
		std::list<size_t> path;
		auto next = context.closedList.back();
		while (next != AI::InvalidNode)
		{
			path.push_front(next);
			next = context.parent[next];
		}

		ShowPath(path, graph, nodeRadius, color);
	}
}

template<typename T>
inline void ShowPath(AI::GraphSearch::Context & context, T & graph, float nodeRadius, Color color, const Camera2D & camera)
{
	if (context.found)
	{
		std::list<size_t> path;
		auto next = context.closedList.back();
		while (next != AI::InvalidNode)
		{
			path.push_front(next);
			next = context.parent[next];
		}

		ShowPath(path, graph, nodeRadius, color,camera);
	}
}

template<typename T>
void ShowClosedList(AI::GraphSearch::Context & context, T & graph, float nodeRadius, Color color)
{
	for (auto& nodeIndex : context.closedList)
	{
		MapNode& nodeB = graph.GetNode(nodeIndex);
		DrawScreenCircle(nodeB.position, nodeRadius, color);

		if (context.parent[nodeIndex] != AI::InvalidNode)
		{
			MapNode& nodeA = graph.GetNode(context.parent[nodeIndex]);
			DrawScreenLine(nodeA.position, nodeB.position, color);
		}
	}
}

template<typename T>
inline void ShowClosedList(AI::GraphSearch::Context & context, T & graph, float nodeRadius, Color color, const Camera2D & camera)
{
	for (auto& nodeIndex : context.closedList)
	{
		MapNode& nodeB = graph.GetNode(nodeIndex);
		DrawScreenCircle(camera.ConvertToScreen(nodeB.position), nodeRadius * camera.GetScale(), color);

		if (context.parent[nodeIndex] != AI::InvalidNode)
		{
			MapNode& nodeA = graph.GetNode(context.parent[nodeIndex]);
			DrawScreenLine(camera.ConvertToScreen(nodeA.position), camera.ConvertToScreen(nodeB.position), color);
		}
	}
}

template<typename T, typename U, typename V, typename W>
void PathFinding(AI::GraphSearch::Context & context, T& graph, int start, int end, SearchMethod method)
{
	context.Init(graph.GetNodeCount());
	context.Reset();
	context.start = start;
	context.end = end;

	switch (method)
	{
	case BreathFirst:
	{
		AI::GraphSearch::BFS<T, U> bfs(graph);
		bfs.Execute(context);
		break;
	}
	case DepthFirst:
	{
		AI::GraphSearch::DFS<T, U> dfs(graph);
		dfs.Execute(context);
		break;
	}
	case Djika:
	{
		AI::GraphSearch::Dijkstra<T, U, V> djika(graph);
		djika.Execute(context);
		break;
	}
	case A_Star:
	{
		AI::GraphSearch::A_Star<T, U, V,W> aStar(graph);
		aStar.Execute(context);
		break;
	}
	default:
		break;
	}
}

#endif // !HELLOPATHFINDING_GRAPHUTIL_HEADER
