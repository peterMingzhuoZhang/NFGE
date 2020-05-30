#include "Precompiled.h"
#include "GraphicsSystem.h"

using namespace NFGE;
using namespace NFGE::Graphics;

// Nameless/Anonymous namespace
// Internal Linkage
namespace
{
	std::unique_ptr<GraphicsSystem> sGraphicsSystem;
	Core::WindowMessageHandler sWindowMessageHandler;
}

LRESULT CALLBACK Graphics::GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wPrarm, LPARAM lParam)
{
	if (sGraphicsSystem)
	{
		switch (message)
		{
			case WM_SIZE:
			{
				const uint32_t width = static_cast<uint32_t>(LOWORD(lParam));
				const uint32_t height = static_cast<uint32_t>(HIWORD(lParam));
				sGraphicsSystem->Resize(width, height);
				break;
			}
		}
	}
	return sWindowMessageHandler.ForwardMessage(window, message, wPrarm, lParam);
}

void GraphicsSystem::StaticInitialize(HWND window, bool fullscreen)
{
	ASSERT(sGraphicsSystem == nullptr, "[Graphics::GraphicsSystem] System already initialized!");
	sGraphicsSystem = std::make_unique<GraphicsSystem>();
	sGraphicsSystem->Initialize(window, fullscreen);
}

void GraphicsSystem::StaticTerminate()
{
	if (sGraphicsSystem != nullptr)
	{
		sGraphicsSystem->Terminate();
		sGraphicsSystem.reset();
	}
}

GraphicsSystem* GraphicsSystem::Get()
{
	ASSERT(sGraphicsSystem != nullptr, "[Graphics::graphicsSystem] No system registered.");
	return sGraphicsSystem.get();
}

GraphicsSystem::~GraphicsSystem()
{
	ASSERT(mD3ddDevice == nullptr, "[Graphics::GraphicsSystem] Terminate() must be called to clean up!");
}

void GraphicsSystem::Initialize(HWND window, bool fullscreen)
{
	RECT clientRect = {};
	GetClientRect(window, &clientRect);
	UINT width = (UINT)(clientRect.right - clientRect.left);
	UINT height = (UINT)(clientRect.bottom - clientRect.top);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // color data depth
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !fullscreen;
	// - Enable Debug mode --------------------------------------------------------
#if (defined(DEBUG) || defined(_DEBUG))
	swapChainDesc.Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif /* (defined(DEBUG) || defined(_DEBUG)) */
	// ----------------------------------------------------------------------------

	const D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&FeatureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mSwapChain,
		&mD3ddDevice,
		nullptr,
		&mImmediateContext
	);

	ASSERT(SUCCEEDED(hr), "[Graphics:;GraphicsSystem] Failed to create device or swap chain.");

	// Cashe swap chain description
	mSwapChain->GetDesc(&mSwapChainDesc);

	Resize(GetBackBufferWidth(), GetBackBufferHeight());

	// Hook appl;iocation to windows procedure
	sWindowMessageHandler.Hook(window, GraphicsSystemMessageHandler);
}

void GraphicsSystem::Terminate()
{
	// Restore original windows procedure
	sWindowMessageHandler.Unhook();

	SafeRelease(mDepthStencilView);
	SafeRelease(mDepthStencilBuffer);
	SafeRelease(mRenderTargetView);
	SafeRelease(mSwapChain);
	SafeRelease(mImmediateContext);
	SafeRelease(mD3ddDevice);
}

void GraphicsSystem::BeginRender()
{
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, (FLOAT*)(&mClearColor));
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicsSystem::EndRender()
{
	mSwapChain->Present(mVSync, 0);
}

void GraphicsSystem::ToggleFullscreen()
{
	BOOL fullscreen;
	mSwapChain->GetFullscreenState(&fullscreen, nullptr);
	mSwapChain->SetFullscreenState(!fullscreen, nullptr);
}

void NFGE::Graphics::GraphicsSystem::Resize(uint32_t width, uint32_t height)
{
	mImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

	SafeRelease(mRenderTargetView);
	SafeRelease(mDepthStencilView);

	HRESULT hr;
	if (width != GetBackBufferWidth() || height != GetBackBufferHeight())
	{
		hr = mSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Filed to resize swap chain buffer.");

		// Cache swap chin description
		mSwapChain->GetDesc(&mSwapChainDesc);
	}

	// Create a render target view
	ID3D11Texture2D* backBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer); // Direx11 give us a LPVOID* (all kind pointer), we cast it to a type then use it
	ASSERT(SUCCEEDED(hr), "[Graphics::GraphicsSystem] Failed to access swap chain buffer.");

	hr = mD3ddDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
	SafeRelease(backBuffer);
	ASSERT(SUCCEEDED(hr), "[Graphics::GraphicsSystem] failed to create render target view.");

	// Create depth stencial texture
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = GetBackBufferWidth();
	descDepth.Height = GetBackBufferHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = mD3ddDevice->CreateTexture2D(&descDepth, nullptr, &mDepthStencilBuffer);
	ASSERT(SUCCEEDED(hr), "[Graphics::GraphicsSystem] failed to create depth stencil buffer.");

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = descDepth.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = mD3ddDevice->CreateDepthStencilView(mDepthStencilBuffer, &dsvDesc, &mDepthStencilView);
	ASSERT(SUCCEEDED(hr), "[Graphics::GraphicsSystem] failed to create depth stencil view.");

	// Set the render target view and depth stencil view
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// Setup the viewport
	mViewport.Width = (float)GetBackBufferWidth();
	mViewport.Height = (float)GetBackBufferHeight();
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mImmediateContext->RSSetViewports(1, &mViewport);
}

void NFGE::Graphics::GraphicsSystem::ResetRenderTarget()
{
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void NFGE::Graphics::GraphicsSystem::ResetViewport()
{
	mImmediateContext->RSSetViewports(1, &mViewport);
}

uint32_t NFGE::Graphics::GraphicsSystem::GetBackBufferWidth() const
{
	return mSwapChainDesc.BufferDesc.Width;
}

uint32_t NFGE::Graphics::GraphicsSystem::GetBackBufferHeight() const
{
	return mSwapChainDesc.BufferDesc.Height;
}

