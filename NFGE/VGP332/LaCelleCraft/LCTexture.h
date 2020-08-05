#ifndef LACELLECRAFT_UTILITYELEMENT_LCTEXTURE_HEADER
#define LACELLECRAFT_UTILITYELEMENT_LCTEXTURE_HEADER

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
