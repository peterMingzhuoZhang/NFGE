#include "PTCamera.h"


PTCamera::PTCamera()
	:mViewPostion({0.0f,0.0f})
{
	
}

PTCamera::~PTCamera()
{
}



void PTCamera::Load()
{
	mScreenWidth = static_cast<float>(NFGE::sApp.GetScreenWidth());
	mScreenHeight = static_cast<float>(NFGE::sApp.GetScreenHeight());
	mViewPostion = NFGE::Math::Vector2{ mScreenWidth * 0.5f, mScreenHeight * 0.5f };
}

void PTCamera::LoadScreen(const NFGE::Math::Vector2 & size)
{
	mScreenWidth = size.x;
	mScreenHeight = size.y;
}

void PTCamera::SetViewPos(Vector2 postion)
{
	mViewPostion = postion * mCurrentScale;
}

Vector2 PTCamera::ConvertToWorld(Vector2 screenPos) const 
{
	Vector2 worldPos = screenPos - Vector2({ mScreenWidth * 0.5f, mScreenHeight * 0.5f});
	worldPos += mViewPostion;
	return worldPos;
}

Vector2 PTCamera::ConvertToScreen(Vector2 worldPos) const
{
	NFGE::Math::Vector2 scaledWorldPos = worldPos * mCurrentScale;
	Vector2 screenPos = scaledWorldPos - mViewPostion;
	screenPos += Vector2({ mScreenWidth *0.5f, mScreenHeight * 0.5f });
	return screenPos;
}

void PTCamera::Render(NFGE::Graphics::TextureId &texture, Vector2 worldPos)
{
	Vector2 screenPos = ConvertToScreen(worldPos);
	if ((screenPos.x >-32.0f)&&((screenPos.x < mScreenWidth + 32.0f) && (screenPos.y >-32.0f)&&(screenPos.y < mScreenHeight + 32.0f)))
	{
		NFGE::sApp.DrawSprite(texture, screenPos);
	}
	
}

void PTCamera::Render(NFGE::Graphics::TextureId & texture, Vector2 worldPos, float rotation, Vector2 archroPoint, NFGE::Math::Rect rect)
{
	Vector2 screenPos = ConvertToScreen(worldPos);
	if ((screenPos.x >-32.0f) && ((screenPos.x < mScreenWidth + 32.0f) && (screenPos.y >-32.0f) && (screenPos.y < mScreenHeight + 32.0f)))
	{
		NFGE::sApp.DrawSprite(texture, screenPos, rotation, 1.0f, archroPoint.x, archroPoint.y, 1.0f, 1.0f, rect);
	} 
}

void PTCamera::Render(NFGE::Graphics::TextureId & texture, Vector2 worldPos, float scale, NFGE::Math::Rect rect)
{
	Vector2 screenPos = ConvertToScreen(worldPos);
	if ((screenPos.x >-32.0f) && ((screenPos.x < mScreenWidth + 32.0f) && (screenPos.y >-32.0f) && (screenPos.y < mScreenHeight + 32.0f)))
	{
		NFGE::sApp.DrawSprite(texture, screenPos, 0.0f, 1.0f, 0.5f, 0.5f, scale, scale, rect);
	}
}

void PTCamera::RenderWithCurrentScale(NFGE::Graphics::TextureId & texture, Vector2 worldPos, float rotation, float alpha, float Scale, Vector2 archroPoint, NFGE::Math::Rect rect)
{
	worldPos * mCurrentScale;
	Vector2 screenPos = ConvertToScreen(worldPos);
	if ((screenPos.x >-32.0f) && ((screenPos.x < mScreenWidth + 32.0f) && (screenPos.y >-32.0f) && (screenPos.y < mScreenHeight + 32.0f)))
	{
		NFGE::sApp.DrawSprite(texture, screenPos, rotation, Scale, archroPoint.x, archroPoint.y, Scale * mCurrentScale, Scale * mCurrentScale, rect);
	}
}

void PTCamera::LowControlRender(NFGE::Graphics::TextureId & texture, Vector2 thePos, float rotation, float alpha, float Scale, Vector2 archroPoint, NFGE::Math::Rect rect)
{
	if ((thePos.x >-32.0f) && ((thePos.x < mScreenWidth + 32.0f) && (thePos.y >-32.0f) && (thePos.y < mScreenHeight + 32.0f)))
	{
		NFGE::sApp.DrawSprite(texture, thePos, rotation, Scale, archroPoint.x, archroPoint.y, Scale * mCurrentScale, Scale * mCurrentScale, rect);
	}
}
