#include "PTLabel.h"
#include "PTCamera.h"

PTLabel::PTLabel()
	: mText("")
	, mTextSize(0.0f)
	, mPosition({0.0f,0.0f})
	, mColor(NFGE::Graphics::Colors::Gray)
{
}

PTLabel::~PTLabel()
{
}

void PTLabel::Load(std::string theText, float theSize, NFGE::Math::Vector2 thePos, NFGE::Math::Vector4 theColor)
{
	mText = theText;
	mTextSize = theSize;
	mPosition = thePos;
	mColor = theColor;
}

void PTLabel::UnLoad()
{
}

void PTLabel::SetText(std::string theText)
{
	mText = theText;
}

std::string PTLabel::GetText() const
{
	return mText;
}

void PTLabel::SetPos(NFGE::Math::Vector2 thePos)
{
	mPosition = thePos;
}

NFGE::Math::Vector2 PTLabel::GetPos() const
{
	return mPosition;
}

void PTLabel::Render(const PTCamera& camera)
{
	Render(camera, mColor);
}

void PTLabel::Render(const PTCamera & camera, const NFGE::Graphics::Color & color)
{
	auto useTextSize = mTextSize / camera.GetScale();
	auto usePosition = camera.ConvertToScreen(mPosition);
	float textWidth = mText.length() * useTextSize * 0.5;
	float textHeight = useTextSize;
	//X::Math::Vector2 drawTextPosition = { mPosition.x - (textWidth * 0.5f),mPosition.y - (textHeight / 0.5f) };
	float drawPosX = usePosition.x - (textWidth * 0.5f);
	float drawPosY = usePosition.y - (textHeight * 0.5f);
	NFGE::sApp.DrawScreenText(mText.c_str(), drawPosX, drawPosY, useTextSize, color);
}

void PTLabel::Render(const PTCamera & camera, float alpha)
{
	auto renderColor = NFGE::Graphics::Color(mColor.x, mColor.y, mColor.z, alpha);
	Render(camera, renderColor);
}
