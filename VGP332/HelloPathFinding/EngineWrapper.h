//====================================================================================================
// Filename:	EngineWrapper.h
// Created by:	Mingzhuo Zhang
// Description:	It has all the engine function that I use in this project. Which means you can basicly
//				hook up to any engine by changeing the implemtation of all those functions.
//				--- Right now is hook up with XEngine. ---
//====================================================================================================

#ifndef HELLOPATHFINDING_ENGINEWRAPPER_HEADER
#define HELLOPATHFINDING_ENGINEWRAPPER_HEADER

#include <AI/Inc/AI.h>
#include <NFGE/Inc/NFGE.h>

using namespace NFGE;
namespace EngineWrapper
{
	using Vector2 = NFGE::Math::Vector2;
	using Color = NFGE::Graphics::Color;
	using Circle = NFGE::Math::Circle;
	using TextureId = NFGE::Graphics::TextureId;
	using Rect = NFGE::Math::Rect;

	using namespace NFGE::Input;

	TextureId LoadTexture(const char* filename);
	void DrawSprite(TextureId textureId, Vector2 position, float rotation, float alpha, float anchorX, float anchorY, float xScale, float yScale);
	float GetSpriteWidth(TextureId texture);
	float GetSpriteHeight(TextureId texuture);
	void* GetSprite(TextureId texture);

	void DrawScreenCircle(const Vector2& vector, float radius ,const Color& color);
	void DrawScreenRect(const Rect& rect, const Color& color);
	void DrawScreenLine(const Vector2& pos1, const Vector2& pos2, const Color& color);

	int GetMouseScreenX();
	int GetMouseScreenY();
	bool IsMousePressed(int mouseButton);
	bool IsMouseDown(int mouseButton);
	bool IsKeyPressed(int button);

	bool PointInCircle(const Vector2& mousePosition, const Circle& circle);
	bool PointInRect(const Vector2& mousePosition, const Rect& circle);

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
			if (NFGE::Input::InputSystem::Get()->IsKeyDown(KeyCode::W))
				position.y += moveSpeed * deltaTime;
			if (NFGE::Input::InputSystem::Get()->IsKeyDown(KeyCode::S))
				position.y -= moveSpeed * deltaTime;
			if (NFGE::Input::InputSystem::Get()->IsKeyDown(KeyCode::A))
				position.x -= moveSpeed * deltaTime;
			if (NFGE::Input::InputSystem::Get()->IsKeyDown(KeyCode::D))
				position.x += moveSpeed * deltaTime;
			if (NFGE::Input::InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))// reset
			{
				scale = 1.0f;
				position = Vector2{ 0.0f,0.0f };
			}

			//ImGui::SliderFloat("Scale", &scale, 0.2f, 5.0f);
			//ImGui::SliderFloat("Camera speed", &moveSpeed, 50.0f, 500.0f);

		}

		float GetScale() const { return scale; }
		float SetScale(float value) { scale = value; }
	};
}



#endif // !HELLOPATHFINDING_ENGINEWRAPPER_HEADER
