#include "PTButton.h"
#include "PTCamera.h"
#include "GameState.h"

namespace
{
	const int buttonBoraderLength = 2;
	const float disappearTime = 0.5f;
	const float effectDensity = 5.0f;
}

PTButton::PTButton()
	: mTextLabel()
	, mTextureId(0)
	, mPosition({ 0.0f,0.0f })
	, mWidth(0.0f)
	, mHeight(0.0f)
	, mColor(NFGE::Graphics::Colors::Pink)
	, mIsOnTop(false)
{
}

PTButton::~PTButton()
{
}

void PTButton::Load(NFGE::Math::Vector2 thePosition, NFGE::Graphics::TextureId theTextureId, std::string theText, float theSize, float theWidth, float theHeight, NFGE::Graphics::Color textColor, NFGE::Graphics::Color boardColor)
{
	
	mPosition = thePosition;
	mTextureId = theTextureId;
	mWidth = theWidth;
	mHeight = theHeight;

	NFGE::Math::Vector2 textLabelPos = { mPosition.x + (mWidth * 0.5f), mPosition.y + (mHeight* 0.5f) };
	mTextLabel.Load(theText, theSize, textLabelPos, textColor);
	mColor = boardColor;
}

void PTButton::Unload()
{
}

void PTButton::Update(GameState* theGame, NFGE::Math::Vector2 mousePosition, float deltaTime)
{
	if (mIsDisable)
		return;
	if (mIsOnTop == true)
	{
		mDisapperaTimer += deltaTime;
	}
	else
	{
		auto gabbyPosition = theGame->mGabby.position;
		if (((gabbyPosition.x > mPosition.x) && (gabbyPosition.x < (mPosition.x + mWidth))) &&
			(gabbyPosition.y >mPosition.y) && (gabbyPosition.y < (mPosition.y + mHeight)))
		{
			//TODO switch state
			//Spwan effect
			auto renderPosition = theGame->mCamera.ConvertToScreen(mPosition);
			auto renderScale = theGame->mCamera.GetScale();

			float left = renderPosition.x;
			float top = renderPosition.y;
			float right = renderPosition.x + mWidth / renderScale;
			float bottom = renderPosition.y + mHeight / renderScale;

			for (size_t i = 0; i < (int)((right - left) / effectDensity); i++)
			{
				Boom({ left + i * effectDensity,top }, theGame);
				Boom({ left + i * effectDensity,bottom }, theGame);
			}
			for (size_t i = 0; i < (int)((bottom - top) / effectDensity); i++)
			{
				Boom({ left ,top + i * effectDensity }, theGame);
				Boom({ right ,top + i * effectDensity }, theGame);
			}

			mIsOnTop = true;
		}
	}
	if (mDisapperaTimer > disappearTime)
	{
		mIsDisable = true;
	}
}

void PTButton::Render(const PTCamera& camera)
{
	//X::Math::Vector2 drawRectPosition = { mPosition.x - (mWidth * 0.5f),mPosition.y - (mHeight * 0.5f) };
	if (mIsDisable)
		return;
	float alpha = (disappearTime - mDisapperaTimer) / disappearTime;
	if (alpha > 1.0f) alpha = 1.0f;
	if (alpha < 0.0f) alpha = 0.0f;

	auto renderColor = NFGE::Graphics::Color{ mColor.x,mColor.y, mColor.z, alpha};
	auto renderPosition = camera.ConvertToScreen(mPosition);
	auto renderScale = camera.GetScale();
	
	float left = renderPosition.x;
	float top  = renderPosition.y;
	float right = renderPosition.x + mWidth / renderScale;
	float bottom = renderPosition.y + mHeight / renderScale;
	
	NFGE::sApp.DrawScreenRect(left,top,right,bottom, renderColor);
	for (int i = 1; i < buttonBoraderLength; ++i)
	{
		++left;
		++top;
		--right;
		--bottom;
		NFGE::sApp.DrawScreenRect(left, top, right, bottom, renderColor);
	}

	mTextLabel.Render(camera, renderColor);
}

void PTButton::Boom(const NFGE::Math::Vector2 & pos, GameState * theGame)
{
	for (size_t i = 0; i < 10; i++)
	{
		auto vel = NFGE::Math::RandomUnitSphere() * 1.5f;

		auto particle = new NFGE::Physics::Particle();
		particle->SetPosition({ pos.x,pos.y,0.0f });
		particle->SetVelocity(vel);
		particle->radius = 0.05f;
		particle->invMass = 1.0f;
		particle->gravityScale = 0.0f;
		theGame->mPhysicsWorld.AddParticle(particle);
		theGame->particleLifeTimeCounter.push_back(0.0f);
		theGame->mParticles.push_back(particle);
	}
}
