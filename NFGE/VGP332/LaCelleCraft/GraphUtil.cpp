#include "GraphUtil.h"

using namespace NFGE::Graphics::Colors;

void BuildGridBaseGraph(NavGraph & graph, int col, int row, float xGap, float yGap, Vector2 initPosition)
{
	graph.Initialize(row * col);
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			MapNode& currentNode = graph.GetNode(i * col + j);
			currentNode.neighbors.clear();
			currentNode.tileType = 0;

			for (int h = i - 1; h < i + 2; h++)
			{
				for (int k = j - 1; k < j + 2; k++)
				{
					if ((h == i) && (k == j))
					{
						continue;
					}
					else if (((h >= 0) && (h <= row - 1)) && ((k >= 0) && (k <= col - 1)))
					{
						currentNode.neighbors.push_back(h * col + k );
					}
				}
			}

			currentNode.position = Vector2{ initPosition.x + j * xGap, initPosition.y + i * yGap };
			currentNode.rect = NFGE::Math::Rect(
				currentNode.position.x - xGap * 0.5f,
				currentNode.position.y - yGap * 0.5f,
				currentNode.position.x + xGap * 0.5f,
				currentNode.position.y + yGap * 0.5f
			);
		}
	}
}

void BuildGridBaseGraphWithInSize(NavGraph& theGraph, int col, int row, float maxWidth, float maxHeight, Vector2 initPosition)
{
	float xGap = maxWidth / ((col == 0) ? 1 : col);
	float yGap = maxHeight / ((row == 0) ? 1 : row);
	BuildGridBaseGraph(theGraph, col, row, xGap, yGap, initPosition);
}

template<typename T>
void BuildRandomGraph_basic(T& theGraph, int nodeCount, int maxWidth, int maxHeight, int maxNeighbor, Vector2 initPosition)
{
	maxNeighbor = maxNeighbor > nodeCount ? nodeCount : maxNeighbor;
	maxNeighbor = maxNeighbor < 0 ? 1 : maxNeighbor;
	for (int i = 0; i < nodeCount; i++)											// Clear neighbor at begin, there for we can achieve two direction neighbor adding
	{
		MapNode& currentNode = theGraph.GetNode(i);
		currentNode.neighbors.clear();
	}

	for (int i = 0; i < nodeCount; i++)
	{
		MapNode& currentNode = theGraph.GetNode(i);
		int totalNeighbor = rand() % maxNeighbor + 1;

		std::vector<bool> isAdded(nodeCount, false);

		for (auto& neighbor : currentNode.neighbors)							// update isAdded vector with current neighbor
		{
			isAdded[neighbor] = true;
			totalNeighbor--;
		}

		for (int j = 0; j < totalNeighbor; j++)
		{
			int addingNeightbor = (rand() % nodeCount - i) + i;
			if ((isAdded[addingNeightbor] == false) && (addingNeightbor != i))	// make sure (not adding the same neighbor) && ( not add itself as neighbor)
			{
				MapNode& neighbor = theGraph.GetNode(addingNeightbor);
				currentNode.neighbors.push_back(addingNeightbor);
				neighbor.neighbors.push_back(i);
				isAdded[addingNeightbor] = true;
			}

		}

		float positionX = static_cast<float>(rand() % maxWidth);
		float positionY = static_cast<float>(rand() % maxHeight);

		currentNode.position = initPosition + Vector2{ positionX, positionY };
	}
}
void BuildRandomGraph(NavGraph& theGraph, int nodeCount, int maxWidth, int maxHeight, int maxNeighbor, Vector2 initPosition)
{
	theGraph.Initialize(nodeCount);
	BuildRandomGraph_basic(theGraph, nodeCount, maxWidth, maxHeight, maxNeighbor, initPosition);
}

void BuildRandomGraph(NavIneractGraph & theGraph, int nodeCount, float radius, int maxWidth, int maxHeight, int maxNeighbor, Vector2 initPosition)
{
	theGraph.Initialize(nodeCount);
	for (int i = 0; i < nodeCount; i++)
	{
		MapInteractNode& theNode = theGraph.GetNode(i);
		theNode.radius = radius;
	}
	BuildRandomGraph_basic(theGraph, nodeCount, maxWidth, maxHeight, maxNeighbor, initPosition);
}

