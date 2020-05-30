#include "Precompiled.h"
#include "DepthStencilState.h"
#include "D3DUtil.h"

using namespace NFGE;
using namespace NFGE::Graphics;

namespace
{
	inline bool GetEnable(DepthStencilState::StateMode stateMode)
	{
		switch (stateMode)
		{
		case NFGE::Graphics::DepthStencilState::StateMode::DepthDefault:
			return true;
			break;
		case NFGE::Graphics::DepthStencilState::StateMode::DepthNone:
			return false;
			break;
		case NFGE::Graphics::DepthStencilState::StateMode::DepthRead:
			return true;
			break;
		}
		return false;
	}
	inline D3D11_DEPTH_WRITE_MASK GetWriteMask(DepthStencilState::StateMode stateMode)
	{
		switch (stateMode)
		{
		case NFGE::Graphics::DepthStencilState::StateMode::DepthDefault:
			return D3D11_DEPTH_WRITE_MASK_ALL;
			break;
		case NFGE::Graphics::DepthStencilState::StateMode::DepthNone:
			return D3D11_DEPTH_WRITE_MASK_ZERO;
			break;
		case NFGE::Graphics::DepthStencilState::StateMode::DepthRead:
			return D3D11_DEPTH_WRITE_MASK_ALL;
			break;
		default:
			return D3D11_DEPTH_WRITE_MASK_ALL;
			break;
		}
	}
}

NFGE::Graphics::DepthStencilState::~DepthStencilState()
{
	ASSERT(mDepthStencilState == nullptr, "[DepthStencilState ] Rasterizer state not released!");
}

void NFGE::Graphics::DepthStencilState::Initialize(StateMode stateMode)
{
	D3D11_DEPTH_STENCIL_DESC desc = {};

	desc.DepthEnable = GetEnable(stateMode);
	desc.DepthWriteMask = GetWriteMask(stateMode);
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	desc.StencilEnable = FALSE;
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	desc.BackFace = desc.FrontFace;

	auto device = Graphics::GetDevice();
	HRESULT hr = device->CreateDepthStencilState(&desc, &mDepthStencilState);

	ASSERT(SUCCEEDED(hr),"[DepthStencilState] Failed to create depth stencil state");
}

void NFGE::Graphics::DepthStencilState::Terminate()
{
	SafeRelease(mDepthStencilState);
}

void NFGE::Graphics::DepthStencilState::Set()
{
	Graphics::GetContext()->OMSetDepthStencilState(mDepthStencilState, 0);
}

void NFGE::Graphics::DepthStencilState::Clear()
{
	Graphics::GetContext()->OMSetDepthStencilState(nullptr, 0);
}
