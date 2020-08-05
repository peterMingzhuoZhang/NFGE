#ifndef INCLUDED_NFGE_GRAPHIC_DRAWCOMMAND_H
#define INCLUDED_NFGE_GRAPHIC_DRAWCOMMAND_H

#include "TextureManager.h"

namespace NFGE::Graphics
{
	//----------------------------------- Mingzhuo zhang Editing --------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------------------------------
	//------------------------------------|||||||||||||||||||||||||||----------------------------------------------------------------------
	//------------------------------------vvvvvvvvvvvvvvvvvvvvvvvvvvv----------------------------------------------------------------------
	enum class Pivot
	{
		TopLeft,
		Top,
		TopRight,
		Left,
		Center,
		Right,
		BottomLeft,
		Bottom,
		BottomRight
	};

	struct SpriteCommand
	{
		SpriteCommand(TextureId inTextureId, const Math::Vector2& inPosiiton, float inRotation)
			: textureId(inTextureId)
			, sourceRect({ 0.0f, 0.0f, 0.0f, 0.0f })
			, position(inPosiiton)
			, rotation(inRotation)
		{}

		SpriteCommand(TextureId inTextureId, const Math::Vector2& inPosiiton, float inRotation, Pivot pivot)
			: textureId(inTextureId)
			, sourceRect({ 0.0f, 0.0f, 0.0f, 0.0f })
			, position(inPosiiton)
			, rotation(inRotation)
			, pivot(pivot)
		{}

		SpriteCommand(TextureId inTextureId, const Math::Rect& inSourceRect, const Math::Vector2& inPosiiton, float inRotation)
			: textureId(inTextureId)
			, sourceRect(inSourceRect)
			, position(inPosiiton)
			, rotation(inRotation)
		{}

		// [Mingzhuo Edited]
		SpriteCommand(TextureId inTextureId, const Math::Vector2& inPosiiton, float inRotation, float alpha)
			: textureId(inTextureId)
			, sourceRect({ 0.0f, 0.0f, 0.0f, 0.0f })
			, position(inPosiiton)
			, rotation(inRotation)
			, alpha(alpha)
		{}

		// [Mingzhuo Edited]
		SpriteCommand(TextureId inTextureId, const Math::Vector2& inPosiiton, float inRotation, float alpha, float anchorX, float anchorY, float xScale, float yScale, NFGE::Math::Rect rect)
			: textureId(inTextureId)
			, sourceRect( rect )
			, position(inPosiiton)
			, rotation(inRotation)
			, alpha(alpha)
			, anchorX(anchorX)
			, anchorY(anchorY)
			, xScale(xScale)
			, yScale(yScale)
		{}

		TextureId textureId = 0;
		Math::Rect sourceRect{};
		Math::Vector2 position{ 0.0f };
		float rotation{ 0.0f };
		float alpha = 1.0f;				// [Mingzhuo Edited]
		float anchorX = -0.1f;			// Same with pivot [Mingzhuo Edited]
		float anchorY = -0.1f;			// Same with pivot
		float xScale = 1.0f;			// [Mingzhuo Edited]
		float yScale = 1.0f;			// [Mingzhuo Edited]
		Pivot pivot = Pivot::Center;
	};

	struct TextCommand
	{
		TextCommand(std::wstring inStr, float inSize, float inX, float inY, uint32_t inColor)
			: str(std::move(inStr))
			, size(inSize)
			, x(inX)
			, y(inY)
			, color(inColor)
		{}

		std::wstring str;
		float size, x, y;
		uint32_t color;
	};
}

#endif // !INCLUDED_NFGE_GRAPHIC_DRAWCOMMAND_H
