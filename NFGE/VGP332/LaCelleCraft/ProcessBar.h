#ifndef LACELLCRAFT_UTILITYELEMENT_RESOURCE_HEADER
#define LACELLCRAFT_UTILITYELEMENT_RESOURCE_HEADER

#include "EngineWrapper.h"

using namespace EngineWrapper;

struct ProcessBar
{
	static float mHeight;
	static float mDownOffSet;
	static int mBorderWidth;
	static TextureId mProcessTexture;
	static Vector2 mProcessTextureSize;
	static TextureId mEmptyTexture;
	static Vector2 mEmptyTextureSize;

	float mPercentage;

	void Load();
	void UnLoad();
	void Update(float processPercentage);
	void Render(const Vector2& leftUpPos, float drawLength, float drawScale);
};

#endif // !LACELLCRAFT_UTILITYELEMENT_RESOURCE_HEADER
