#ifndef INCLUDED_NFGE_GRAPHICS_GRAPHICSSYSTEM_H
#define INCLUDED_NFGE_GRAPHICS_GRAPHICSSYSTEM_H

#include "Colors.h"

namespace NFGE {
namespace Graphics {

class GraphicsSystem
{
public:
	static void StaticInitialize(HWND window, bool fullscreen);
	static void StaticTerminate();
	static GraphicsSystem* Get();

public:
	GraphicsSystem() = default;
	~GraphicsSystem();

	GraphicsSystem(const GraphicsSystem&) = delete;
	GraphicsSystem& operator=(const GraphicsSystem&) = delete;

	void Initialize(HWND window, bool fullscreen);
	void Terminate();

	void BeginRender();
	void EndRender();

	void ToggleFullscreen();
	void Resize(uint32_t width, uint32_t height);

	void ResetRenderTarget();
	void ResetViewport();

	void SetClearColor(Color clearColor) { mClearColor = clearColor; }
	Color& GetClearColor() { return mClearColor; }
	void SetVSync(bool vSync) { mVSync = vSync ? 1 : 0, 0; }

	uint32_t GetBackBufferWidth() const;
	uint32_t GetBackBufferHeight() const;


private:
	friend LRESULT CALLBACK GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wPrarm, LPARAM lParam);
	friend ID3D11Device* GetDevice();
	friend ID3D11DeviceContext* GetContext();

	ID3D11Device* mD3ddDevice{ nullptr };
	ID3D11DeviceContext* mImmediateContext{ nullptr };

	IDXGISwapChain* mSwapChain{ nullptr };
	ID3D11RenderTargetView* mRenderTargetView{ nullptr };

	ID3D11Texture2D* mDepthStencilBuffer{ nullptr };
	ID3D11DepthStencilView* mDepthStencilView{ nullptr };

	DXGI_SWAP_CHAIN_DESC mSwapChainDesc;
	D3D11_VIEWPORT mViewport;

	Color mClearColor = {0.0f,0.0f,0.0f,0.0f};
	int mVSync = 1;
};

} // namespace Graphics
} // namespace Engine

#endif // !INCLUDED_NFGE_GRAPHICS_GRAPHICSSYSTEM_H
