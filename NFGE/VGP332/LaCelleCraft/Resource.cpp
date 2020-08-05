#include "Resource.h"
#include "Worker.h"
#include "LevelManager.h"

using namespace LCUnit;

const int totalResource = 2;
const int totalState = 3;

//-------------------------------------------------------------------------
//---------------------------------------| Money | Matrix |----------------
//---------------------------------------v       v--------v----------------
const int sizeCol[totalResource] = { 4,		 2 };
const int sizeRow[totalResource] = { 2,		 2 };
std::string defaultTexture[totalResource] = { "LaCelleCraft/city.png",	"LaCelleCraft/matrix.png" };
std::string texturePrefix[totalResource][totalState] = { { "LaCelleCraft/city_normal_","LaCelleCraft/city_almost_", "LaCelleCraft/city_gone_" },{ "LaCelleCraft/matrix_normal_", "LaCelleCraft/matrix_almost_" , "LaCelleCraft/matrix_gone_" } };
std::string textureSubFix[totalResource] = { ".png",		".png" };
int frameCount[totalResource][totalState] = { { 36, 12 ,12 },{ 12, 12, 12 } };
const int capacity[totalResource] = { 5,		 3 };

void Resource::Load(TextureGridMap & worldMap, const Vector2 & initPos, ResourceType whichType, int amount)
{
		mType = StructureType::Resource;
		mResourceType = whichType;
		mState = ResourceState::Sufficient;
		mTotalAmount = amount;
		m25PercentAmount = static_cast<int>(static_cast<float>(mTotalAmount) * 0.25f);
		mCurrentAmount = mTotalAmount;

		Structure::Load
		(
			worldMap,
			initPos,
			sizeCol[mResourceType],
			sizeRow[mResourceType],
			defaultTexture[mResourceType],
			capacity[mResourceType]
		);

		for (int i = 0; i < totalState; i++)
		{
			mAnimator.AddClip(LCAnimationClip(texturePrefix[mResourceType][i], textureSubFix[mResourceType], true, frameCount[mResourceType][i]));
		}
	}

void Resource::UpdateWorkers(float deltaTime)
{
	if (mWorkers.empty())
		return;

	for (auto iter = mWorkers.begin(); iter != mWorkers.end();)
	{
		if ((*iter)->Work(deltaTime))
		{
			(*iter)->mCurrentCarryMoney = (int)((*iter)->mTotalWorkTime * (*iter)->mHourlyRate);
			mTotalAmount -= (*iter)->mCurrentCarryMoney;
  			(*iter)->GetOutCity();
			
			mWorkers.erase(iter++);
		}
		else
		{
			iter++;
		}
		if (mWorkers.empty())
			break;
	}
}
