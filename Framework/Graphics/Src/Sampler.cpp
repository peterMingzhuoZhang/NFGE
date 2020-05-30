#include "Precompiled.h"
#include "Sampler.h"

#include "D3DUtil.h"

using namespace NFGE;
using namespace NFGE::Graphics;

D3D11_FILTER GetFilter(Sampler::Filter filter)
{
	switch (filter)
	{
	case NFGE::Graphics::Sampler::Filter::Point:
		return D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	case NFGE::Graphics::Sampler::Filter::Linear:
		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case NFGE::Graphics::Sampler::Filter::Anisotropic:
		return D3D11_FILTER_ANISOTROPIC;
		break;
	default:
		return D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	}
}

D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(Sampler::AddressMode addressMode)
{
	switch (addressMode)
	{
	case NFGE::Graphics::Sampler::AddressMode::Border:
		return D3D11_TEXTURE_ADDRESS_BORDER;
		break;
	case NFGE::Graphics::Sampler::AddressMode::Clamp:
		return D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	case NFGE::Graphics::Sampler::AddressMode::Mirror:
		return D3D11_TEXTURE_ADDRESS_MIRROR;
		break;
	case NFGE::Graphics::Sampler::AddressMode::Wrap:
		return D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	default:
		return D3D11_TEXTURE_ADDRESS_BORDER;
		break;
	}
}

NFGE::Graphics::Sampler::~Sampler()
{
	ASSERT(mSampler == nullptr, "[Sampler] Terminate must be called befre destruction.");
}

void NFGE::Graphics::Sampler::Initialize(Filter filter, AddressMode addressMode)
{
	D3D11_FILTER d3dFilter = GetFilter(filter);
	D3D11_TEXTURE_ADDRESS_MODE d3dAddressMode = GetAddressMode(addressMode);

	D3D11_SAMPLER_DESC sampleDesc{};
	sampleDesc.Filter = d3dFilter;
	sampleDesc.AddressU = d3dAddressMode;
	sampleDesc.AddressV = d3dAddressMode;
	sampleDesc.AddressW = d3dAddressMode;
	sampleDesc.BorderColor[0] = 1.0f;
	sampleDesc.BorderColor[1] = 1.0f;
	sampleDesc.BorderColor[2] = 1.0f;
	sampleDesc.BorderColor[3] = 1.0f;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampleDesc.MinLOD = 0;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;

	auto device = Graphics::GetDevice();
	auto hr = device->CreateSamplerState(&sampleDesc, &mSampler);
	ASSERT(SUCCEEDED(hr), "[Sampler] Failed to crreate sampler state.");
}

void NFGE::Graphics::Sampler::Terminate()
{
	SafeRelease(mSampler);
}

void NFGE::Graphics::Sampler::BindPS(uint32_t sloat) const
{
	Graphics::GetContext()->PSSetSamplers(sloat, 1, &mSampler);
}

void NFGE::Graphics::Sampler::BindVS(uint32_t sloat) const
{
	Graphics::GetContext()->VSSetSamplers(sloat, 1, &mSampler);
}
