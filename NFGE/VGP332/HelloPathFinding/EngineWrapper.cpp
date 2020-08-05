
//====================================================================================================
// Filename:	EngineWrapper.cpp
// Created by:	Mingzhuo Zhang
//====================================================================================================
#include "EngineWrapper.h"

EngineWrapper::TextureId EngineWrapper::LoadTexture(const char * filename)
{
	return NFGE::Graphics::TextureManager::Get()->LoadTexture(filename);
}

void EngineWrapper::DrawSprite(TextureId textureId, Vector2 position, float rotation, float alpha, float anchorX, float anchorY, float xScale, float yScale)
{
	NFGE::sApp.DrawSprite(textureId, position, rotation, alpha, anchorX, anchorY, xScale, yScale);
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



void EngineWrapper::DrawScreenCircle(const Vector2 & vector, float radius, const Color & color)
{
	NFGE::sApp.DrawScreenCircle(vector, radius, color);
}

void EngineWrapper::DrawScreenRect(const Rect & rect, const Color & color)
{
	NFGE::sApp.DrawScreenRect(rect, color);
}

void EngineWrapper::DrawScreenLine(const Vector2 & pos1, const Vector2 & pos2, const Color & color)
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

bool EngineWrapper::IsMousePressed(int mouseButton)
{
	return NFGE::Input::InputSystem::Get()->IsMousePressed(static_cast<NFGE::Input::MouseButton>(mouseButton));
}

bool EngineWrapper::IsMouseDown(int mouseButton)
{
	return NFGE::Input::InputSystem::Get()->IsMouseDown(static_cast<NFGE::Input::MouseButton>(mouseButton));
}

bool EngineWrapper::IsKeyPressed(int button)
{
	return NFGE::Input::InputSystem::Get()->IsKeyPressed(static_cast<NFGE::Input::KeyCode>(button));
}

bool EngineWrapper::PointInCircle(const Vector2 & mousePosition, const Circle & circle)
{
	return NFGE::Math::PointInCircle(mousePosition, circle);
}

bool EngineWrapper::PointInRect(const Vector2 & mousePosition, const Rect & rect)
{
	return NFGE::Math::PointInRect(mousePosition, rect);
}

bool EngineWrapper::OpenFileDialog(char fileName[MAX_PATH], const char * title, const char * filter)
{
	return NFGE::sApp.OpenFileDialog(fileName, title, filter);
}

bool EngineWrapper::SaveFileDialog(char fileName[MAX_PATH], const char * title, const char * filter)
{
	return NFGE::sApp.SaveFileDialog(fileName, title, filter);
}
