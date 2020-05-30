//====================================================================================================
// Filename:	RandomSpringMap.cpp
// Created by:	Mingzhuo Zhang
//====================================================================================================
#include "RandomSpringMap.h"

void RandomSpringMap::Load()
{
	initPosition = Vector2{600.0f,100.0f};
	BuildRandomGraph(randomGraph, totalNode, nodeRadius, static_cast<int>(graphMaxArea.x), static_cast<int>(graphMaxArea.y), maxNeighbor, initPosition);
	isGraphUpdate = true;

	GraphContext.Init(randomGraph.GetNodeCount());
}

void RandomSpringMap::Unload()
{
}

void RandomSpringMap::Render(const Camera2D& camera)
{
	ShowGraph(randomGraph,camera);

	if (isGraphUpdate)
	{
		if (isGraphPathFinded)
		{
			ImGui::Text("Time Use: %f", timeUse);
			ShowClosedList(GraphContext, randomGraph, 4.0f, Graphics::Colors::Red,camera);
			ShowPath(GraphContext, randomGraph, 4.0f, Graphics::Colors::LightGreen, camera);
		}
	}

	for (int i = 0; i < PathEndPoints.size(); i++)
	{
		MapInteractNode& currentNode = randomGraph.GetNode(PathEndPoints[i]);
		DrawScreenCircle(camera.ConvertToScreen(currentNode.position), currentNode.radius * camera.GetScale(), Graphics::Colors::LightGreen);
	}

	if (activeNode != -1)
	{
		randomGraph.GetNode(activeNode).ShowActive(camera);
	}
}
