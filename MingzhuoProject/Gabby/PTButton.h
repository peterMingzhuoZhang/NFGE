#ifndef PTBUTTON
#define PTBUTTON

#include "PTLabel.h"

class PTCamera;
class GameState;
class PTButton
{
public:
	PTButton();
	~PTButton();

	void Load(NFGE::Math::Vector2 thePosition, NFGE::Graphics::TextureId theTextureId, std::string theText, float theSize, float theWidth, float theHeight, NFGE::Graphics::Color textColor, NFGE::Graphics::Color boardColor);
	void Unload();

	void Update(GameState* theGame, NFGE::Math::Vector2 mousePosition,float deltaTime);
	void Render(const PTCamera& camera);

	bool IsSelecting() const { return mIsOnTop; }

	void Reset()
	{
		mIsOnTop = false;
		mDisapperaTimer = 0.0f;
		mIsDisable = false;
	}
private:

	void Boom(const NFGE::Math::Vector2& pos, GameState* theGame);
	

	PTLabel mTextLabel;
	//std::string mText;
	NFGE::Graphics::TextureId mTextureId;
	//float mTextSize;
	NFGE::Math::Vector2 mPosition;
	float mWidth;
	float mHeight;
	NFGE::Math::Vector4 mColor;

	bool mIsOnTop = false;
	bool mIsDisable = false;

	float mDisapperaTimer = 0.0f;
};

#endif // !PTBUTTON
