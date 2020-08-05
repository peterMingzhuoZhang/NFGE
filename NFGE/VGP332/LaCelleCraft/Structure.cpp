#include "Structure.h"
#include "LevelManager.h"

using namespace NFGE::Input;

void Structure::Load(TextureGridMap & worldMap, const Vector2 & initPos, int col, int row, std::string defaultTexture, int capacity)
{
	mIsActive = true;
	mWorldMap = &worldMap;
	mSizeCol = col;
	mSizeRow = row;
	ASSERT((mSizeCol != 0 && mSizeRow != 0), "[Structure] sizeCol or sizeRow is zero.");
	if (TryBuildOnMap(initPos) == false)
	{
		mIsActive = false;
		LOG("[Resource] invalid build position.");
		return;
	}
	
	PutOnMap();


	mTextureRenderOffSet = Vector2{ -worldMap.textureSize.x * 0.5f * worldMap.xScale, worldMap.textureSize.y * 0.5f * worldMap.yScale };
	mAnimator.Load();
	mDefaultTexture = LoadTexture(defaultTexture.c_str());
	mDefaultWidth = GetSpriteWidth(mDefaultTexture);
	mDefaultHeight = GetSpriteHeight(mDefaultTexture);
	mDrawScale = (worldMap.textureSize.x * worldMap.xScale * mSizeCol) / mDefaultWidth;



	mCapacity = capacity;
	for (int i = 0; i < mCapacity; i++)
	{
		room.emplace_back();
	}
}

void Structure::Update(float deltaTime)
{
	if (!mIsActive)
		return;

	if (mAnimator.mClipCount != 0)
	{
		mAnimator.Update(deltaTime);
	}

	auto& levelManager = *LevelManager::Get();
	Vector2 checkPos = levelManager.mWorldCamera->ConvertToScreen(mPosition + mTextureRenderOffSet + Vector2{ mDefaultWidth * mDrawScale * 0.5f, -mDefaultHeight * mDrawScale * 0.5f });
	mIsMouseOnTop = EngineWrapper::PointInCircle(levelManager.mPlayerController.mCurrentMousePos, Circle{ checkPos, mDefaultWidth * mDrawScale * 0.5f });
	if (mIsMouseOnTop)
	{
		levelManager.mPlayerController.mIsHighLight = true;
		if (levelManager.mPlayerController.mCurrentHighLightStructure != this)
		{
			levelManager.mPlayerController.mCurrentHighLightUnit = nullptr;
			switch (mType)
			{
			case StructureType::Resource:
				levelManager.mPlayerController.mCurrentHighLightStructure = nullptr;
				levelManager.mPlayerController.mCurrentHighLightResource = this;
				break;
			case StructureType::Building:
				levelManager.mPlayerController.mCurrentHighLightResource = nullptr;
				levelManager.mPlayerController.mCurrentHighLightStructure = this;
				break;
			default:
				break;
			}
			
		}
	}
}

void Structure::DebugUIUpdate(float deltaTime, const Camera2D & camera)
{
	if (!mIsActive)
		return;
	Vector2 mousePos = { static_cast<float>(GetMouseScreenX()), static_cast<float>(GetMouseScreenY()) };
	bool isSelectedHappen = IsMousePressed(MouseButton::LBUTTON) ? true : false;
	if (isSelectedHappen)
	{
		if (mIsEditorSelected)
		{
			TryBuildOnMap(mPosition, mOldPosition);
			mIsEditorSelected = false;
		}
		else
		{
			if (EngineWrapper::PointInRect(mousePos, camera.ConvertToScreen(GetCurrentRect())))
			{
				mIsEditorSelected = true;
				mOldPosition = mPosition;
			}
			else
				mIsEditorSelected = false;
		}
	}

	if (mIsEditorSelected)
	{
		int currentWorldIndex = mWorldMap->ConvertToMapIndex(camera.ConvertToWorld(mousePos));
		if ((currentWorldIndex > 0) && (currentWorldIndex < mWorldMap->gridGraph.GetNodeCount()))
		{
			mPosition = mWorldMap->gridGraph.GetNode(currentWorldIndex).position;
			for (int i = 0; i < mSizeCol; i++)
			{
				for (int j = 0; j < mSizeRow; j++)
				{
					NFGE::Graphics::Color renderColor = Green;
					int index = currentWorldIndex - j * mWorldMap->col + i;
					if ((index < 0) || (index > mWorldMap->gridGraph.GetNodeCount()))
					{
						continue;
					}
					if (mWorldMap->IsWall(index))
					{
						renderColor = Red;
					}
					DrawScreenRect(camera.ConvertToScreen(mWorldMap->gridGraph.GetNode(index).rect), renderColor);
				}
			}
		}
	}
	
	Vector2 drawPos = camera.ConvertToScreen(mPosition + mTextureRenderOffSet);
	if (!camera.IsOutOfView(drawPos, { mDefaultWidth * mDrawScale * camera.GetScale(), mDefaultHeight * mDrawScale * camera.GetScale() }))
	{
		for (auto& tileIndex : mOccupiedTile)
		{
			auto screenRect = camera.ConvertToScreen(mWorldMap->gridGraph.GetNode(tileIndex).rect);
			if (tileIndex >= 0)
			{
				DrawScreenRect(screenRect, Yellow);
			}
		}
	}

	Vector2 checkPos = camera.ConvertToScreen(mPosition + mTextureRenderOffSet + Vector2{ mDefaultWidth * mDrawScale * 0.5f, -mDefaultHeight * mDrawScale * 0.5f });
	DrawScreenCircle(checkPos, mDefaultWidth * mDrawScale * 0.5f, Red);
}

void Structure::Unload()
{
}
