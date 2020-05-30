//====================================================================================================
// Filename:	Tile.h
// Created by:	Mingzhuo Zhang
// Description:	Has a structure <Tile> for textureGridMap, which store the tile infomation such as
//				texture, cost, etc
//====================================================================================================

#ifndef HELLOPATHFINDING_TileMap_HEADER
#define HELLOPATHFINDING_TileMap_HEADER

#include "EngineWrapper.h"

using namespace EngineWrapper;

struct Tile
{
	size_t index;
	std::string textureName;
	EngineWrapper::TextureId textureId;
	float walkThroughCost;
	bool isWall;
	Tile()
		:index(0)
		, textureName("grass.png")
		, textureId (0)
		, walkThroughCost ( 1.0f)
		,isWall(false)
	{
		textureId = LoadTexture(textureName.c_str());
	}

	Tile(const char* fileName)
		:index(0)
		, textureName(fileName)
		, textureId(0)
		, walkThroughCost(1.0f)
		, isWall(false)
	{
		textureId = LoadTexture(textureName.c_str());
	}

};

#endif // !HELLOPATHFINDING_TileMap_HEADER
