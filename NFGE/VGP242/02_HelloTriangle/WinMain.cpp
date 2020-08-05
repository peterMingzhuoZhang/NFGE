#include <Core\Inc\Core.h>
#include <Graphics\Inc\Graphics.h>
#include <Graphics/Src/D3DUtil.h>
#include <NFGEMath/Inc/NFGEMath.h>

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Math;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// Setup our application window
	Window myWindow;
	myWindow.Initialize(hInstance, "Hello Triangle", 1280, 720);

	// Initialize the graphics system
	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	// Define our vertex data
	const VertexPC vertices[] =
	{
		{ {  0.0f, 0.5f,0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{ {  0.5f, 0.0f,0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
		{ { -0.5f, 0.0f,0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}
	};
	const D3D11_INPUT_ELEMENT_DESC elementDesc[] = 
	{
		{ 
			"POSITION", 
			0, 
			DXGI_FORMAT_R32G32B32_FLOAT, 
			0, 
			D3D11_APPEND_ALIGNED_ELEMENT, 
			D3D11_INPUT_PER_VERTEX_DATA, 
			0
		},
		{ 
			"COLOR", 
			0, 
			DXGI_FORMAT_R32G32B32A32_FLOAT, 
			0, 
			D3D11_APPEND_ALIGNED_ELEMENT, 
			D3D11_INPUT_PER_VERTEX_DATA, 
			0
		},
	};

	auto device = GetDevice();

	// Allocate memory in VRAM for our vertex buffer and initialize
	// with our vertex data.
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(std::size(vertices) * sizeof(VertexPC));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride= 0;
	
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	ID3D11Buffer* vertexBuffer = nullptr;
	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &initData, &vertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");

	// Compile our vertex shader code
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx", 
		nullptr, nullptr, 
		"VS", 
		"vs_5_0", 0, 0, // which compiler
		&shaderBlob,	//
		&errorBlob);
	ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader. Error: %s", (const char*)errorBlob->GetBufferPointer());

	// Create vertex shader and input layout descriptor
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&vertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	hr = device->CreateInputLayout(
		elementDesc, static_cast<UINT>(std::size(elementDesc)),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&inputLayout);
	ASSERT(SUCCEEDED(hr), "Failed to create input layout.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

	
	// Compile our pixel shader code
	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx",
		nullptr, nullptr,
		"PS",
		"ps_5_0", 0, 0, // which compiler
		&shaderBlob,	//
		&errorBlob);
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader. Error: %s", (const char*)errorBlob->GetBufferPointer());

	//Create pixel shader
	ID3D11PixelShader* pixelshader = nullptr;
	hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &pixelshader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

	bool done = false;

	while (!done)
	{
		done = myWindow.ProcessMessage();
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			done = true;
		}

		// Run your game logic ...
		GraphicsSystem::Get()->BeginRender();
	
		auto context = GetContext();
		
		context->IASetInputLayout(inputLayout);
		context->VSSetShader(vertexShader, nullptr, 0);
		context->PSSetShader(pixelshader, nullptr, 0);

		UINT stride = sizeof(VertexPC);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Draw stuff
		context->Draw(static_cast<UINT>(std::size(vertices)), 0);

		GraphicsSystem::Get()->EndRender();
	}

	SafeRelease(pixelshader);
	SafeRelease(inputLayout);
	SafeRelease(vertexShader);
	SafeRelease(vertexBuffer);

	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}