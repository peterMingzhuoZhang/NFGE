//====================================================================================================
// Filename:	TextureGridMap.cpp
// Created by:	Mingzhuo Zhang
//====================================================================================================

#include "TextureGridMap.h"
#include <fstream>
#include <string>
#include <sstream>

const char* grassTextureName = "grass.png";
const char* stoneTextureName = "flower.png";

bool TextureGridMap::IsWall(size_t n)
{
	return allTiles[gridGraph.GetNode(n).tileType].isWall;
}

void TextureGridMap::Save(std::string fileName)
{
	std::ofstream myfile;
	myfile.open(fileName.c_str());

	//Start writing in
	myfile << "TileSize_X: " << textureSize.x << ".0f\n";
	myfile << "TileSize_Y: " << textureSize.y << ".0f\n";
	myfile << "Rows: " << row << ".0f\n";
	myfile << "Colunms: " << col << ".0f\n";

	myfile << "TileCount: " << allTiles.size() << "\n";
	for (int i = 0; i < allTiles.size(); ++i)
	{
		myfile << allTiles[i].textureName << " " << allTiles[i].walkThroughCost << " " << allTiles[i].isWall << "\n";
	}

	for (int i = 0; i < (row * col); i++)
	{
		myfile << gridGraph.GetNode(i).tileType;
		if ((((i + 1) % col) == 0) && (i != 0))
		{
			myfile << "\n";
		}
	}

	
	myfile.close();
}

void TextureGridMap::Open(std::string fileName)
{
	allTiles.clear();

	std::string line;
	std::stringstream tokenizer;
	std::ifstream theFile(fileName.c_str());

	ASSERT(theFile.is_open(), "Fail to load file %s", fileName.c_str());

	std::string temp;

	getline(theFile, line);
	tokenizer = std::stringstream(line);
	tokenizer >> temp;
	tokenizer >> textureSize.x;

	getline(theFile, line);
	tokenizer = std::stringstream(line);
	tokenizer >> temp;
	tokenizer >> textureSize.y;

	getline(theFile, line);
	tokenizer = std::stringstream(line);
	tokenizer >> temp;
	tokenizer >> row;

	getline(theFile, line);
	tokenizer = std::stringstream(line);
	tokenizer >> temp;
	tokenizer >> col;

	int tileKindCount = 0;
	getline(theFile, line);
	tokenizer = std::stringstream(line);
	tokenizer >> temp;
	tokenizer >> tileKindCount;

	allTiles.clear();
	allTiles.reserve(tileKindCount);
	for (int i = 0; i < tileKindCount; ++i)
	{
		allTiles.emplace_back();
		getline(theFile, line);
		tokenizer = std::stringstream(line);
		tokenizer >> allTiles[i].textureName;
		tokenizer >> allTiles[i].walkThroughCost;
		tokenizer >> allTiles[i].isWall;
		allTiles[i].index = i;
		allTiles[i].textureId = NFGE::sApp.LoadTexture(allTiles[i].textureName.c_str());
	}

	//-----------------------------------------//|
	char theChar;							   //|
	Load();									   //|
	for (int i = 0; i < gridGraph.GetNodeCount(); ++i)	   //|
	{										   //|
		theFile.get(theChar);				   //|
		if (theChar == '\n')				   //|--- Only work for less than 10 tile kinds
		{									   //|
			theFile.get(theChar);			   //|
		}									   //|
		gridGraph.GetNode(i).tileType = (int)(theChar - '0');		   //|
	}										   //|
	//-----------------------------------------//|

	
	theFile.close();
}

void TextureGridMap::Load()
{
	BuildGridBaseGraphWithInSize(gridGraph, col, row, graphMaxArea.x, graphMaxArea.y, initPosition);
	
	isGraphUpdate = true;

	GraphContext.Init(gridGraph.GetNodeCount());
	if (allTiles.size() == 0)
	{
		allTiles.emplace_back("grass.png");
		allTiles.emplace_back("stone.png");
		allTiles.emplace_back("grass_2.png");
		allTiles.emplace_back("flower.png");
	}

	textureSize.x = GetSpriteWidth(allTiles[0].textureId);
	textureSize.y = GetSpriteHeight(allTiles[0].textureId);
	
	startX = 0;
	startY = 0;
	endX = 0;
	endY = 0;
}

void TextureGridMap::Unload()
{
}


void TextureGridMap::Render(const Camera2D& camera)
{
	float simpleDrawScale = (xScale < yScale) ? xScale : yScale;
	if (isGraphUpdate)
	{
		for (int i = 0; i < gridGraph.GetNodeCount(); i++)
		{
			auto& currentNode = gridGraph.GetNode(i);

			Vector2 inScreenPosition = camera.ConvertToScreen(currentNode.position);
			if (!camera.IsOutOfView(inScreenPosition, textureSize))
			{
				DrawSprite(allTiles[gridGraph.GetNode(i).tileType].textureId, inScreenPosition, 0.0f, 1.0f, 0.5f, 0.5f, xScale * camera.GetScale(), yScale* camera.GetScale());
			}
		}
		if (activeNode != -1)
		{
			NFGE::sApp.DrawScreenRect(camera.ConvertToScreen( gridGraph.GetNode(activeNode).rect), Graphics::Colors::Cyan);
		}
	}

	ShowGraph(gridGraph, allTiles, 5.0f *  simpleDrawScale * camera.GetScale(),camera);


	if (isGraphPathFinded)
	{
		ImGui::Text("Time Use: %f", timeUse);
		ShowClosedList(GraphContext, gridGraph, 6.0f*  simpleDrawScale, Graphics::Colors::Red,camera);
		ShowPath(GraphContext, gridGraph, 6.0f*  simpleDrawScale, Graphics::Colors::LightGreen, camera);
	}

	int start = startX + (startY * col);
	int end = endX + (endY * col);
	DrawScreenCircle(camera.ConvertToScreen(gridGraph.GetNode(start).position), 10.0f*  simpleDrawScale * camera.GetScale(), Graphics::Colors::LightGreen);
	DrawScreenCircle(camera.ConvertToScreen(gridGraph.GetNode(end).position), 10.0f*  simpleDrawScale * camera.GetScale(), Graphics::Colors::LightGreen);
}
