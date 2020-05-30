#include "Precompiled.h"
#include "SpriteRenderer.h"

#include "D3DUtil.h"
#include "GraphicsSystem.h"
#include "Texture.h"
#include <DirectXTK/Inc/SpriteBatch.h>
#include <DirectXTK/Inc/CommonStates.h>

using namespace NFGE;
using namespace NFGE::Graphics;

namespace
{
	std::unique_ptr<SpriteRenderer> sSpriteRenderer = nullptr;

	namespace
	{
		DirectX::XMFLOAT2 GetOrigin(uint32_t width, uint32_t height, Pivot pivot)
		{
			auto index = static_cast<std::underlying_type_t<Pivot>>(pivot);
			constexpr DirectX::XMFLOAT2 offsets[] =
			{
				{ 0.0f, 0.0f }, // TopLeft
			{ 0.5f, 0.0f }, // Top
			{ 1.0f, 0.0f }, // TopRight
			{ 0.0f, 0.5f }, // Left
			{ 0.5f, 0.5f }, // Center
			{ 1.0f, 0.5f }, // Right
			{ 0.0f, 1.0f }, // BottomLeft
			{ 0.5f, 1.0f }, // Bottom
			{ 1.0f, 1.0f }, // BottomRight
			};
			return { width * offsets[index].x, height * offsets[index].y };
		}

		DirectX::XMFLOAT2 GetOrigin(uint32_t width, uint32_t height, const NFGE::Math::Vector2& pivot)
		{
			return { width * pivot.x, height * pivot.y };
		}

		DirectX::XMFLOAT2 ToXMFLOAT2(const Math::Vector2& v)
		{
			return { v.x, v.y };
		}
	}
}

void NFGE::Graphics::SpriteRenderer::StaticInitialize()
{
	ASSERT(sSpriteRenderer == nullptr, "[SpriteRenderer] System already initlizlized!");
	sSpriteRenderer = std::make_unique<SpriteRenderer>();
	sSpriteRenderer->Initialize();
}

void NFGE::Graphics::SpriteRenderer::StaticTerminate()
{
	if (sSpriteRenderer != nullptr)
	{
		sSpriteRenderer->Terminate();
		sSpriteRenderer.reset();
	}
}

SpriteRenderer * NFGE::Graphics::SpriteRenderer::Get()
{
	ASSERT(sSpriteRenderer != nullptr, "[SpriteRenderer] No instance registered.");
	return sSpriteRenderer.get();
}

NFGE::Graphics::SpriteRenderer::~SpriteRenderer()
{
	ASSERT(mSpriteBatch == nullptr, "[SpriteRenderer] Renderer not freed.");
}

void NFGE::Graphics::SpriteRenderer::Initialize()
{
	ASSERT(mSpriteBatch == nullptr, "[SpriteRenderer] already initlizlized!");
	ID3D11DeviceContext* context = Graphics::GetContext();
	ID3D11Device* device = Graphics::GetDevice();
	mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	mCommonStates = std::make_unique<DirectX::CommonStates>(device);
}

void NFGE::Graphics::SpriteRenderer::Terminate()
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] already terminate!");
	mSpriteBatch.reset();
}

void NFGE::Graphics::SpriteRenderer::BeginRender()
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initliazed.");
	mSpriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, mCommonStates->NonPremultiplied());
}

void NFGE::Graphics::SpriteRenderer::EndRender()
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initliazed.");
	mSpriteBatch->End();
}

//----------------------------------------------------------------------------------------------------
void NFGE::Graphics::SpriteRenderer::Draw(const Texture& texture, const Math::Vector2& pos, float rotation, Pivot pivot)
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	DirectX::XMFLOAT2 origin = GetOrigin(texture.GetWidth(), texture.GetHeight(), pivot);
	mSpriteBatch->Draw(GetShaderResourceView(texture), ToXMFLOAT2(pos), nullptr, DirectX::Colors::White, rotation, origin);
}

//----------------------------------------------------------------------------------------------------
void NFGE::Graphics::SpriteRenderer::Draw(const Texture& texture, const Math::Rect& sourceRect, const Math::Vector2& pos, float rotation, Pivot pivot)
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	RECT rect;
	rect.left = static_cast<LONG>(sourceRect.left);
	rect.top = static_cast<LONG>(sourceRect.top);
	rect.right = static_cast<LONG>(sourceRect.right);
	rect.bottom = static_cast<LONG>(sourceRect.bottom);
	DirectX::XMFLOAT2 origin = GetOrigin(rect.right - rect.left, rect.bottom - rect.top, pivot);
	mSpriteBatch->Draw(GetShaderResourceView(texture), ToXMFLOAT2(pos), &rect, DirectX::Colors::White, rotation, origin);
}

//----------------------------------- Mingzhuo zhang Editing --------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------|||||||||||||||||||||||||||----------------------------------------------------------------------
//------------------------------------vvvvvvvvvvvvvvvvvvvvvvvvvvv----------------------------------------------------------------------
//---------------------- Mingzhuo zhang added draw with alpha ---------------------------------------
void NFGE::Graphics::SpriteRenderer::Draw(const Texture & texture, const Math::Vector2 & pos, float rotation, float alpha)
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	DirectX::XMFLOAT2 origin{ texture.GetWidth() * 0.5f, texture.GetHeight() * 0.5f };
	mSpriteBatch->Draw(GetShaderResourceView(texture), *(DirectX::XMFLOAT2*)&pos, nullptr, DirectX::Colors::White * alpha, rotation, origin);
}

//---------------------- Mingzhuo zhang added draw with alpha & anchor point ---------------------
void NFGE::Graphics::SpriteRenderer::Draw(const Texture & texture, const Math::Vector2 & pos, float rotation, float alpha, float anchorX, float anchorY, float xScale, float yScale)
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	DirectX::XMFLOAT2 origin{ texture.GetWidth() * anchorX, texture.GetHeight() * anchorY };
	DirectX::XMFLOAT2 scale{ xScale, yScale };
	mSpriteBatch->Draw(GetShaderResourceView(texture), *(DirectX::XMFLOAT2*)&pos, nullptr, DirectX::Colors::White * alpha, rotation, origin, scale);
}

//---------------------- Mingzhuo zhang added draw with alpha & anchor point & Source rect ---------------------
void NFGE::Graphics::SpriteRenderer::Draw(const Texture & texture, const Math::Rect& sourceRect, const Math::Vector2 & pos, float rotation, float alpha, float anchorX, float anchorY, float xScale, float yScale)
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	//DirectX::XMFLOAT2 origin{ texture.GetWidth() * anchorX, texture.GetHeight() * anchorY };
	DirectX::XMFLOAT2 scale{ xScale, yScale };
	RECT rect;
	rect.left = static_cast<LONG>(sourceRect.left);
	rect.top = static_cast<LONG>(sourceRect.top);
	rect.right = static_cast<LONG>(sourceRect.right);
	rect.bottom = static_cast<LONG>(sourceRect.bottom);
	DirectX::XMFLOAT2 origin = GetOrigin(rect.right - rect.left, rect.bottom - rect.top, { anchorX ,anchorY});
	mSpriteBatch->Draw(GetShaderResourceView(texture), *(DirectX::XMFLOAT2*)&pos, &rect, DirectX::Colors::White * alpha, rotation, origin, scale);
}
//------------------------------------^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^--------------------------------------------------------------
//------------------------------------|||||||||||||||||||||||||||||||||||--------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------- End Edite -------------------------------------------------------------------------------------