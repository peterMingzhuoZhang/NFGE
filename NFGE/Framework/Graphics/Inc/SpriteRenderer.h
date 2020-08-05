#ifndef INCLUDED_NFGE_GRAPHICS_SPRITERENDER_H
#define INCLUDED_NFGE_GRAPHICS_SPRITERENDER_H

#include "DrawCommand.h"

namespace DirectX { class SpriteBatch; class CommonStates; }

namespace NFGE {
namespace Graphics {
	class Texture;

	class SpriteRenderer
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static SpriteRenderer* Get();
	public:
		SpriteRenderer() = default;
		~SpriteRenderer();

		SpriteRenderer(const SpriteRenderer&) = delete;
		SpriteRenderer& operator=(const SpriteRenderer&) = delete;

		void Initialize();
		void Terminate();

		void BeginRender();
		void EndRender();

		void Draw(const Texture& texture, const Math::Vector2& pos, float rotation = 0.0f, Pivot pivot = Pivot::Center);
		void Draw(const Texture& texture, const Math::Rect& sourceRect, const Math::Vector2& pos, float rotation = 0.0f, Pivot pivot = Pivot::Center);

		//----------------------------------- Mingzhuo zhang Editing --------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------------------------------------
		//------------------------------------|||||||||||||||||||||||||||----------------------------------------------------------------------
		//------------------------------------vvvvvvvvvvvvvvvvvvvvvvvvvvv----------------------------------------------------------------------
		void Draw(const Texture& texture, const Math::Vector2& pos, float rotation, float alpha);
		void Draw(const Texture & texture, const Math::Vector2 & pos, float rotation, float alpha, float anchorX, float anchorY, float xScale, float yScale);
		void Draw(const Texture & texture, const Math::Rect& sourceRect, const Math::Vector2 & pos, float rotation, float alpha, float anchorX, float anchorY, float xScale, float yScale);
		//------------------------------------^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^--------------------------------------------------------------
		//------------------------------------|||||||||||||||||||||||||||||||||||--------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------------------------------------
		//------------------------------------- End Edite -------------------------------------------------------------------------------------
	private:
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch = nullptr;
		std::unique_ptr<DirectX::CommonStates> mCommonStates = nullptr;
	};
	
} // namespace NFGE
} // namespace Graphics



#endif // !INCLUDED_NFGE_GRAPHICS_SPRITERENDER_H
