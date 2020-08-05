#include "ObjectPoolManager.h"

// -Units-
#include "PCLWorker.h"
#include "Carrier.h"
#include "Interceptor.h"

// -Buildings-
#include "Base.h"

// -Projectiles-
#include "PSmallSpark.h"

using namespace LCUnit;

int ObjectPoolManager::PoolIncreaseAmout = 50;

namespace
{
	std::unique_ptr<ObjectPoolManager> sObjectPoolManager;
	
	struct PooledObject
	{
		std::string typeName;
		int pooledAmount;
	};

	const int pooledUnitCount = 3;
	const PooledObject pooledUnitObjects[pooledUnitCount] = 
	{
		{ "PCLWorker", 25},
		{ "Carrier", 25},
		{"Interceptor",50}
	};

	const int pooledBuildingCount = 1;
	const PooledObject pooledBuildingObjects[pooledUnitCount] =
	{
		{ "Base", 25 }
	};

	const int pooledProjectileCount = 1;
	const PooledObject pooledProjectileObjects[pooledProjectileCount] =
	{
		{ "PSmallSpark", 25}
	};
}

void ObjectPoolManager::StaticInitialize()
{
	ASSERT(sObjectPoolManager == nullptr, "[sObjectPoolManager] alrerady initialized!");
	sObjectPoolManager = std::make_unique<ObjectPoolManager>();
	sObjectPoolManager->Initlialize();
}

void ObjectPoolManager::StaticTerminate()
{
	if (sObjectPoolManager != nullptr)
	{
		sObjectPoolManager->Terminate();
		sObjectPoolManager.reset();
	}
}

ObjectPoolManager * ObjectPoolManager::Get()
{
	ASSERT(sObjectPoolManager != nullptr, "[GameSettingManager] not initialized!");
	return sObjectPoolManager.get();
}

ObjectPoolManager::~ObjectPoolManager()
{
	ASSERT(sObjectPoolManager == nullptr, "[GameSettingManager] Terminate must be called to clean up!");
}

void ObjectPoolManager::Initlialize()
{
	// Register all ObjectType need to pooled
	Register<Base>();
	Register<PCLWorker>();
	Register<Carrier>();
	Register<Interceptor>();
	Register<PSmallSpark>();

	for (size_t i = 0; i < pooledUnitCount; i++)
	{
		mUnitPools[pooledUnitObjects[i].typeName] = std::vector<Unit*>();
		IncreaseUnitPoolSize(pooledUnitObjects[i].typeName, pooledUnitObjects[i].pooledAmount);
	}

	for (size_t i = 0; i < pooledBuildingCount; i++)
	{
		mBuildingPools[pooledBuildingObjects[i].typeName] = std::vector<Building*>();
		IncreaseBuildingPoolSize(pooledBuildingObjects[i].typeName, pooledBuildingObjects[i].pooledAmount);
	}

	for (size_t i = 0; i < pooledProjectileCount; i++)
	{
		mProjectilePools[pooledProjectileObjects[i].typeName] = std::vector<Projectile*>();
		IncreaseProjectilePoolSize(pooledProjectileObjects[i].typeName, pooledProjectileObjects[i].pooledAmount);
	}
}

void ObjectPoolManager::Terminate()
{
	for (size_t i = 0; i < pooledUnitCount; i++)
	{
		for (int j = 0; j < mUnitPools[pooledUnitObjects[i].typeName].size(); ++j)
		{
			delete mUnitPools[pooledUnitObjects[i].typeName][j];
		}
	}

	for (size_t i = 0; i < pooledBuildingCount; i++)
	{
		for (int j = 0; j < mBuildingPools[pooledBuildingObjects[i].typeName].size(); ++j)
		{
			delete mBuildingPools[pooledBuildingObjects[i].typeName][j];
		}
	}
}
