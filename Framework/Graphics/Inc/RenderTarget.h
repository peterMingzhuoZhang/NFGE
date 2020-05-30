#ifndef INCLUDED_GRAPHICS_RENDERTARGET_H
#define INCLUDED_GRAPHICS_RENDERTARGET_H

namespace NFGE::Graphics {

class RenderTarget
{
public:
	enum class Format
	{
		RGBA_U8,
		RGBA_F16,
		RGBA_U32,
		R_F16,
		R_S32
	};

	RenderTarget() = default;
	~RenderTarget();

	RenderTarget(const RenderTarget&) = delete;
	RenderTarget& operator=(const RenderTarget&) = delete;
	
	void Initialize(uint32_t width, uint32_t height, Format format);
	void Terminate();
	
	void BeginRender();
	void ResumeRender();
	void EndRender();

	void BindPS(uint32_t slot);
	void BindVS(uint32_t slot);
	void UnbindPS(uint32_t slot);
	void UnbindVS(uint32_t slot);

	ID3D11ShaderResourceView* GetShaderResourceView() const { return mShaderResourceView; }
	
private:
	ID3D11ShaderResourceView* mShaderResourceView = nullptr;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;
	D3D11_VIEWPORT mViewport;
	D3D11_MAPPED_SUBRESOURCE mSubresource;
};

} // namespace Siege::Graphics

#endif // #ifndef INCLUDED_GRAPHICS_RENDERTARGET_H
