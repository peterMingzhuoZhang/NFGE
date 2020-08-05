#include "HealthBar.h"

namespace
{
	std::string healthTextureName = "LaCelleCraft/Util/healthChunck.png";
	std::string emptyTextureName = "LaCelleCraft/Util/emptyChunck.png";
};

float HealthBar::mHeight = 5.0f;
float HealthBar::mUpOffSet = 5.0f;
int HealthBar::mBorderWidth = 1;
TextureId HealthBar::mHealthTexture = 0;
Vector2 HealthBar::mHealthTextureSize = {};
TextureId HealthBar::mEmptyTexture = 0;
Vector2 HealthBar::mEmptyTextureSize = {};

void HealthBar::Load(int sliceCount)
{
	if (mHealthTexture == 0)
	{
		mHealthTexture = LoadTexture(healthTextureName.c_str());
		mHealthTextureSize = { GetSpriteWidth(mHealthTexture),GetSpriteHeight(mHealthTexture) };
	}
	if (mEmptyTexture== 0)
	{
		mEmptyTexture = LoadTexture(emptyTextureName.c_str());
		mEmptyTextureSize = { GetSpriteWidth(mEmptyTexture),GetSpriteHeight(mEmptyTexture) };
	}
	mSliceCount = sliceCount;
	mPercentage = 100.0f;
}

void HealthBar::UnLoad()
{
}

void HealthBar::Update(float healthPercentage)
{
	mPercentage = healthPercentage;
}

void HealthBar::Render(const Vector2& leftUpPos, float drawLength, float drawScale)
{
	float currentHeight = mHeight * drawScale;
	float currentUpOffSet = mUpOffSet;
	int currentBorderWidth = static_cast<int>(static_cast<float>(mBorderWidth) * drawScale);
	currentBorderWidth = currentBorderWidth > 0 ? currentBorderWidth : 1;
	Vector2 currentDrawPos = Vector2{ leftUpPos.x, leftUpPos.y + currentUpOffSet };

	// DrawEmpty
	DrawSprite(mEmptyTexture, currentDrawPos, 0.0f, 1.0f, 0.0f, 1.0f, drawLength / mEmptyTextureSize.x, currentHeight / mEmptyTextureSize.y);

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
	
	// Draw health chunck
	float currentHCWidth = drawLength / static_cast<float>(mSliceCount);
	float currentHWidth = currentHCWidth - currentBorderWidth;
	int drawCount = static_cast<int>(static_cast<float>(mSliceCount) * mPercentage);
	for (int i = 0; i < drawCount; i++)
	{
		float i_float = static_cast<float>(i);
		DrawSprite(mHealthTexture, { currentDrawPos.x + currentHCWidth * i_float, currentDrawPos.y }, 0.0f, 1.0f, 0.0f, 1.0f, currentHWidth / mHealthTextureSize.x, currentHeight / mHealthTextureSize.y);
	}
}
