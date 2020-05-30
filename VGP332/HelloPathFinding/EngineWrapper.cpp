
//====================================================================================================
// Filename:	EngineWrapper.cpp
// Created by:	Mingzhuo Zhang
//====================================================================================================
#include "EngineWrapper.h"

EngineWrapper::TextureId EngineWrapper::LoadTexture(const char * filename)
{
	return X::LoadTexture(filename);
}

void EngineWrapper::DrawSprite(TextureId textureId, Vector2 position, float rotation, float alpha, float anchorX, float anchorY, float xScale, float yScale)
{
	X::DrawSprite(textureId, position, rotation, alpha, anchorX, anchorY, xScale, yScale);
}

float EngineWrapper::GetSpriteWidth(TextureId texture)
{
	return static_cast<float>(X::GetSpriteWidth(texture));
}

float EngineWrapper::GetSpriteHeight(TextureId texuture)
{
	return static_cast<float>(X::GetSpriteHeight(texuture));
}

void * EngineWrapper::GetSprite(TextureId texture)
{
	return X::GetSprite(texture);
}



void EngineWrapper::DrawScreenCircle(const Vector2 & vector, float radius, const Color & color)
{
	X::DrawScreenCircle(vector, radius, color);
}

void EngineWrapper::DrawScreenRect(const Rect & rect, const Color & color)
{
	X::DrawScreenRect(rect, color);
}

void EngineWrapper::DrawScreenLine(const Vector2 & pos1, const Vector2 & pos2, const Color & color)
{
	X::DrawScreenLine(pos1, pos2, color);
}

int EngineWrapper::GetMouseScreenX()
{
	return X::GetMouseScreenX();
}

int EngineWrapper::GetMouseScreenY()
{
	return X::GetMouseScreenY();
}

bool EngineWrapper::IsMousePressed(int mouseButton)
{
	return X::IsMousePressed(mouseButton);
}

bool EngineWrapper::IsMouseDown(int mouseButton)
{
	return X::IsMouseDown(mouseButton);
}

bool EngineWrapper::IsKeyPressed(int button)
{
	return X::IsKeyPressed(button);
}

bool EngineWrapper::PointInCircle(const Vector2 & mousePosition, const Circle & circle)
{
	return X::Math::PointInCircle(mousePosition, circle);
}

bool EngineWrapper::PointInRect(const Vector2 & mousePosition, const Rect & rect)
{
	return X::Math::PointInRect(mousePosition, rect);
}

bool EngineWrapper::OpenFileDialog(char fileName[MAX_PATH], const char * title, const char * filter)
{
	return X::OpenFileDialog(fileName, title, filter);
}

bool EngineWrapper::SaveFileDialog(char fileName[MAX_PATH], const char * title, const char * filter)
{
	return X::SaveFileDialog(fileName, title, filter);
}
