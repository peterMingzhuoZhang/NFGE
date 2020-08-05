#include "Precompiled.h"
#include "ConstantBuffer.h"
#include "D3DUtil.h"

NFGE::Graphics::ConstantBuffer::~ConstantBuffer()
{
	ASSERT(mConstantBuffer == nullptr, "[Graphics::ConstantBuffer] Terminate() must be called to clean up!");
}

void NFGE::Graphics::ConstantBuffer::Initialize(uint32_t constantSize)
{

	D3D11_BUFFER_DESC constantBufferDesc{};
	constantBufferDesc.ByteWidth = constantSize;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = Graphics::GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &mConstantBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create constant buffer.");
}

void NFGE::Graphics::ConstantBuffer::Terminate()
{
	SafeRelease(mConstantBuffer);
}

void NFGE::Graphics::ConstantBuffer::Load(const void * data) const
{
	Graphics::GetContext()->UpdateSubresource(mConstantBuffer, 0, nullptr, data, 0, 0);
}



void NFGE::Graphics::ConstantBuffer::BindVS(uint32_t slot) const
{
	Graphics::GetContext()->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void NFGE::Graphics::ConstantBuffer::BindGS(uint32_t slot) const
{
	Graphics::GetContext()->GSSetConstantBuffers(slot, 1, &mConstantBuffer);
}



void NFGE::Graphics::ConstantBuffer::BindPS(uint32_t slot) const
{
	Graphics::GetContext()->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}
