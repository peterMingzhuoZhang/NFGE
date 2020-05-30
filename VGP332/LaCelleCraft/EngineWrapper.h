#ifndef LACELLECRAFT_ENGINEWRAPPER_HEADER
#define LACELLECRAFT_ENGINEWRAPPER_HEADER

#include <NFGE/Inc/NFGE.h>

#define RTTI(ClassName)\
static std::string GetTypeName() { return #ClassName; }\
static void* CreateInstance() { return new ClassName(); }

namespace EngineWrapper
{
	
	using Vector2 = NFGE::Math::Vector2;
	using Circle = NFGE::Math::Circle;
	using LineSegment = NFGE::Math::LineSegment;
	using TextureId = NFGE::Graphics::TextureId;
	using Rect = NFGE::Math::Rect;
	const float kTwoPi = NFGE::Math::Constants::TwoPi;

	using namespace NFGE::Input;
	using namespace NFGE::Graphics::Colors;

	TextureId LoadTexture(const char* filename);
	void DrawSprite(TextureId textureId, Vector2 position, float rotation, float alpha, float anchorX, float anchorY, float xScale, float yScale, NFGE::Math::Rect sourceRect = NFGE::Math::Rect{});
	float GetSpriteWidth(TextureId texture);
	float GetSpriteHeight(TextureId texuture);
	void* GetSprite(TextureId texture);

	int GetScreenWidth();
	int GetScreenHeight();

	void DrawScreenCircle(const Vector2& vector, float radius ,const NFGE::Graphics::Color& color);
	void DrawScreenRect(const Rect& rect, const NFGE::Graphics::Color& color);
	void DrawScreenLine(const Vector2& pos1, const Vector2& pos2, const NFGE::Graphics::Color& color);

	int GetMouseScreenX();
	int GetMouseScreenY();
	bool IsMousePressed(MouseButton mouseButton);
	bool IsMouseDown(MouseButton mouseButton);
	bool IsKeyPressed(NFGE::Input::KeyCode button);

	bool PointInCircle(const Vector2& mousePosition, const Circle& circle);
	bool PointInRect(const Vector2& mousePosition, const Rect& circle);

	bool Intersect(const LineSegment& line1, const LineSegment& line2);

	Vector2 RandomUnitCircle();
	float RandomFloat(float min, float max);
	float DistanceSqr(const Vector2& p1, const Vector2& p2);
	float Distance(const Vector2& p1, const Vector2& p2);
	Vector2 Normalize(const Vector2& dir);

	bool OpenFileDialog(char fileName[MAX_PATH], const char* title, const char* filter);
	bool SaveFileDialog(char fileName[MAX_PATH], const char* title, const char* filter);

	class Camera2D
	{
		float screenWidth;
		float screenHeight;
		Vector2 position;
		float scale;
		float moveSpeed;
		float scaleSpeed;
	public:
		Camera2D()
		{

		}

		Camera2D(float scrW, float scrH)
			:screenWidth(scrW)
			, screenHeight(scrH)
			, scale(1.0f)
			, moveSpeed(300.0f)
			, scaleSpeed(0.5f)
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
			auto inputSystem = NFGE::Input::InputSystem::Get();
			if (inputSystem->IsKeyDown(KeyCode::W) || inputSystem->IsMouseTopEdge())
				position.y -= moveSpeed * deltaTime;
			if (inputSystem->IsKeyDown(KeyCode::S) || inputSystem->IsMouseBottomEdge())
				position.y += moveSpeed * deltaTime;
			if (inputSystem->IsKeyDown(KeyCode::A) || inputSystem->IsMouseLeftEdge())
				position.x -= moveSpeed * deltaTime;
			if (inputSystem->IsKeyDown(KeyCode::D) || inputSystem->IsMouseRightEdge())
				position.x += moveSpeed * deltaTime;
			if (inputSystem->IsKeyDown(KeyCode::Q))
				scale = NFGE::Math::Max(scale - scaleSpeed * deltaTime, 0.2f);
			if (inputSystem->IsKeyDown(KeyCode::E))
				scale = NFGE::Math::Min(scale + scaleSpeed * deltaTime, 5.0f);
			if (inputSystem->IsKeyPressed(KeyCode::SPACE))// reset
			{
				scale = 1.0f;
				position = Vector2{ 0.0f,0.0f };
			}
		}

		void DebugUIUpdate(float deltaTime)
		{
			ImGui::SliderFloat("Scale", &scale, 0.2f, 5.0f);
			ImGui::SliderFloat("Camera speed", &moveSpeed, 50.0f, 500.0f);
			ImGui::SliderFloat("Camera scale speed", &scaleSpeed, 0.1f, 2.0f);
		}

		float GetScale() const { return scale; }
		float SetScale(float value) { scale = value; }
	};
}



#endif // !HELLOPATHFINDING_ENGINEWRAPPER_HEADER
