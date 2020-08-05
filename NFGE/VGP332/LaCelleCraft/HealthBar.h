#ifndef LACELLECRAFT_UTILITYELEMENT_HEALTHBAR_HEADER
#define LACELLECRAFT_UTILITYELEMENT_HEALTHBAR_HEADER

#include "EngineWrapper.h"

using namespace EngineWrapper;

struct HealthBar
{
	static float mHeight;
	static float mUpOffSet;
	static int mBorderWidth;
	static TextureId mHealthTexture;
	static Vector2 mHealthTextureSize;
	static TextureId mEmptyTexture;
	static Vector2 mEmptyTextureSize;

	float mPercentage;
	int mSliceCount;

	void Load(int sliceCount);
	void UnLoad();
	void Update(float healthPercentage);
	void Render(const Vector2& leftUpPos, float drawLength, float drawScale);
};

#endif // !LACELLECRAFT_UTILITYELEMENT_HEALTHBAR_HEADER
