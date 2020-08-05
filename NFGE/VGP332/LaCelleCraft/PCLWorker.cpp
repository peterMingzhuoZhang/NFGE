#include "PCLWorker.h"
#include "ObjectPoolManager.h"
#include "LevelManager.h"

using namespace NFGE::Input;
using namespace LCUnit;

namespace
{
	std::string defaultTexturePrefix = "LaCelleCraft/PeterChanLegend/Unit/Worker/PCL_worker_idel_";
	std::string defaultTextureSubfix = ".png";
	const Vector2 size{ 64.0f,64.0f };
	const float maxHealth = 10.0f;
	const float workerMass = 1.0f;
	const float workerMaxSpeed = 200.0f;
	const float turningSpeed = 10.0f;
	const float Altitude = -10.0f;
	const float hourlyRate = 15.0f;
	const float totalWorkTime = 8.0f;

	const float attackSpeed = 1.0f;
	const float attack = 5.0f;
	const float attackRange = 100.0f;

	std::string texturePrefix[WorkerState::TotalStateCount] =
	{
		
		"LaCelleCraft/PeterChanLegend/Unit/Worker/PCL_worker_idel_",
		"LaCelleCraft/PeterChanLegend/Unit/Worker/PCL_worker_idel_",
		"LaCelleCraft/PeterChanLegend/Unit/Worker/PCL_worker_idel_",
		"LaCelleCraft/PeterChanLegend/Unit/Worker/PCL_worker_idel_",
		"LaCelleCraft/PeterChanLegend/Unit/Worker/PCL_worker_idel_",
		"LaCelleCraft/PeterChanLegend/Unit/Worker/PCL_worker_idel_",
	};
	std::string textureSubfix = ".png";
	const int frameCount[WorkerState::TotalStateCount] =
	{
		1, 1 ,1, 1, 1, 1
	};
	const bool isLoop[WorkerState::TotalStateCount] =
	{
		 true, true, false, false, true, false
	};
	const int toState[WorkerState::TotalStateCount] =
	{
		 -1, -1, WorkerState::Idle, WorkerState::Idle, -1, WorkerState::Idle
	};

	const int totalProjectile = 1;
	std::string projectiles[totalProjectile] = { "PSmallSpark" };
}

TextureId PCLWorker::mDefaultTexture[PCLWorker::UnitDefaultDirectionCount];
PCLWorker::PCLWorker()
	:Worker()
{
	mIsActive = false;
	mIsWorking = false;
	if (mDefaultTexture[0] == NULL)
	{
		for (int i = 0; i < UnitDefaultDirectionCount; i++)
		{
			if (i < 10)
			{
				mDefaultTexture[i] = LoadTexture((defaultTexturePrefix + "0" + std::to_string(i) + "_0" + defaultTextureSubfix).c_str());
			}
			else
			{
				mDefaultTexture[i] = LoadTexture((defaultTexturePrefix + std::to_string(i) + "_0" + defaultTextureSubfix).c_str());
			}
		}
	}

	mDefaultWidth = GetSpriteWidth(mDefaultTexture[0]);
	mDefaultHeight = GetSpriteHeight(mDefaultTexture[0]);
	
	for (int i = 0; i < WorkerState::TotalStateCount; i++)
	{
		directionCount = 0;											// Essiential. Do this lane for [Animator] constructor check for <directionCount> consistancy of this [Unit]
		mAnimator.AddClip(LCAnimationClip(texturePrefix[i], textureSubfix, isLoop[i], frameCount[i], *this, toState[i]));
	}

	mStatus.mAttack = attack;
	mStatus.mAttackSpeed = attackSpeed;
	mStatus.mAttackSpeedCounter = attackSpeed;
	mStatus.mAttackRange = attackRange;

	mProjectileInventory.reserve(totalProjectile);
	for (int i = 0; i < totalProjectile; ++i)
		mProjectileInventory.emplace_back(projectiles[i]);
}

void PCLWorker::Load(TextureGridMap & worldMap, const Vector2 & initPos, int commanderIndex)
{
	Worker::Load(worldMap, initPos, commanderIndex, size, maxHealth);

	mass = workerMass;
	maxSpeed = workerMaxSpeed;
	mTurningSpeed = turningSpeed;
	mAltitude = Altitude;
	mHourlyRate = hourlyRate;
	mTotalWorkTime = totalWorkTime;
	mWorkTimeCounter = mTotalWorkTime;
}

void PCLWorker::Attack(Unit & unit, float deltaTime)
{
}

void PCLWorker::Attack(Building & building, float deltaTime)
{
	if (mStatus.mAttackSpeedCounter <= 0.0f)
	{
		auto levelManager = LevelManager::Get();
		//Fire
		auto bullet = ObjectPoolManager::Get()->GetProjectileFromPool(mProjectileInventory[0]);

		int randomtile = NFGE::Math::Random() % building.mOccupiedTile.size();
		TextureGridMap& worldMap = *(levelManager->myBattleMapPtr);
		auto initPos = worldMap.gridGraph.GetNode(building.mOccupiedTile[randomtile]).position;

		bullet->mDmg = mStatus.mAttack;
		bullet->Fire(initPos, { 1.0f,1.0f }, mCommanderIndex, building);
		mStatus.mAttackSpeedCounter = mStatus.mAttackSpeed;
	}
	else
	{
		mStatus.mAttackSpeedCounter -= deltaTime;
	}
}

void PCLWorker::TakeDamage(LC::Projectile & bullet)
{
}

void PCLWorker::Die()
{
	Unit::Die();
}
