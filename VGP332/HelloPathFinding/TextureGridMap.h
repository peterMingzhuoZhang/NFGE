//====================================================================================================
// Filename:	TextureGridMap.h
// Created by:	Mingzhuo Zhang
// Description:	Has a structure <TextureGirdMap> which hook up the grid tilemap with AI::Graph
//				It can do :	[Load & save to a file]
//							[Editable in run time with ImGui]
//							[Pathfinding in run time with ImGui]
//====================================================================================================

#ifndef HELLOPATHFINDING_TEXTUREGRIDMAP_HEADER
#define HELLOPATHFINDING_TEXTUREGRIDMAP_HEADER

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
	
	template<typename CanOpenPolicy_Djika, typename CanOpenPolicy_DFSnDFS, typename GetGCostPolicy, typename GetHCostPolicy>
	void Update(float deltaTime);

	
	void Render(const Camera2D& camera) override;
	
	bool IsWall(size_t n);
};

// Template implemention ------------------------------------------------------------------------------------------------------
template<typename CanOpenPolicy_Djika, typename CanOpenPolicy_DFSnDFS, typename GetGCostPolicy, typename GetHCostPolicy>
void TextureGridMap::Update(float deltaTime)
{
	
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
		_itoa_s(i, i_char,10);
		std::string i_str(i_char);
		ImGui::SameLine();
		ImGui::PushItemWidth(50.0f);
		ImGui::SliderFloat(std::string("Cost" + i_str).c_str(), &(allTiles[i].walkThroughCost),0.0f,10.0f);
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

			if (EngineWrapper::PointInRect(Vector2{ static_cast<float>(GetMouseScreenX()), static_cast<float>(GetMouseScreenY()) }, currentNode.rect))
			{
				activeNode = i;
				mouseNotInAll = false;

				if (IsMouseDown(LBUTTON))
				{
					gridGraph.GetNode(i).tileType = currentPaintTile;
					isGraphPathFinded = false;
				}
				if (IsMouseDown(RBUTTON))
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
			PathFinding<AI::Graph<MapNode>, CanOpenPolicy_DFSnDFS, GetGCostPolicy, GetHCostPolicy>(GraphContext, gridGraph, start, end, SearchMethod::BreathFirst);
			auto currentTime = std::chrono::high_resolution_clock::now();
			timeUse = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count() / 1000000.0f;
			isGraphPathFinded = true;
		}
		if (ImGui::Button("Grid Graph DFS"))
		{
			auto lastTime = std::chrono::high_resolution_clock::now();
			PathFinding<AI::Graph<MapNode>, CanOpenPolicy_DFSnDFS, GetGCostPolicy, GetHCostPolicy>(GraphContext, gridGraph, start, end, SearchMethod::DepthFirst);
			auto currentTime = std::chrono::high_resolution_clock::now();
			timeUse = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count() / 1000000.0f;
			isGraphPathFinded = true;
		}
		if (ImGui::Button("Grid Graph Djika"))
		{
			auto lastTime = std::chrono::high_resolution_clock::now();
			PathFinding<AI::Graph<MapNode>, CanOpenPolicy_Djika, GetGCostPolicy, GetHCostPolicy>(GraphContext, gridGraph, start, end, SearchMethod::Djika);
			auto currentTime = std::chrono::high_resolution_clock::now();
			timeUse = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count() / 1000000.0f;
			isGraphPathFinded = true;
		}
		if (ImGui::Button("Grid Graph A_Star"))
		{
			auto lastTime = std::chrono::high_resolution_clock::now();
			PathFinding<AI::Graph<MapNode>, CanOpenPolicy_Djika, GetGCostPolicy, GetHCostPolicy>(GraphContext, gridGraph, start, end, SearchMethod::A_Star);
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

	
}

#endif // !HELLOPATHFINDING_TEXTUREGRIDMAP_HEADER


