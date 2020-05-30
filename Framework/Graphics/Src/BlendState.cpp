#include "Precompiled.h"
#include "BlendState.h"
#include "D3DUtil.h"
#include "GraphicsSystem.h"

using namespace NFGE;
using namespace NFGE::Graphics;

namespace
{
	D3D11_BLEND GetSrcBlend(BlendState::Mode mode)
	{
		switch (mode)
		{
		case NFGE::Graphics::BlendState::Mode::Opaque:
			return D3D11_BLEND_ONE;
		case NFGE::Graphics::BlendState::Mode::AlphaBlend:
			return D3D11_BLEND_SRC_ALPHA;
		case NFGE::Graphics::BlendState::Mode::AlphaPremultiplied:
			return D3D11_BLEND_ONE;
		case NFGE::Graphics::BlendState::Mode::Additive:
			return D3D11_BLEND_SRC_ALPHA;
		}
		return D3D11_BLEND_ONE;
	}
	D3D11_BLEND GetDestBlend(BlendState::Mode mode)
	{
		switch (mode)
		{
		case NFGE::Graphics::BlendState::Mode::Opaque:
			return D3D11_BLEND_ZERO;
		case NFGE::Graphics::BlendState::Mode::AlphaBlend:
			return D3D11_BLEND_INV_SRC_ALPHA;
		case NFGE::Graphics::BlendState::Mode::AlphaPremultiplied:
			return D3D11_BLEND_INV_SRC_ALPHA;
		case NFGE::Graphics::BlendState::Mode::Additive:
			return D3D11_BLEND_ONE;
		}
		return D3D11_BLEND_ZERO;
	}
}

void NFGE::Graphics::BlendState::ClearState()
{
	Graphics::GetContext()->OMSetBlendState(nullptr, nullptr, UINT_MAX);
}

BlendState::~BlendState()
{
	ASSERT(mBlendState == nullptr, "[BlendState] Blend state not released!");
}

void BlendState::Initialize(Mode mode)
{
	D3D11_BLEND srcBlend = GetSrcBlend(mode);
	D3D11_BLEND destBlend = GetDestBlend(mode);

	D3D11_BLEND_DESC desc{};
	desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);
	desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
	desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
	desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr =  Graphics::GetDevice()->CreateBlendState(&desc, &mBlendState);
	ASSERT(SUCCEEDED(hr), "[BlendState] Failed to create Blend state.");
}

void BlendState::Terminate()
{
	SafeRelease(mBlendState);
}

void BlendState::Set()
{
	Graphics::GetContext()->OMSetBlendState(mBlendState, nullptr, UINT_MAX);
}


