#ifndef LACELLECRAFT_GAMEENTITY_LEVELMANAGER_HEADER
#define LACELLECRAFT_GAMEENTITY_LEVELMANAGER_HEADER


#include "PlayerControllor.h"
#include "Projectile.h"

struct Unit;
struct Building;

struct LevelManager
{
	static void StaticInitialize();
	static void StaticTerminate();
	static LevelManager* Get();

	LevelManager() = default;
	~LevelManager();

	LevelManager(const LevelManager& other) = delete;
	LevelManager& operator=(const LevelManager& other) = delete;

	void Initlialize();
	void Terminate();
	
	void Update(float deltaTime);
	void Render(const Camera2D& camera);
	void UIUpdate(float deltaTime, const Camera2D& camera);
	void DebugUIUpdate(float deltaTime, const Camera2D& camera);
	void LateUpdate(float deltaTime);
	
	//Members
	BattleMap* myBattleMapPtr;
	Camera2D* mWorldCamera;

	std::list<Unit*> mInLevelUnit;
	std::list<Building*> mInLevelBuilding;
	std::list<LC::Projectile*> mInLevelProjectile;
	std::list<LC::Projectile*> mInLevelAOEProjectile;

	std::vector<Commander> mCommanders;
	int mLocalCommandersIndex;
	std::vector<int> mAICommanders;

	PlayerController mPlayerController;

	Unit** RegisterNewUnit(Unit* unit)
	{
		for (std::list<Unit*>::iterator it = mInLevelUnit.begin(); it != mInLevelUnit.end(); ++it)
		{
			if ((*it) == nullptr)
			{
				*it = unit;
				return &(*it);
			}
		}
		mInLevelUnit.push_back(unit);
		return &(mInLevelUnit.back());
	}

	Building** RegisterNewBuilding(Building* building)
	{

		for (std::list<Building*>::iterator it = mInLevelBuilding.begin(); it != mInLevelBuilding.end(); ++it)
		{
			if ((*it) == nullptr)
			{
				*it = building;
				return &(*it);
			}
		}
		mInLevelBuilding.push_back(building);
		return &(mInLevelBuilding.back());
	}

	Projectile** RegisterNewProjectile(Projectile* projectile)
	{

		for (std::list<Projectile*>::iterator it = mInLevelProjectile.begin(); it != mInLevelProjectile.end(); ++it)
		{
			if ((*it) == nullptr)
			{
				*it = projectile;
				return &(*it);
			}
		}
		mInLevelProjectile.push_back(projectile);
		return &(mInLevelProjectile.back());
	}

	Projectile** RegisterNewAoeProjectile(Projectile* projectile)
	{

		for (std::list<Projectile*>::iterator it = mInLevelAOEProjectile.begin(); it != mInLevelAOEProjectile.end(); ++it)
		{
			if ((*it) == nullptr)
			{
				*it = projectile;
				return &(*it);
			}
		}
		mInLevelAOEProjectile.push_back(projectile);
		return &(mInLevelAOEProjectile.back());
	}
};

#endif // !LACELLECRAFT_GAMEENTITY_LEVELMANAGER_HEADER
