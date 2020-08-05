#ifndef LACELLECRAFT_TEXTUREGRIDMAP_HEADER
#define LACELLECRAFT_TEXTUREGRIDMAP_HEADER

#include "PathFindingMapShell.h"
#include "Background.h"

#include <fstream>
#include <string>
#include <sstream>

struct TextureGridMap : PathFindingMapShell
{
	// Members
	AI::Graph<MapNode> gridGraph;
	int col = 10;
	int row = 10;
	int startX = 0, startY = 0;
	int endX = 0, endY = 0;
	std::vector<Tile> allTiles;
	Background background;
	std::string backGoundImageName;
	int currentPaintTile = 0;
	Vector2 textureSize;
	
	float xScale = 1.0f;
	float yScale = 1.0f;

	void Save(std::string fileName);
	void Open(std::string fileName);

	bool isShowTile = false;
	int activeNode = -1;

	// Functions
	void Load() override;
	void Load(const Vector2& initPos, const Vector2& maxSize, std::string backgroundName);
	void Unload() override;
	
	void Update(float deltaTime);

	void DebugUIUpdate(float deltaTime,const Camera2D & camera);
	
	void Render(const Camera2D& camera) override;
	
	bool IsWall(size_t n);

	int ConvertToMapIndex(const Vector2& position) const;
	Vector2 GetRowAndCol(int tileIndex) const;
	virtual void FindPath(int start, int end);

protected:
	virtual void Serialize_txt(std::ofstream& myFile);
	virtual void Deserialize_txt(std::ifstream& theFile);
};

// Template implemention ------------------------------------------------------------------------------------------------------




#endif // !HELLOPATHFINDING_TEXTUREGRIDMAP_HEADER


