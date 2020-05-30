#include "LCTexture.h"

LCTexture::LCTexture(const Vector2 & size, const Vector2 & pivoit, std::string textureDir)
{
	Load(size, pivoit, textureDir);
}

void LCTexture::Load(const Vector2& size, const Vector2& pivot, std::string textureDir)
{
	mTextureId = LoadTexture(textureDir.c_str());
	mTextureSize = Vector2{ GetSpriteWidth(mTextureId), GetSpriteHeight(mTextureId) };
	mDrawSize = size;
	mCurrentRect = Rect{ 0.0f,0.0f,mDrawSize.x, mDrawSize.y };
	mPivoit = pivot;
}

void LCTexture::Render(const Vector2& pos, const Camera2D & camera, bool IsInScreen)
{
	Vector2 drawPos = IsInScreen ? pos : camera.ConvertToScreen(pos);
	float cameraScale = IsInScreen ? 1.0f : camera.GetScale();

	float drawScaleX = mDrawSize.x / mTextureSize.x;
	float drawScaleY = mDrawSize.y / mTextureSize.y;

	if (!camera.IsOutOfView(drawPos, { mDrawSize.x * cameraScale, mDrawSize.y * cameraScale }))
	{
		DrawSprite(mTextureId, drawPos, mRotation, mAlpha, mPivoit.x, mPivoit.y, drawScaleX * cameraScale, drawScaleY * cameraScale);
	}
}

void LCTexture::ChangeSize(const Vector2 & newSize)
{
	mDrawSize = newSize;
	mCurrentRect = Rect{ 0.0f,0.0f,mDrawSize.x, mDrawSize.y };
}
