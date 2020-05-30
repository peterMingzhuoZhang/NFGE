#ifndef LACELLCRAFT_GAMEOBJECT_BATTLEMAP_HEADER
#define LACELLCRAFT_GAMEOBJECT_BATTLEMAP_HEADER

#include "Resource.h"
#include "Base.h"

struct BattleMap : TextureGridMap, public AI::AIWorld
{
	std::vector<Resource> mAllResource;
	std::vector<Base> mBases;
	int mTotalCommander;

	void Load(std::string fileName);
	void Unload();
	void Update(float deltaTime);
	void Render(const Camera2D& camera);
	void DebugUIUpdate(float deltaTime, const Camera2D& camera);
	void FindPath(int start, int end) override;

private:
	void Serialize_txt(std::ofstream& myFile) override;
	void Deserialize_txt(std::ifstream& theFile) override;
};

#endif // !LACELLCRAFT_GAMEOBJECT_BATTLEMAP_HEADER
