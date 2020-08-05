//====================================================================================================
// Filename:	EngineWrapper.h
// Created by:	Mingzhuo Zhang
// Description:	It has all the engine function that I use in this project. Which means you can basicly
//				hook up to any engine by changeing the implemtation of all those functions.
//				--- Right now is hook up with XEngine. ---
//====================================================================================================

#ifndef HELLOPERCEPTION_ENGINEWRAPPER_HEADER
#define HELLOPERCEPTION_ENGINEWRAPPER_HEADER

#include <NFGE/Inc/NFGE.h>
using namespace NFGE;
using namespace NFGE::Graphics;

namespace EngineWrapper
{
	using Vector2 = NFGE::Math::Vector2;
	using Vector3 = NFGE::Math::Vector3;
	using Color = NFGE::Graphics::Color;
	using Circle = NFGE::Math::Circle;
	using LineSegment = NFGE::Math::LineSegment;
	using TextureId = NFGE::Graphics::TextureId;
	using Rect = NFGE::Math::Rect;

	struct Triangle
	{
		union
		{
			struct { Vector2 p0, p1, p2; };
			std::array<Vector2, 3> v;
		};

		Triangle() :p0(0.0f, 0.0f), p1(0.0f, 0.0f), p2(0.0f, 0.0f){}
		Triangle(const Vector2& _p0, const Vector2& _p1, const Vector2& _p2) :p0(_p0), p1(_p1), p2(_p2) {}
		Triangle(const Triangle& other) : p0(other.p0), p1(other.p1), p2(other.p2) {}
	};

	bool ClosePointInTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& checkingPoint, Vector3& returnPoint, float& returnDistance, Vector3& farPoint);
	bool IsPointInTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& checkingPoint);
	bool ClosePointInTriangle_Custom_OnEdgeCount(const Vector3 & p0, const Vector3 & p1, const Vector3 & p2, const Vector3 & checkingPoint);
	bool IsPointInTriangle_Custom_OnEdgeCount(const Vector2 & p0, const Vector2 & p1, const Vector2 & p2, const Vector2 & checkingPoint);
	Rect GetMiniRect(const Triangle& tri);

	using Mouse = NFGE::Input::MouseButton;
	using Keys = NFGE::Input::KeyCode;

	TextureId LoadTexture(const char* filename);
	void DrawSprite(TextureId textureId, Vector2 position, float rotation, float alpha, float anchorX, float anchorY, float xScale, float yScale);
	float GetSpriteWidth(TextureId texture);
	float GetSpriteHeight(TextureId texuture);
	void* GetSprite(TextureId texture);

	int GetScreenWidth();
	int GetScreenHeight();

	void DrawScreenCircle(const Vector2& vector, float radius ,const Color& color);
	void DrawScreenRect(const Rect& rect, const Color& color);
	void DrawScreenLine(const Vector2& pos1, const Vector2& pos2, const Color& color);

	int GetMouseScreenX();
	int GetMouseScreenY();
	bool IsMousePressed(Mouse mouseButton);
	bool IsMouseDown(Mouse mouseButton);
	bool IsKeyPressed(Keys button);

	bool PointInCircle(const Vector2& mousePosition, const Circle& circle);
	bool PointInRect(const Vector2& mousePosition, const Rect& circle);
	bool Intersect(const LineSegment& line1, const LineSegment& line2);
	bool Intersect(const LineSegment& line1, const LineSegment& line2, Vector2& intersectPoint);
	bool customIntersect(const LineSegment& a, const LineSegment& b, bool& IsOverLap, bool& IsSameLine);

	float Cross2D(const Vector2& a, const Vector2& b);
	bool IsClockwise(const Vector2& a, const Vector2& b, const Vector2& c);

	bool OpenFileDialog(char fileName[MAX_PATH], const char* title, const char* filter);
	bool SaveFileDialog(char fileName[MAX_PATH], const char* title, const char* filter);

	class Camera2D
	{
		float screenWidth;
		float screenHeight;
		Vector2 position;
		float scale;
		float moveSpeed;
	public:
		Camera2D()
		{

		}

		Camera2D(float scrW, float scrH)
			:screenWidth(scrW)
			, screenHeight(scrH)
			, scale(1.0f)
			, moveSpeed(50.0f)
		{
			position = Vector2{0.0f, 0.0f };
		}

		Vector2 ConvertToWorld (Vector2 screenPos) const
		{
			return (screenPos / scale + position);
		}

		Vector2 ConvertToScreen(Vector2 worldPos) const
		{
			return (worldPos - position) * scale;
		}

		Rect ConvertToScreen(Rect worldRec) const
		{
			Vector2 min = ConvertToScreen(worldRec.min);
			Vector2 max = ConvertToScreen(worldRec.max);
			return Rect{min.x, min.y,max.x,max.y};
		}

		bool IsOutOfView(const Vector2& screenPosition, const Vector2& size) const
		{
			float halfWidth = size.x * 0.5f;
			float halfHeight = size.y * 0.5f;
			if (((screenPosition.x + halfWidth < 0.0f) || (screenPosition.x - halfWidth > screenWidth)) ||
				((screenPosition.y + halfHeight < 0.0f) || (screenPosition.y - halfHeight > screenHeight)))
			{
				return true;
			}
			return false;
		}

		void Update(float deltaTime)
		{
			if (NFGE::Input::InputSystem::Get()->IsKeyDown(Keys::W))
				position.y += moveSpeed * deltaTime;
			if (NFGE::Input::InputSystem::Get()->IsKeyDown(Keys::S))
				position.y -= moveSpeed * deltaTime;
			if (NFGE::Input::InputSystem::Get()->IsKeyDown(Keys::A))
				position.x -= moveSpeed * deltaTime;
			if (NFGE::Input::InputSystem::Get()->IsKeyDown(Keys::D))
				position.x += moveSpeed * deltaTime;
			if (NFGE::Input::InputSystem::Get()->IsKeyPressed(Keys::SPACE))// reset
			{
				scale = 1.0f;
				position = Vector2{ 0.0f,0.0f };
			}

			ImGui::SliderFloat("Scale", &scale, 0.2f, 5.0f);
			ImGui::SliderFloat("Camera speed", &moveSpeed, 50.0f, 500.0f);

		}

		float GetScale() const { return scale; }
		float SetScale(float value) { scale = value; }
	};
}



#endif // !HELLOPERCEPTION_ENGINEWRAPPER_HEADER
