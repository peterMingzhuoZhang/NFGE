#include "Precompiled.h"
#include "PixelShader.h"

#include "D3DUtil.h"

void NFGE::Graphics::PixelShader::Initialize(const wchar_t* fileName, const char* shaderName)
{
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	// Compile our pixel shader code

	uint32_t flags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	HRESULT hr = D3DCompileFromFile(
		//L"../Assets/Shaders/DoSomething.fx",
		fileName,
		nullptr, nullptr,
		shaderName,
		"ps_5_0", flags, 0, // which compiler
		&shaderBlob,	//
		&errorBlob);
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader. Error: %s", (const char*)errorBlob->GetBufferPointer());

	//Create pixel shader
	hr = Graphics::GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &pixelshader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void NFGE::Graphics::PixelShader::Terminate()
{
	SafeRelease(pixelshader);
}

void NFGE::Graphics::PixelShader::Bind()
{

	Graphics::GetContext()->PSSetShader(pixelshader, nullptr, 0);
}
