//====================================================================================================
// Filename:	PathFindingMapShell.h
// Created by:	Mingzhuo Zhang
// Description:	It contain a structure <PathFindingMapShell> which is the parent of all PathFindMap
//====================================================================================================
#ifndef HELLOPATHFINDING_PATHFINDINGMAPSHELL_HEADER
#define HELLOPATHFINDING_PATHFINDINGMAPSHELL_HEADER

#include "GraphUtil.h"

struct PathFindingMapShell
{
	bool isGraphUpdate = false;
	Vector2 graphMaxArea = { 500.0f,500.0f };
	Vector2 initPosition{ 100.0f, 100.0f };
	AI::GraphSearch::Context GraphContext;
	bool isGraphPathFinded = false;
	float timeUse = 0.0f;

	virtual void Load() = 0;
	virtual void Unload() = 0;

	virtual void Render(const Camera2D& camera) = 0;
};

#endif // !HELLOPATHFINDING_PATHFINDINGMAPSHELL_HEADER
