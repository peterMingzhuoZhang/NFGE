#include "Precompiled.h"
#include "Texture.h"

#include "D3DUtil.h"
#include <DirectXTK/Inc/WICTextureLoader.h>

using namespace NFGE;
using namespace NFGE::Graphics;

NFGE::Graphics::Texture::~Texture()
{
	ASSERT(mShaderResourceView == nullptr, "[Texture] Terminate must be called befre destruction.");
}

bool NFGE::Graphics::Texture::Initialize(const std::filesystem::path& fileName)
{
	auto device = Graphics::GetDevice();
	auto context = Graphics::GetContext();

	/*const char* buffer = fileName.string.c_str();
	wchar_t wbuffer[1024];
	mbstowcs_s(nullptr, wbuffer, fileName.string.c_str(), 1024);*/

	HRESULT hr = DirectX::CreateWICTextureFromFile(device, fileName.c_str(), nullptr, &mShaderResourceView);
	if (FAILED(hr))
	{
		LOG("[Texture] Failed to load texture %ls.", fileName.c_str());
		return false;
	}

	// Cache the texture dimensions
	ID3D11Resource* resource = nullptr;
	mShaderResourceView->GetResource(&resource);

	ID3D11Texture2D* texture = static_cast<ID3D11Texture2D*>(resource);
	D3D11_TEXTURE2D_DESC desc = {};
	texture->GetDesc(&desc);

	mWidth = desc.Width;
	mHeight = desc.Height;

	return true;
}

void NFGE::Graphics::Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void NFGE::Graphics::Texture::BindPS(uint32_t slot) const
{
	Graphics::GetContext()->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

void NFGE::Graphics::Texture::BindVS(uint32_t slot) const
{
	Graphics::GetContext()->VSSetShaderResources(slot, 1, &mShaderResourceView);
}