void BuildRandomGraph(NavSpringGraph & theGraph, int nodeCount, float radius, int maxWidth, int maxHeight, int maxNeighbor, Vector2 initPosition)
{
	theGraph.Initialize(nodeCount);
	for (int i = 0; i < nodeCount; i++)
	{
		MapSpringNode& theNode = theGraph.GetNode(i);
		theNode.radius = radius;
		theNode.springLengths.clear();
	}

	BuildRandomGraph_basic(theGraph, nodeCount, maxWidth, maxHeight, maxNeighbor, initPosition);


	for (int i = 0; i < nodeCount; i++)
	{
		MapSpringNode& theNode = theGraph.GetNode(i);
		Vector2 offSet{};
		for (auto& neighbor : theNode.neighbors)
		{
			Vector2 vecDifference = theGraph.GetNode(neighbor).position - theNode.position;
			float length = sqrtf(vecDifference.x * vecDifference.x + vecDifference.y * vecDifference.y);
			theNode.springLengths.push_back(static_cast<size_t>(length));

			Vector2 directionNormal = (vecDifference) / length;

			offSet += directionNormal;
		}
		theNode.supposePosition = theNode.position;
		theNode.position = initPosition + Vector2{ maxWidth * 0.5f,maxHeight* 0.5f } + offSet;
	}
}

template<typename T>
void DrawLineBetweenNeighbor(T& graph, const MapNode& currentNode)
{

	for (size_t j = 0; j < currentNode.neighbors.size(); j++)
	{
		MapNode& neighbourNode = graph.GetNode(currentNode.neighbors[j]);
		DrawScreenLine(currentNode.position, neighbourNode.position, Gray);
	}	
}

template<typename T>
void DrawLineBetweenNeighbor(T& graph, const MapNode& currentNode, const Camera2D& camera)
{

	for (size_t j = 0; j < currentNode.neighbors.size(); j++)
	{
		MapNode& neighbourNode = graph.GetNode(currentNode.neighbors[j]);
		DrawScreenLine(camera.ConvertToScreen(currentNode.position), camera.ConvertToScreen(neighbourNode.position), Gray);
	}

}

void ShowGraph(NavGraph & graph, float nodeRadius)
{
	for (size_t i = 0; i < graph.GetNodeCount(); i++)
	{
		MapNode& currentNode = graph.GetNode(i);
		DrawScreenCircle(currentNode.position, nodeRadius, LightBlue);

		DrawLineBetweenNeighbor(graph, currentNode);
	}
}

void ShowGraph(NavGraph & graph, const std::vector<Tile>& allTile, float nodeRadius)
{
	for (size_t i = 0; i < graph.GetNodeCount(); i++)
	{
		MapNode& currentNode = graph.GetNode(i);
		if (allTile[graph.GetNode(i).tileType].isWall)
		{
			DrawScreenCircle(currentNode.position, nodeRadius, Yellow);
			continue;
		}
		DrawScreenCircle(currentNode.position, nodeRadius, DarkBlue);

		DrawLineBetweenNeighbor(graph, currentNode);
	}
}

void ShowGraph(NavGraph & graph, const std::vector<Tile>& allTile, float nodeRadius, const Camera2D & camera)
{
	for (size_t i = 0; i < graph.GetNodeCount(); i++)
	{
		MapNode& currentNode = graph.GetNode(i);

		Vector2 inScreenPosition = camera.ConvertToScreen(currentNode.position);
		
		if (!camera.IsOutOfView(inScreenPosition, Vector2()))
		{
			if (allTile[graph.GetNode(i).tileType].isWall)
			{
				DrawScreenCircle(inScreenPosition, nodeRadius, Yellow);
				continue;
			}
			DrawScreenCircle(inScreenPosition, nodeRadius, DarkBlue);
			
			DrawLineBetweenNeighbor(graph, currentNode,camera);
		}

	}
}

void ShowGraph(NavIneractGraph & graph)
{
	for (size_t i = 0; i < graph.GetNodeCount(); i++)
	{
		MapNode& currentNode = graph.GetNode(i);
		DrawScreenCircle(currentNode.position, graph.GetNode(i).radius, LightBlue);

		DrawLineBetweenNeighbor(graph, currentNode);
	}
}

void ShowGraph(NavSpringGraph & graph)
{
	for (size_t i = 0; i < graph.GetNodeCount(); i++)
	{
		MapNode& currentNode = graph.GetNode(i);
		DrawScreenCircle(currentNode.position, graph.GetNode(i).radius, LightBlue);

		DrawLineBetweenNeighbor(graph, currentNode);
	}
}

void ShowGraph(NavSpringGraph & graph, const Camera2D & camera)
{
	for (size_t i = 0; i < graph.GetNodeCount(); i++)
	{
		MapNode& currentNode = graph.GetNode(i);
		DrawScreenCircle(camera.ConvertToScreen(currentNode.position), graph.GetNode(i).radius * camera.GetScale(), LightBlue);

		DrawLineBetweenNeighbor(graph, currentNode,camera);
	}
}





