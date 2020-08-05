#include "TextureGridMap.h"

using namespace NFGE::Input;

const char* grassTextureName = "grass.png";
const char* stoneTextureName = "flower.png";

//-------------------------------------------------------------------------------------------------------------------------------------//|
TextureGridMap myGridMap;																											   //|
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
		if (myGridMap.IsWall(n))																									   //|--- Grid map and its policies
			return false;																											   //|
		return true;																												   //|
	}																																   //|
};																																	   //|
struct GridMap_GetGCost																												   //|
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

bool TextureGridMap::IsWall(size_t n)
{
	return allTiles[gridGraph.GetNode(n).tileType].isWall;
}

int TextureGridMap::ConvertToMapIndex(const Vector2& position) const
{
	Vector2 relativePosition = position - (initPosition - textureSize);
	int theCol = static_cast<int>(relativePosition.x / (xScale * textureSize.x));
	int theRow = static_cast<int>(relativePosition.y / (yScale * textureSize.y));

	return theCol + (theRow * col);
}

Vector2 TextureGridMap::GetRowAndCol(int tileIndex) const
{
	int theRow = tileIndex / col;
	int theCol = tileIndex % col;
	return Vector2((float)theCol, (float)theRow);
}

void TextureGridMap::FindPath(int start, int end)
{
	PathFinding<AI::Graph<MapNode>, GridMap_CanOpenNode_Djika, GridMap_GetGCost, GridMap_GetHCost>(GraphContext, gridGraph, start, end, SearchMethod::A_Star);
}

void TextureGridMap::Serialize_txt(std::ofstream & myfile)
{
	//Start writing in
	myfile << "Map: " << background.textureName << "\n";
	myfile << "InitPosition_X: " << initPosition.x << "\n";
	myfile << "InitPosition_Y: " << initPosition.y << "\n";
	myfile << "MaxArea_X: " << graphMaxArea.x << "\n";
	myfile << "MaxArea_Y: " << graphMaxArea.y << "\n";
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
}

void TextureGridMap::Deserialize_txt(std::ifstream & theFile)
{
	std::string line;
	std::stringstream tokenizer;

	std::string temp;

	getline(theFile, line);
	tokenizer = std::stringstream(line);
	tokenizer >> temp;
	tokenizer >> backGoundImageName;

	getline(theFile, line);
	tokenizer = std::stringstream(line);
	tokenizer >> temp;
	tokenizer >> initPosition.x;

	getline(theFile, line);
	tokenizer = std::stringstream(line);
	tokenizer >> temp;
	tokenizer >> initPosition.y;

	getline(theFile, line);
	tokenizer = std::stringstream(line);
	tokenizer >> temp;
	tokenizer >> graphMaxArea.x;

	getline(theFile, line);
	tokenizer = std::stringstream(line);
	tokenizer >> temp;
	tokenizer >> graphMaxArea.y;

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
}

void TextureGridMap::Save(std::string fileName)
{
	std::ofstream myfile;
	myfile.open(fileName.c_str());

	Serialize_txt(myfile);

	
	myfile.close();
}

void TextureGridMap::Open(std::string fileName)
{
	allTiles.clear();

	std::ifstream theFile(fileName.c_str());

	ASSERT(theFile.is_open(), "Fail to load file %s", fileName.c_str());

	Deserialize_txt(theFile);
	
	theFile.close();
}

void TextureGridMap::Load()
{
	BuildGridBaseGraphWithInSize(gridGraph, col, row, graphMaxArea.x, graphMaxArea.y, initPosition);
	

	GraphContext.Init(gridGraph.GetNodeCount());
	if (allTiles.size() == 0)
	{
		allTiles.emplace_back("LaCelleCraft/Map/editorTile_1.png");
		allTiles.emplace_back("LaCelleCraft/Map/editorTile_2.png");
	}
	textureSize.x = GetSpriteWidth(allTiles[0].textureId);
	textureSize.y = GetSpriteHeight(allTiles[0].textureId);
	
	float xGap = graphMaxArea.x / static_cast<float>(col);
	float yGap = graphMaxArea.y / static_cast<float>(row);
	xScale = xGap / textureSize.x;
	yScale = yGap / textureSize.y;
	
	background.Load(backGoundImageName, graphMaxArea);

	//-----------------------------------//|
	isGraphUpdate = true;				 //|
	startX = 0;							 //|
	startY = 0;							 //|--- For Debug purpose
	endX = 0;							 //|
	endY = 0;							 //|
	//-----------------------------------//|
}

