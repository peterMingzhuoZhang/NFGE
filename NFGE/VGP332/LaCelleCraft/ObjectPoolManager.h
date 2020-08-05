#ifndef LACELLECRAFT_OBJECTPOOL_HEADER
#define LACELLECRAFT_OBJECTPOOL_HEADER

#include "Unit.h"
#include "Building.h"
#include "Projectile.h"

struct ObjectPoolManager
{
	static void StaticInitialize();
	static void StaticTerminate();
	static ObjectPoolManager* Get();

	ObjectPoolManager() = default;
	~ObjectPoolManager();

	ObjectPoolManager(const ObjectPoolManager& other) = delete;
	ObjectPoolManager& operator=(const ObjectPoolManager& other) = delete;

	void Initlialize();

	void Terminate();

	Unit* GetUnitFromPool(std::string UnitTypeName)
	{
		ASSERT(mUnitPools.find(UnitTypeName) != mUnitPools.end(), "[%s] type can not be found in object pool", UnitTypeName.c_str());
		return GetNextUnit(UnitTypeName);
	}
	
	Building* GetBuildingFromPool(std::string BuildingTypeName)
	{
		ASSERT(mBuildingPools.find(BuildingTypeName) != mBuildingPools.end(), "[%s] type can not be found in object pool", BuildingTypeName.c_str());
		return GetNextBuilding(BuildingTypeName);
	}

	Projectile* GetProjectileFromPool(std::string ProjectileTypeName)
	{
		ASSERT(mProjectilePools.find(ProjectileTypeName) != mProjectilePools.end(), "[%s] type can not be found in object pool", ProjectileTypeName.c_str());
		return GetNextProjectile(ProjectileTypeName);
	}

	std::unordered_map<std::string, std::vector<Unit*>> mUnitPools;
	std::unordered_map<std::string, std::vector<Building*>> mBuildingPools;
	std::unordered_map<std::string, std::vector<Projectile*>> mProjectilePools;
	std::unordered_map<std::string, std::function<void*()>> mCreators;
private:
	static int PoolIncreaseAmout;

	template<class T>
	void Register()
	{
		mCreators[T::GetTypeName()] = T::CreateInstance;
	}

	void* CreateInstance(std::string typeName)
	{
		ASSERT(mCreators.find(typeName) != mCreators.end(), "[ObjectFactory] Type id %s not registered.", typeName.c_str());
		return mCreators[typeName]();
	}

	template <class T>
	T* CreateInstance()
	{
		auto typeId = T::GetTypeId();
		return reinterpret_cast<T*>(CreateInstance(T::GetTypeId()));
	}

	Unit* GetNextUnit(std::string poolName)
	{
		std::vector<Unit*>& thePool = mUnitPools[poolName];
		int i = 0;
		for (; i < thePool.size(); ++i)
		{
			auto& unit = thePool[i];
			if (unit == nullptr)
			{
				LOG("[%s] Pooled object is null", poolName.c_str());
			}
			if ((unit->mIsActive == true) || (unit->mIsBuilding == true))
			{
				continue;
			}
			else
			{
				return unit;
			}
		}
		// increase the pool by 50;
		IncreaseUnitPoolSize(poolName, PoolIncreaseAmout);
		return thePool[i];
	}

	Building* GetNextBuilding(std::string poolName)
	{
		std::vector<Building*>& thePool = mBuildingPools[poolName];
		int i = 0;
		for (; i < thePool.size();++i)
		{
			auto& building = thePool[i];
			if (building == nullptr)
			{
				LOG("[%s] Pooled object is null", poolName.c_str());
			}
			if ((building->mIsActive == true) || (building->mIsInBuild == true))
			{
				continue;
			}
			else
			{
				return building;
			}
		}
		// increase the pool by 50;
		IncreaseBuildingPoolSize(poolName, PoolIncreaseAmout);
		return thePool[i];
	}

	Projectile* GetNextProjectile(std::string poolName)
	{
		std::vector<Projectile*>& thePool = mProjectilePools[poolName];
		int i = 0;
		for (; i < thePool.size(); ++i)
		{
			auto& projectile = thePool[i];
			if (projectile == nullptr)
			{
				LOG("[%s] Pooled object is null", poolName.c_str());
			}
			if ((projectile->mIsActive == true) )
			{
				continue;
			}
			else
			{
				return projectile;
			}
		}
		// increase the pool by 50;
		IncreaseProjectilePoolSize(poolName, PoolIncreaseAmout);
		return thePool[i];
	}

	void IncreaseUnitPoolSize(std::string poolName, int size)
	{
		std::vector<Unit*>& thePool = mUnitPools[poolName];
		for (int i = 0; i < size; i++)
		{
			thePool.push_back(reinterpret_cast<Unit*>(CreateInstance(poolName)));
		}
	}

	void IncreaseBuildingPoolSize(std::string poolName, int size)
	{
		std::vector<Building*>& thePool = mBuildingPools[poolName];
		for (int i = 0; i < size; i++)
		{
			thePool.push_back(reinterpret_cast<Building*>(CreateInstance(poolName)));
		}
	}

	void IncreaseProjectilePoolSize(std::string poolName, int size)
	{
		std::vector<Projectile*>& thePool = mProjectilePools[poolName];
		for (int i = 0; i < size; i++)
		{
			thePool.push_back(reinterpret_cast<Projectile*>(CreateInstance(poolName)));
		}
	}
};

#endif // !LACELLECRAFT_OBJECTPOOL_HEADER
