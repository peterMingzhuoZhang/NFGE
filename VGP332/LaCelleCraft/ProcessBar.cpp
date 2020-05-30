#include "ProcessBar.h"

namespace
{
	std::string processTextureName = "LaCelleCraft/Util/processChunck.png";
	std::string emptyTextureName = "LaCelleCraft/Util/emptyChunck.png";
};

float ProcessBar::mHeight = 5.0f;
float ProcessBar::mDownOffSet = 5.0f;
int ProcessBar::mBorderWidth = 1;
TextureId ProcessBar::mProcessTexture = 0;
Vector2 ProcessBar::mProcessTextureSize = {};
TextureId ProcessBar::mEmptyTexture = 0;
Vector2 ProcessBar::mEmptyTextureSize = {};

void ProcessBar::Load()
{
	if (mProcessTexture == 0)
	{
		mProcessTexture = LoadTexture(processTextureName.c_str());
		mProcessTextureSize = { GetSpriteWidth(mProcessTexture),GetSpriteHeight(mProcessTexture) };
	}
	if (mEmptyTexture == 0)
	{
		mEmptyTexture = LoadTexture(emptyTextureName.c_str());
		mEmptyTextureSize = { GetSpriteWidth(mEmptyTexture),GetSpriteHeight(mEmptyTexture) };
	}
	mPercentage = 0.0f;
}

void ProcessBar::UnLoad()
{
}

void ProcessBar::Update(float processPercentage)
{
	mPercentage = processPercentage;
}

void ProcessBar::Render(const Vector2 & leftUpPos, float drawLength, float drawScale)
{
	float currentHeight = mHeight * drawScale;
	float currentDownOffSet = mDownOffSet * drawScale;
	int currentBorderWidth = static_cast<int>(static_cast<float>(mBorderWidth) * drawScale);
	currentBorderWidth = currentBorderWidth > 0 ? currentBorderWidth : 1;
	Vector2 currentDrawPos{ leftUpPos.x, leftUpPos.y - currentDownOffSet };

	if (mPercentage == 0.0f)
		return;

	DrawSprite(mEmptyTexture, currentDrawPos, 0.0f, 1.0f, 0.0f, 1.0f, drawLength / mEmptyTextureSize.x, currentHeight / mEmptyTextureSize.y);
	DrawSprite(mProcessTexture, currentDrawPos, 0.0f, 1.0f, 0.0f, 1.0f, (drawLength / mEmptyTextureSize.x) * mPercentage, currentHeight / mEmptyTextureSize.y);
	
	// Draw Boarder
	for (int i = 0; i < currentBorderWidth; i++)
	{
		float i_float = static_cast<float>(i);
		Rect currentRect
		{
			currentDrawPos.x - i,
			currentDrawPos.y - currentHeight - i,
			currentDrawPos.x + drawLength + i,
			currentDrawPos.y - i
		};
		DrawScreenRect(currentRect, White);
	}
}
