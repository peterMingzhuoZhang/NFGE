//====================================================================================================
// Filename:	LCTexture.h
// Created by:	Mingzhuo Zhang
// Description:	Has a structure <LCTexture> which comes from My "LacelleCraft" project, It is the texture
//				that has size information, that allow you to draw a texture in any size any time any where
//====================================================================================================

#ifndef HELLOPERCEPTION_UTILITYELEMENT_LCTEXTURE_HEADER
#define HELLOPERCEPTION_UTILITYELEMENT_LCTEXTURE_HEADER

#include "EngineWrapper.h"

using namespace EngineWrapper;

struct LCTexture
{
	TextureId mTextureId;
	Vector2 mTextureSize;

	Vector2 mDrawSize;
	Rect mCurrentRect;
	Vector2 mPivoit;

	float mRotation = 0.0f;
	float mAlpha = 1.0f;

	LCTexture() = default;
	LCTexture(const Vector2& size, const Vector2& pivoit, std::string textureDir);

	void Load(const Vector2& size, const Vector2& pivoit, std::string textureDir);
	void Render(const Vector2& pos, const Camera2D& camera, bool IsInScreen = false);

	void ChangeSize(const Vector2& newSize);
};

#endif // !LACELLECRAFT_UTILITYELEMENT_LCTEXTURE_HEADER
