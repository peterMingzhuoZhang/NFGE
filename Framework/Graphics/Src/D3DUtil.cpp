#include "Precompiled.h"
#include "D3DUtil.h"

#include "GraphicsSystem.h"
#include "Texture.h"

ID3D11Device* NFGE::Graphics::GetDevice()
{
	return GraphicsSystem::Get()->mD3ddDevice;
}

ID3D11DeviceContext * NFGE::Graphics::GetContext()
{
	return GraphicsSystem::Get()->mImmediateContext;
}

ID3D11ShaderResourceView * NFGE::Graphics::GetShaderResourceView(const Texture & texture)
{
	return texture.mShaderResourceView;
}
