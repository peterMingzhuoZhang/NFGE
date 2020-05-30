#ifndef LACELLCRAFT_GRIDMAP_BACKGROUND_HEADER
#define LACELLCRAFT_GRIDMAP_BACKGROUND_HEADER

#include "EngineWrapper.h"

using namespace EngineWrapper;

struct Background
{
	std::string textureName;
	TextureId texture;
	Vector2 textureSize;
	Vector2 worldSize;
	Vector2 renderScale;

	inline void Load(std::string textureName, const Vector2& worldSize);
	inline void Update(float deltaTime);
	inline void Rander(const Vector2& renderPos, const Camera2D& camera);
	inline void DebugUIUpdate(float deltaTime, const Camera2D& camera);
	inline void UnLoad();
};

// Function Implementation ----------------------------------------------------------------------------------------------------------------------------

inline void Background::Load(std::string theTextureName, const Vector2& theWorldSize)
{
	textureName = theTextureName;
	texture = LoadTexture(textureName.c_str());
	textureSize = Vector2{ GetSpriteWidth(texture), GetSpriteHeight(texture) };
	worldSize = theWorldSize;
	renderScale = Vector2{ worldSize.x / textureSize.x, worldSize.y / textureSize.y };

}

inline void Background::Update(float deltaTime)
{

}

inline void Background::Rander(const Vector2& renderPos, const Camera2D& camera)
{
	DrawSprite(texture, camera.ConvertToScreen(renderPos), 0.0f, 1.0f, 0.0f, 0.0f, renderScale.x * camera.GetScale(), renderScale.y * camera.GetScale());
}

inline void Background::DebugUIUpdate(float deltaTime, const Camera2D& camera)
{

}

inline void Background::UnLoad()
{

}

#endif // !LACELLCRAFT_GRIDMAP_BACKGROUND_HEADER

