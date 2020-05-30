//====================================================================================================
// Filename:	RandomSpringMap.h
// Created by:	Mingzhuo Zhang
// Description:	Has a structure <RandomSpringMap> which contain a graph of Clickable node with spring 
//				connecting each other.
//				It can do :	[Regenerate map with Imgui]
//							[Editable in run time with ImGui]
//							[Pathfinding in run time with ImGui]
//							[Moving object with the spring force to its destination in each frame]
//====================================================================================================

#ifndef HELLOPERCEPTION_RANDOMSPRINGMAP_HEADER
#define HELLOPERCEPTION_RANDOMSPRINGMAP_HEADER

#include "PathFindingMapShell.h"

struct RandomSpringMap : PathFindingMapShell
{
	AI::Graph<MapSpringNode> randomGraph;
	float nodeRadius = 10.0f;
	int totalNode = 11;
	int maxNeighbor = 7;
	std::array<int, 2> PathEndPoints;
	int clickCount{ 0 };
	int activeNode = -1;

	void Load() override;
	void Unload() override;

	template<typename CanOpenPolicy_Djika, typename CanOpenPolicy_DFSnDFS, typename GetGCostPolicy, typename GetHCostPolicy>
	void Update(float deltaTime);

	void Render(const Camera2D& camera) override;
};

#endif // !HELLOPATHFINDING_RANDOMSPRINGMAP_HEADER

// Template implemention ------------------------------------------------------------------------------------------------------
template<typename CanOpenPolicy_Djika, typename CanOpenPolicy_DFSnDFS, typename GetGCostPolicy, typename GetHCostPolicy>
inline void RandomSpringMap::Update(float deltaTime)
{
	// Random Graph
	ImGui::Text("\nRandom Graph");
	if (ImGui::SliderInt("Total Node", &totalNode, 1, 50))
		isGraphUpdate = false;
	if (ImGui::SliderInt("Max of neighbors", &maxNeighbor, 1, totalNode))
		isGraphUpdate = false;
	if (ImGui::Button("Rebuild Random Graph"))
	{
		BuildRandomGraph(randomGraph, totalNode, nodeRadius, static_cast<int>(graphMaxArea.x), static_cast<int>(graphMaxArea.y), maxNeighbor, initPosition);
		isGraphUpdate = true;

		GraphContext.Reset();
		PathEndPoints[0] = 0;
		PathEndPoints[1] = 0;
		isGraphPathFinded = false;
	}

	if (isGraphUpdate)
	{

		bool mouseNotInAll = true;
		for (int i = 0; i < randomGraph.GetNodeCount(); i++)
		{
			Vector2 mousePosition{ static_cast<float>(GetMouseScreenX()), static_cast<float>(GetMouseScreenY()) };
			MapInteractNode& currentNode = randomGraph.GetNode(i);
			if (EngineWrapper::PointInCircle(mousePosition, Circle(currentNode.position, currentNode.radius)))
			{
				mouseNotInAll = false;
				activeNode = i;
				if (IsMousePressed(NFGE::Input::MouseButton::LBUTTON))
				{
					PathEndPoints[clickCount % 2] = i;
					clickCount++;
					isGraphPathFinded = false;
				}
			}
		}
		if (mouseNotInAll)
			activeNode = -1;

		if (ImGui::Button("Random Graph BFS"))
		{
			auto lastTime = std::chrono::high_resolution_clock::now();
			PathFinding<AI::Graph<MapSpringNode>, CanOpenPolicy_DFSnDFS, GetGCostPolicy, GetHCostPolicy>(GraphContext, randomGraph, PathEndPoints[0], PathEndPoints[1], SearchMethod::BreathFirst);
			auto currentTime = std::chrono::high_resolution_clock::now();
			timeUse = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count() / 1000000.0f;
			isGraphPathFinded = true;
		}
		if (ImGui::Button("Random Graph DFS"))
		{
			auto lastTime = std::chrono::high_resolution_clock::now();
			PathFinding<AI::Graph<MapSpringNode>, CanOpenPolicy_DFSnDFS, GetGCostPolicy, GetHCostPolicy>(GraphContext, randomGraph, PathEndPoints[0], PathEndPoints[1], SearchMethod::DepthFirst);
			auto currentTime = std::chrono::high_resolution_clock::now();
			timeUse = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count() / 1000000.0f;
			isGraphPathFinded = true;
		}
		if (ImGui::Button("Random Graph Djika"))
		{
			auto lastTime = std::chrono::high_resolution_clock::now();
			PathFinding<AI::Graph<MapSpringNode>, CanOpenPolicy_Djika, GetGCostPolicy, GetHCostPolicy>(GraphContext, randomGraph, PathEndPoints[0], PathEndPoints[1], SearchMethod::Djika);
			auto currentTime = std::chrono::high_resolution_clock::now();
			timeUse = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count() / 1000000.0f;
			isGraphPathFinded = true;
		}
		if (ImGui::Button("Random Graph A_Star"))
		{
			auto lastTime = std::chrono::high_resolution_clock::now();
			PathFinding<AI::Graph<MapSpringNode>, CanOpenPolicy_Djika, GetGCostPolicy, GetHCostPolicy>(GraphContext, randomGraph, PathEndPoints[0], PathEndPoints[1], SearchMethod::A_Star);
			auto currentTime = std::chrono::high_resolution_clock::now();
			timeUse = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count() / 1000000.0f;
			isGraphPathFinded = true;
		}
	}

	randomGraph.GetNode(0).position = initPosition + Vector2{ graphMaxArea.x * 0.5f , graphMaxArea.y * 0.5f };
	for (size_t i = 0; i < randomGraph.GetNodeCount(); i++)
	{
		randomGraph.GetNode(i).Update(randomGraph, randomGraph.GetNode(0).position, deltaTime);
	}
}