void TextureGridMap::Load(const Vector2 & initPos, const Vector2 & maxSize, std::string backgroundName)
{
	graphMaxArea = maxSize;
	initPosition = initPos;
	backGoundImageName = backgroundName;
	Load();
}

void TextureGridMap::Unload()
{
}
void TextureGridMap::Update(float deltaTime)
{
	background.Update(deltaTime);
}

void TextureGridMap::DebugUIUpdate(float deltaTime, const Camera2D & camera)
{
	//pImGui::Text("Editor: Adam Xu");

	ImGui::Text("Grid Graph");

	ImGui::BeginChild("TileMap Editor", { 300.0f, 120.0f + static_cast<float>(allTiles.size()) * (textureSize.y + 5.0f) }, true);
	if (ImGui::SliderInt("Columns", &col, 10, 100))
		isGraphUpdate = false;
	if (ImGui::SliderInt("Rows", &row, 10, 100))
		isGraphUpdate = false;

	for (int i = 0; i < allTiles.size(); i++)
	{
		if (ImGui::ImageButton(GetSprite(allTiles[i].textureId), { 32.0f ,32.0f }))
		{
			currentPaintTile = i;
		}

		char i_char[10];
		_itoa_s(i, i_char, 10);
		std::string i_str(i_char);
		ImGui::SameLine();
		ImGui::PushItemWidth(50.0f);
		ImGui::SliderFloat(std::string("Cost" + i_str).c_str(), &(allTiles[i].walkThroughCost), 0.0f, 10.0f);
		ImGui::SameLine();
		ImGui::Checkbox(std::string("Is Wall" + i_str).c_str(), &(allTiles[i].isWall));
	}

	if (ImGui::Button("Add Tile"))
	{
		char filePath[MAX_PATH] = "";
		if (OpenFileDialog(filePath, "Pick your image", "Image Files\0*.png\0"))
		{
			std::string wholePath(filePath);
			size_t index = wholePath.find_last_of("\\");

			allTiles.emplace_back(wholePath.substr(index + 1).c_str());
			allTiles.back().index = allTiles.size() - 1;
		}

	}
	ImGui::EndChild();
	if (ImGui::Button("Open"))
	{
		char filePath[MAX_PATH] = "";
		if (OpenFileDialog(filePath, "Pick your txt", "txt Files\0*.txt\0"))
		{
			std::string wholePath(filePath);
			Open(wholePath);
			isGraphPathFinded = false;
		}

	}
	if (ImGui::Button("Save"))
	{
		char filePath[MAX_PATH] = "";
		if (SaveFileDialog(filePath, "save your image", "txt Files\0*.txt\0"))
		{
			std::string wholePath(filePath);
			Save(wholePath);
		}

	}

	if (ImGui::Button("Rebuild Grid Graph"))
	{
		BuildGridBaseGraphWithInSize(gridGraph, col, row, graphMaxArea.x, graphMaxArea.y, initPosition);
		startX = 0;
		startY = 0;
		endX = 0;
		endY = 0;
		isGraphUpdate = true;

		GraphContext.Reset();
		isGraphPathFinded = false;
	}

	if (isGraphUpdate)
	{
		int start = startX + (startY * col);
		int end = endX + (endY * col);

		float xGap = graphMaxArea.x / static_cast<float>(col);
		float yGap = graphMaxArea.y / static_cast<float>(row);

		xScale = xGap / textureSize.x;
		yScale = yGap / textureSize.y;

		ImGui::Text("Start Point: ");
		ImGui::SliderInt("Start_X", &startX, 0, col - 1);
		ImGui::SliderInt("Start_Y", &startY, 0, row - 1);
		ImGui::Text("End Point: ");
		ImGui::SliderInt("End_X", &endX, 0, col - 1);
		ImGui::SliderInt("End_Y", &endY, 0, row - 1);

		// do tile update here
		bool mouseNotInAll = true;
		for (int i = 0; i < gridGraph.GetNodeCount(); i++)
		{
			auto& currentNode = gridGraph.GetNode(i);

			Vector2 mousePos{ static_cast<float>(GetMouseScreenX()), static_cast<float>(GetMouseScreenY()) };
			if (EngineWrapper::PointInRect(camera.ConvertToWorld(mousePos), currentNode.rect))
			{
				activeNode = i;
				mouseNotInAll = false;

				if (IsMouseDown(MouseButton::LBUTTON))
				{
					gridGraph.GetNode(i).tileType = currentPaintTile;
					isGraphPathFinded = false;
				}
				if (IsMouseDown(MouseButton::RBUTTON))
				{
					gridGraph.GetNode(i).tileType = 0;
					isGraphPathFinded = false;
				}
			}
		}

		if (mouseNotInAll)
			activeNode = -1;

		if (ImGui::Button("Grid Graph BFS"))
		{
			auto lastTime = std::chrono::high_resolution_clock::now();
			PathFinding<AI::Graph<MapNode>, GridMap_CanOpenNode_DFSnBFS, GridMap_GetGCost, GridMap_GetHCost>(GraphContext, gridGraph, start, end, SearchMethod::BreathFirst);
			auto currentTime = std::chrono::high_resolution_clock::now();
			timeUse = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count() / 1000000.0f;
			isGraphPathFinded = true;
		}
		if (ImGui::Button("Grid Graph DFS"))
		{
			auto lastTime = std::chrono::high_resolution_clock::now();
			PathFinding<AI::Graph<MapNode>, GridMap_CanOpenNode_DFSnBFS, GridMap_GetGCost, GridMap_GetHCost>(GraphContext, gridGraph, start, end, SearchMethod::DepthFirst);
			auto currentTime = std::chrono::high_resolution_clock::now();
			timeUse = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count() / 1000000.0f;
			isGraphPathFinded = true;
		}
		if (ImGui::Button("Grid Graph Djika"))
		{
			auto lastTime = std::chrono::high_resolution_clock::now();
			PathFinding<AI::Graph<MapNode>, GridMap_CanOpenNode_Djika, GridMap_GetGCost, GridMap_GetHCost>(GraphContext, gridGraph, start, end, SearchMethod::Djika);
			auto currentTime = std::chrono::high_resolution_clock::now();
			timeUse = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count() / 1000000.0f;
			isGraphPathFinded = true;
		}
		if (ImGui::Button("Grid Graph A_Star"))
		{
			auto lastTime = std::chrono::high_resolution_clock::now();
			PathFinding<AI::Graph<MapNode>, GridMap_CanOpenNode_Djika, GridMap_GetGCost, GridMap_GetHCost>(GraphContext, gridGraph, start, end, SearchMethod::A_Star);
			auto currentTime = std::chrono::high_resolution_clock::now();
			timeUse = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count() / 1000000.0f;
			isGraphPathFinded = true;
		}
	}
	else
	{
		startX = 0; startY = 0;
		endX = 0; endY = 0;
	}

	float simpleDrawScale = (xScale < yScale) ? xScale : yScale;
	ShowGraph(gridGraph, allTiles, 5.0f *  simpleDrawScale * camera.GetScale(), camera);


	if (isGraphPathFinded)
	{
		ImGui::Text("Time Use: %f", timeUse);
		ShowClosedList(GraphContext, gridGraph, 6.0f*  simpleDrawScale, Red, camera);
		ShowPath(GraphContext, gridGraph, 6.0f*  simpleDrawScale, LightGreen, camera);
	}

	int start = startX + (startY * col);
	int end = endX + (endY * col);
	DrawScreenCircle(camera.ConvertToScreen(gridGraph.GetNode(start).position), 10.0f*  simpleDrawScale * camera.GetScale(), LightGreen);
	DrawScreenCircle(camera.ConvertToScreen(gridGraph.GetNode(end).position), 10.0f*  simpleDrawScale * camera.GetScale(), LightGreen);

	if (isGraphUpdate)
	{
		for (int i = 0; i < gridGraph.GetNodeCount(); i++)
		{
			auto& currentNode = gridGraph.GetNode(i);

			Vector2 inScreenPosition = camera.ConvertToScreen(currentNode.position);
			if (!camera.IsOutOfView(inScreenPosition, { textureSize.x *  xScale * camera.GetScale() ,textureSize.y * yScale* camera.GetScale() }))
			{
				DrawSprite(allTiles[gridGraph.GetNode(i).tileType].textureId, inScreenPosition, 0.0f, 1.0f, 0.5f, 0.5f, xScale * camera.GetScale(), yScale* camera.GetScale());
			}
		}
		if (activeNode != -1)
		{
			NFGE::sApp.DrawScreenRect(camera.ConvertToScreen(gridGraph.GetNode(activeNode).rect), Cyan);
			activeNode = -1;
		}
	}

	background.DebugUIUpdate(deltaTime, camera);
}


void TextureGridMap::Render(const Camera2D& camera)
{
	background.Rander(initPosition - Vector2{textureSize.x * xScale * 0.5f, textureSize.y * yScale * 0.5f}, camera);
	
}
