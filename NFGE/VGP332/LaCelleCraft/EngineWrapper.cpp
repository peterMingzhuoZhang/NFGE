#include "EngineWrapper.h"

EngineWrapper::TextureId EngineWrapper::LoadTexture(const char * filename)
{
	return NFGE::Graphics::TextureManager::Get()->LoadTexture(filename);
}

void EngineWrapper::DrawSprite(TextureId textureId, Vector2 position, float rotation, float alpha, float anchorX, float anchorY, float xScale, float yScale, NFGE::Math::Rect sourceRect)
{
	NFGE::sApp.DrawSprite(textureId, position, rotation, alpha, anchorX, anchorY, xScale, yScale, sourceRect);
}

float EngineWrapper::GetSpriteWidth(TextureId texture)
{
	return static_cast<float>(NFGE::sApp.GetSpriteWidth(texture));
}

float EngineWrapper::GetSpriteHeight(TextureId texuture)
{
	return static_cast<float>(NFGE::sApp.GetSpriteHeight(texuture));
}

void * EngineWrapper::GetSprite(TextureId texture)
{
	return NFGE::sApp.GetSprite(texture);
}

int EngineWrapper::GetScreenWidth()
{
	return NFGE::sApp.GetScreenWidth();
}

int EngineWrapper::GetScreenHeight()
{
	return NFGE::sApp.GetScreenHeight();
}



void EngineWrapper::DrawScreenCircle(const Vector2 & vector, float radius, const NFGE::Graphics::Color & color)
{
	NFGE::sApp.DrawScreenCircle(vector, radius, color);
}

void EngineWrapper::DrawScreenRect(const Rect & rect, const NFGE::Graphics::Color & color)
{
	NFGE::sApp.DrawScreenRect(rect, color);
}

void EngineWrapper::DrawScreenLine(const Vector2 & pos1, const Vector2 & pos2, const NFGE::Graphics::Color & color)
{
	NFGE::sApp.DrawScreenLine(pos1, pos2, color);
}

int EngineWrapper::GetMouseScreenX()
{
	return NFGE::Input::InputSystem::Get()->GetMouseScreenX();
}

int EngineWrapper::GetMouseScreenY()
{
	return NFGE::Input::InputSystem::Get()->GetMouseScreenY();
}

bool EngineWrapper::IsMousePressed(NFGE::Input::MouseButton mouseButton)
{
	return NFGE::Input::InputSystem::Get()->IsMousePressed(mouseButton);
}

bool EngineWrapper::IsMouseDown(NFGE::Input::MouseButton mouseButton)
{
	return NFGE::Input::InputSystem::Get()->IsMouseDown(mouseButton);
}

bool EngineWrapper::IsKeyPressed(NFGE::Input::KeyCode button)
{
	return NFGE::Input::InputSystem::Get()->IsKeyPressed(button);
}

bool EngineWrapper::PointInCircle(const Vector2 & mousePosition, const Circle & circle)
{
	return NFGE::Math::PointInCircle(mousePosition, circle);
}

bool EngineWrapper::PointInRect(const Vector2 & mousePosition, const Rect & rect)
{
	return NFGE::Math::PointInRect(mousePosition, rect);
}

bool EngineWrapper::Intersect(const LineSegment & line1, const LineSegment & line2)
{
	return NFGE::Math::Intersect(line1, line2);
}

EngineWrapper::Vector2 EngineWrapper::RandomUnitCircle()
{
	return NFGE::Math::RandomUnitCircle();
}

float EngineWrapper::RandomFloat(float min, float max)
{
	return NFGE::Math::RandomFloat(min, max);
}

float EngineWrapper::DistanceSqr(const Vector2 & p1, const Vector2 & p2)
{
	return NFGE::Math::DistanceSqr(p1, p2);
}

float EngineWrapper::Distance(const Vector2 & p1, const Vector2 & p2)
{
	return NFGE::Math::Distance(p1,p2);
}

EngineWrapper::Vector2 EngineWrapper::Normalize(const Vector2& dir)
{
	return NFGE::Math::Normalize(dir);
}

bool EngineWrapper::OpenFileDialog(char fileName[MAX_PATH], const char * title, const char * filter)
{
	return NFGE::sApp.OpenFileDialog(fileName, title, filter);
}

bool EngineWrapper::SaveFileDialog(char fileName[MAX_PATH], const char * title, const char * filter)
{
	return NFGE::sApp.SaveFileDialog(fileName, title, filter);
}
