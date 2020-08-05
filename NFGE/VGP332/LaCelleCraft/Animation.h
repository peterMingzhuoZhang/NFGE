#ifndef LACELLECRAFT_UTILITYELEMENT_ANIMATION_HEADER
#define LACELLECRAFT_UTILITYELEMENT_ANIMATION_HEADER

#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

#include "GameSettingManager.h"
#include <RapidJSON/Inc/document.h>
#include <RapidJSON/Inc/filereadstream.h>

using AtlasInfo = std::vector<NFGE::Math::Rect>;

//----------------------------------------------------------------------------------------------//|
//{																								//|
//	"frames":																					//|
//	{																							//|
//		"PCL_worker_idle_0.png":																//|
//		{																						//|
//			"frame":{"x":0, "y" : 0, "w" : 32, "h" : 32},										//|
//			"rotated" : false,																	//|
//			"trimmed" : false,																	//|
//			"spriteSourceSize" : {"x":0, "y" : 0, "w" : 32, "h" : 32},							//|
//			"sourceSize" : {"w":32, "h" : 32}													//|
//		},																						//|
//		"PCL_worker_idle_1.png":																//|
//		{																						//|
//			"frame":{"x":32, "y" : 0, "w" : 32, "h" : 32},										//|--- Sample json structure
//			"rotated" : false,																	//|
//			"trimmed" : false,																	//|
//			"spriteSourceSize" : {"x":0, "y" : 0, "w" : 32, "h" : 32},							//|
//			"sourceSize" : {"w":32, "h" : 32}													//|
//		}																						//|
//	},																							//|
//	"meta":																						//|
//	{																							//|
//		"app":"https://github.com/piskelapp/piskel/", 											//|
//			"version" : "1.0", 																	//|
//			"image" : "PCL_worker_idle_.png", 													//|
//			"format" : "RGBA8888", 																//|
//			"size" : {"w":192, "h" : 192}														//|
//	}																							//|
//}																								//|
//----------------------------------------------------------------------------------------------//|

struct Unit;
struct LCAnimationClip
{
	std::vector<TextureId> mAllTextures;
	std::vector<AtlasInfo> mAllJsonInfos;
	int mTotalFrame;
	bool mIsLoop = false;
	int mToClipWhenFinish;

	LCAnimationClip() = default;
	LCAnimationClip(std::string textureNamePrefix, std::string textureNameSubfix, bool isLoop, int totalFrame, int direction = 1, int toClipWhenFinish = -1)
		:mTotalFrame(totalFrame)
		,mIsLoop(isLoop)
		,mToClipWhenFinish(toClipWhenFinish)
	{
		if (!mIsLoop)
		{
			ASSERT(mToClipWhenFinish != -1, "[Animation] Not looped animation must have some clip to go when finish.");
		}

		mAllTextures.reserve(direction * totalFrame);
		if (direction != 1)
		{
			for (int j = 0; j < direction; ++j)
			{
				for (int i = 0; i < mTotalFrame; i++)
				{
					if (j < 10)
					{
						if (direction == 1)
							mAllTextures.push_back(LoadTexture((textureNamePrefix + "0" + std::to_string(i) + textureNameSubfix).c_str()));
						else
							mAllTextures.push_back(LoadTexture((textureNamePrefix + "0" + std::to_string(j) + "_" + std::to_string(i) + textureNameSubfix).c_str()));
					}
					else
					{
						if (direction == 1)
							mAllTextures.push_back(LoadTexture((textureNamePrefix + std::to_string(i) + textureNameSubfix).c_str()));
						else
							mAllTextures.push_back(LoadTexture((textureNamePrefix + std::to_string(j) + "_" + std::to_string(i) + textureNameSubfix).c_str()));
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < mTotalFrame; i++)
			{
				if (i < 10)
				{
					mAllTextures.push_back(LoadTexture((textureNamePrefix + "0" + std::to_string(i) + textureNameSubfix).c_str()));
				}
				else
				{
					mAllTextures.push_back(LoadTexture((textureNamePrefix + std::to_string(i) + textureNameSubfix).c_str()));
				}
			}
		}
		
	}

	LCAnimationClip(std::string textureNamePrefix, std::string textureNameSubfix, bool isLoop, int totalFrame, Unit& unit, int toClipWhenFinish = -1);	// New Constructor, with atlasRender, also reset the direction count of the unit who has it

};

struct LCAnimator
{
	int mClipCount = 0;
	std::vector<LCAnimationClip> mAllClips;

	int mState = 0;
	int mFPS = 6;
	int mCurrentFrame = 0;
	int mMaxFrame = 0;

	float mFrameTime;
	float mFrameTimeCounter = 0.0f;

	bool isWaitFinishAndChangeState = false;
	int toState = 0;

	void Load();
	void Update(float deltaTime);
	TextureId GetCurrentTexture();
	TextureId GetCurrentTextureWithDirection(int direction);

	void Render(int currentDirection, const Vector2& drawPos, float rotation, float alpha, float anchorX, float anchorY, float scaleX, float scaleY, const Camera2D& camera);	// New render method with AtlasJson [Warnning: This method only for unit]

	void UnLoad() {};

	void AddClip(const LCAnimationClip&& clip);
	void ChangeToState(int state, bool isWaitFinish);
};

#endif // !LACELLECRAFT_UTILITYELEMENT_ANIMATION_HEADER
