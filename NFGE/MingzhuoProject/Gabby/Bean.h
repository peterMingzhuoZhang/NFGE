#pragma once

#include "NFGE/Inc/NFGE.h"

class TriangleDude;
class PTCamera;
class GameState;

class Bean
{
public: 
	void Load(const NFGE::Math::Vector2& position, float worldWidth, float worldHeight, const NFGE::Graphics::Color& color);
	void Load(float worldWidth, float worldHeight, const NFGE::Graphics::Color& color);

	void Update(GameState* theGame, const TriangleDude& Gabby, float deltaTime);

	void Render(const PTCamera& camera);

private:
	void RandomGenreate();
	void Boom(const NFGE::Math::Vector2& pos, GameState* theGame);

	NFGE::Math::Circle mCircle;
	NFGE::Graphics::Color mColor;
	NFGE::Math::Rect mWorldRect;

	float mRadiusChangeSpeed = 0.0f;
};