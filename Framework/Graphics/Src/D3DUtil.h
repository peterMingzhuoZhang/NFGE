#ifndef INCLUDED_NFGE_GRAPHICS_D3DUTIL_H
#define INCLUDED_NFGE_GRAPHICS_D3DUTIL_H

namespace NFGE::Graphics
{
	class Texture;
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	ID3D11ShaderResourceView* GetShaderResourceView(const Texture& texture);

}

#endif // !INCLUDED_NFGE_GRAPHICS_D3DUTIL_H
