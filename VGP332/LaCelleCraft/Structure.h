#ifndef LACELLCRAFT_GAMEOBJECT_STRUCTURE_HEADER
#define LACELLCRAFT_GAMEOBJECT_STRUCTURE_HEADER

#include "Animation.h"
#include "TextureGridMap.h"
#include "Unit.h"
using namespace EngineWrapper;

enum class StructureType
{
	Resource = 0,
	Building,
};

struct Structure
{
	bool mIsActive = false;

	StructureType mType;

	TextureGridMap* mWorldMap;
	Vector2 mPosition;
	Vector2 mOldPosition;
	int mSizeRow;
	int mSizeCol;
	Vector2 mTextureRenderOffSet;
	LCAnimator mAnimator;
	TextureId mDefaultTexture;
	float mDefaultWidth;
	float mDefaultHeight;
	float mDrawScale;

	std::vector<int> mOccupiedTile;
	std::vector<int> mGetInTile;

	int mCapacity;
	std::vector<Unit*> room;

	//------------------
	bool mIsSelected = false;
	bool mIsEditorSelected = false;
	bool mIsMouseOnTop = false;
	//------------------

	void Load(TextureGridMap& worldMap, const Vector2& initPos, int col, int row, std::string defaultTexture, int capacity);
	

	virtual void Update(float deltaTime);
	

	virtual void Render(const Camera2D& camera)
	{
		if (!mIsActive)
			return;
		Vector2 drawPos = camera.ConvertToScreen(mPosition + mTextureRenderOffSet);
		if (!camera.IsOutOfView(drawPos, {mDefaultWidth * mDrawScale * camera.GetScale() *2.0f, mDefaultHeight * mDrawScale * camera.GetScale()*2.0f }))
		{
			if (mAnimator.mClipCount != 0)
			{
				DrawSprite(mAnimator.GetCurrentTexture(), drawPos, 0.0f, 1.0f, 0.0f, 1.0f, mDrawScale * camera.GetScale(), mDrawScale * camera.GetScale());
				//DrawScreenCircle(drawPos, 5.0f, Red);
			}
			else
			{
				DrawSprite(mDefaultTexture, drawPos, 0.0f, 1.0f, 0.0f, 1.0f, mDrawScale * camera.GetScale(), mDrawScale * camera.GetScale());
			}
		}

		
	}

	virtual void DebugUIUpdate(float deltaTime, const Camera2D& camera);
	void Unload();

	bool TryBuildOnMap(const Vector2& newPos, const Vector2& oldPos = Vector2{0.0f,0.0f})
	{
		int worldMapIndex = mWorldMap->ConvertToMapIndex(newPos);
		
		std::vector<int> temp = mOccupiedTile;
		mOccupiedTile.clear();
		for (int i = 0; i < mSizeRow; i++)
		{
			for (int j = 0; j < mSizeCol; j++)
			{
				int index = worldMapIndex - i * mWorldMap->col + j;
				if ((index < 0) || (index > mWorldMap->gridGraph.GetNodeCount()))
				{
					WrongBuildHandle(oldPos);
					mOccupiedTile = temp;
					return false;
				}
				if(mWorldMap->IsWall(index))
				{
					WrongBuildHandle(oldPos);
					mOccupiedTile = temp;
					return false;
				}
				mOccupiedTile.push_back(index);
			}
		}
		mPosition = mWorldMap->gridGraph.GetNode(worldMapIndex).position;
		
		return true;
	}

	bool Selecting(const Vector2& mousePos, const Camera2D& camera)
	{
		Vector2 checkPos = camera.ConvertToScreen(mPosition + mTextureRenderOffSet + Vector2{ mDefaultWidth * mDrawScale * 0.5f, -mDefaultHeight * mDrawScale * 0.5f });
		mIsSelected = EngineWrapper::PointInCircle(mousePos, Circle{ checkPos, mDefaultWidth * mDrawScale * 0.5f });
		return mIsSelected;
	}
	bool Selecting(const Rect& selectingRect, const Camera2D& camera)
	{
		Vector2 checkPos = camera.ConvertToScreen(mPosition + mTextureRenderOffSet + Vector2{ mDefaultWidth * mDrawScale * 0.5f, -mDefaultHeight * mDrawScale * 0.5f });
		mIsSelected = EngineWrapper::PointInRect(checkPos, selectingRect);
		return mIsSelected;
	}
	void DeSelecting()
	{
		mIsSelected = false;
	}
	virtual void Interact() = 0;

	void RemoveFromMap()
	{
		SetOccupiToFree();
	}

	void PutOnMap()
	{
		SetOccupiToWall();
		SetGetInTile();
	}
protected:
	void WrongBuildHandle(const Vector2& backToPos)
	{
		mPosition = backToPos;
		mIsActive = false;
	}

	Rect GetCurrentRect()
	{
		Vector2 pos = mPosition + mTextureRenderOffSet;
		return Rect
		(
			pos.x,
			pos.y - mSizeRow * mWorldMap->textureSize.y * mWorldMap->yScale,
			pos.x + mSizeCol * mWorldMap->textureSize.x * mWorldMap->xScale,
			pos.y
		);
	}
	void SetOccupiToWall()
	{
		ASSERT(mOccupiedTile.size() != 0, "[Structure] not initlize properly");
		for (auto& tile : mOccupiedTile)
		{
			mWorldMap->gridGraph.GetNode(tile).tileType = 1;	// wall is 1
		}
	}

	void SetGetInTile()
	{
		ASSERT(mOccupiedTile.size() != 0, "[Structure] not initlize properly");
		mGetInTile.push_back(mOccupiedTile[0]);
		mGetInTile.push_back(mOccupiedTile[mSizeCol - 1]);
		for (auto& tile : mGetInTile)
		{
			mWorldMap->gridGraph.GetNode(tile).tileType = 0;	// wall is 1
		}
	}

	void SetOccupiToFree()
	{
		ASSERT(mOccupiedTile.size() != 0, "[Structure] not initlize properly");
		for (auto& tile : mOccupiedTile)
		{
			mWorldMap->gridGraph.GetNode(tile).tileType = 0;	// wall is 1
		}
	}
	
};

#endif // !LACELLCRAFT_GAMEOBJECT_STRUCTURE_HEADER
