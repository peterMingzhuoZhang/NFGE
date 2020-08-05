#include "Animation.h"
#include "Unit.h"

namespace
{
	void LoadAtlasInfo(const char* fileName, AtlasInfo& jsonInfo, int& infoCount, std::string& texturePrefix)
	{
		FILE* file = nullptr;
		fopen_s(&file, fileName, "rb");

		//create a new file  stream to in the json data.
		char buffer[65536];
		rapidjson::FileReadStream readStream(file, buffer, std::size(buffer));

		//Feed the json data into the document object so it is
		//easy to ask for the contents
		rapidjson::Document document;
		document.ParseStream(readStream);

		ASSERT(document.HasMember("frames"), "AtlasJson format does not match");
		auto& firstEntry = document["frames"];

		if (infoCount != 0)
			ASSERT(infoCount == firstEntry.MemberCount(), "[Animation] Load animations of A Unit, But animations does not have same direction count. NOT ALLOWED!");
		else
			infoCount = firstEntry.MemberCount();

		for (auto it = firstEntry.MemberBegin();it != firstEntry.MemberEnd(); ++it)
		{	
			ASSERT(it->value.HasMember("frame"), "AtlasJson format does not match");
			auto& rectInfoEntry = it->value["frame"];

			//Then get the rectangle area.
			ASSERT(rectInfoEntry.HasMember("x"), "AtlasJson format does not match");
			float x = rectInfoEntry.FindMember("x")->value.GetFloat();
			ASSERT(rectInfoEntry.HasMember("y"), "AtlasJson format does not match");
			float y = rectInfoEntry.FindMember("y")->value.GetFloat();
			ASSERT(rectInfoEntry.HasMember("w"), "AtlasJson format does not match");
			float w = rectInfoEntry.FindMember("w")->value.GetFloat();
			ASSERT(rectInfoEntry.HasMember("h"), "AtlasJson format does not match");
			float h = rectInfoEntry.FindMember("h")->value.GetFloat();

			NFGE::Math::Rect rect(
				x,
				y,
				x + w,
				y + h
			);
			//Add the info to our look up table.
			jsonInfo.emplace_back(rect);
		}

		

		fclose(file);
	}
}

void LCAnimator::Load()
{
	mFPS = GameSettingManager::Get()->GetAnimationFPS();
	mFrameTime = 1.0f / mFPS;
}

void LCAnimator::Update(float deltaTime)
{
	if (mFrameTimeCounter <= 0.0f)
	{
		if (mAllClips[mState].mIsLoop == false)
		{
			if ((mCurrentFrame + 1) >= mAllClips[mState].mTotalFrame)
			{
				mCurrentFrame = 0;
				ASSERT(mAllClips[mState].mToClipWhenFinish != -1, "[Animator] try to switch to invalided state");
				mState = mAllClips[mState].mToClipWhenFinish;
			}
			else
			{
				mCurrentFrame = (mCurrentFrame + 1) >= mAllClips[mState].mTotalFrame ? 0 : (mCurrentFrame + 1);
			}
		}
		else
		{
			mCurrentFrame = (mCurrentFrame + 1) >= mAllClips[mState].mTotalFrame ? 0 : (mCurrentFrame + 1);
		}
		mFrameTimeCounter = mFrameTime;
	}
	mFrameTimeCounter -= deltaTime;

	if (isWaitFinishAndChangeState)
	{
		if (mCurrentFrame >= mAllClips[mState].mTotalFrame - 1)
		{
			ASSERT(toState != -1, "[Animator] try to switch to invalided state");
			mState = toState;
			toState = mAllClips[toState].mToClipWhenFinish;		//toState = -1;		2019/12/09	[Mingzhuo Zhang] I think there is a bug by reading code, and did this change;
			mCurrentFrame = 0;
			isWaitFinishAndChangeState = false;
		}
	}
}

TextureId LCAnimator::GetCurrentTexture()
{
	return mAllClips[mState].mAllTextures[mCurrentFrame];
}

TextureId LCAnimator::GetCurrentTextureWithDirection(int direction)
{
	return mAllClips[mState].mAllTextures[(mCurrentFrame + 1) * (direction + 1) - 1];
}

void LCAnimator::Render(int currentDirection, const Vector2 & drawPos, float rotation, float alpha, float anchorX, float anchorY, float scaleX, float scaleY, const Camera2D & camera)
{
	auto& clip = mAllClips[mState];
	DrawSprite(clip.mAllTextures[mCurrentFrame], drawPos, rotation, alpha, anchorX, anchorY, scaleX * camera.GetScale(), scaleY * camera.GetScale(), clip.mAllJsonInfos[mCurrentFrame][currentDirection]);
}

void LCAnimator::AddClip(const LCAnimationClip && clip)
{
	if (clip.mTotalFrame > mMaxFrame)
	{
		mMaxFrame = clip.mTotalFrame;
	}
	mAllClips.push_back(clip);
	mClipCount++;
}

void LCAnimator::ChangeToState(int state, bool isWaitFinish)
{
	isWaitFinishAndChangeState = isWaitFinish;
	if (isWaitFinishAndChangeState)
		toState = state;
	else
	{
		mState = state;					//|
		mCurrentFrame = 0;				//|--- Write a Reset() for those line in future
		mFrameTimeCounter = mFrameTime;
	}
}

LCAnimationClip::LCAnimationClip(std::string textureNamePrefix, std::string textureNameSubfix, bool isLoop, int totalFrame, Unit & unit, int toClipWhenFinish)
	: mTotalFrame(totalFrame)
	, mIsLoop(isLoop)
	, mToClipWhenFinish(toClipWhenFinish)
{
	if (!mIsLoop)
	{
		ASSERT(mToClipWhenFinish != -1, "[Animation] Not looped animation must have some clip to go when finish.");
	}

	mAllTextures.reserve(totalFrame);
	mAllJsonInfos.reserve(totalFrame);

	for (int frameIndex = 0; frameIndex < mTotalFrame; ++frameIndex)
	{
		std::string textureName = textureNamePrefix + std::to_string(frameIndex);
		mAllTextures.push_back(LoadTexture(( textureName + textureNameSubfix).c_str()));
		mAllJsonInfos.emplace_back();
		LoadAtlasInfo(("../../Assets/Images/"+ textureName + ".json").c_str(), mAllJsonInfos.back(), unit.directionCount, textureName);
	}

}
