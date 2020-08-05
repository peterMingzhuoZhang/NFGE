#ifndef PTCAMERA_H
#define PTCAMERA_H

#include <NFGE/Inc/NFGE.h>
using namespace NFGE::Math;

class PTCamera
{
public:
	PTCamera();
	~PTCamera();

	void Load();
	void LoadScreen(const NFGE::Math::Vector2& size);
	void SetViewPos(Vector2 postion);
	void SetScale(float val) { mCurrentScale = val; }
	float GetScale() const { return mCurrentScale; }
	const Vector2& GetViewPosition() const { return mViewPostion; }

	Vector2 ConvertToWorld(Vector2 screenPos) const;
	Vector2 ConvertToScreen(Vector2 worldPos) const;

	void Render(NFGE::Graphics::TextureId &texture, Vector2 worldPos);
	void Render(NFGE::Graphics::TextureId &texture, Vector2 worldPos, float rotation, Vector2 archroPoint, NFGE::Math::Rect rect);
	void Render(NFGE::Graphics::TextureId &texture, Vector2 worldPos, float Scale, NFGE::Math::Rect rect);

	void RenderWithCurrentScale(NFGE::Graphics::TextureId &texture, Vector2 worldPos, float rotation, float alpha, float Scale, Vector2 archroPoint, NFGE::Math::Rect rect);

	void LowControlRender(NFGE::Graphics::TextureId &texture, Vector2 thePos, float rotation, float alpha, float Scale, Vector2 archroPoint, NFGE::Math::Rect rect);
	
	float mScreenWidth;
	float mScreenHeight;
private:
	Vector2 mViewPostion;
	float mCurrentScale = 1.0f;
};


#endif // !PTCAMERA_H
