#ifndef PTLABEL_H
#define PTLABEL_H

#include <NFGE/Inc/NFGE.h>
#include <string>

class PTCamera;

class PTLabel
{
public:
	PTLabel();
	~PTLabel();

	void Load(std::string theText, float theSize, NFGE::Math::Vector2 thePos, NFGE::Math::Vector4 theColor);
	void UnLoad();

	void SetText(std::string theText);
	std::string GetText() const;
	void SetPos(NFGE::Math::Vector2 thePos);
	NFGE::Math::Vector2 GetPos() const;

	void Render(const PTCamera& camera);
	void Render(const PTCamera& camera, const NFGE::Graphics::Color& color);
	void Render(const PTCamera& camera, float alpha);

private:
	std::string mText;
	float mTextSize;
	NFGE::Math::Vector2 mPosition;
	NFGE::Math::Vector4 mColor;
};

#endif // !PTLABEL_H
