//====================================================================================================
// Filename:	TextureGridMap.h
// Created by:	Mingzhuo Zhang
// Description:	Has a structure <TextureGirdMap> which hook up the grid tilemap with AI::Graph
//				It can do :	[Load & save to a file]
//							[Editable in run time with ImGui]
//							[Pathfinding in run time with ImGui]
//====================================================================================================

#ifndef HELLOPERCEPTION_TEXTUREGRIDMAP_HEADER
#define HELLOPERCEPTION_TEXTUREGRIDMAP_HEADER

#include "PathFindingMapShell.h"

struct TextureGridMap : PathFindingMapShell
{
	AI::Graph<MapNode> gridGraph;
	int col = 10;
	int row = 10;
	int startX = 0, startY = 0;
	int endX = 0, endY = 0;
	std::vector<Tile> allTiles;
	int currentPaintTile = 0;
	Vector2 textureSize;
	
	float xScale = 1.0f;
	float yScale = 1.0f;

	void Save(std::string fileName);
	void Open(std::string fileName);

	int activeNode = -1;

	void Load() override;
	void Unload() override;
	
	void Update(float deltaTime);

	
	void Render(const Camera2D& camera) override;
	
	bool IsWall(size_t n);

	int ConvertToMapIndex(const Vector2& position) const;
	Vector2 GetRowAndCol(int tileIndex) const;
	bool IsValidIndex(int index) const;
};


#endif // !HELLOPATHFINDING_TEXTUREGRIDMAP_HEADER


