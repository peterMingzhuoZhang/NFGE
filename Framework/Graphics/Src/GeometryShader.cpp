#include "Precompiled.h"
#include "GeometryShader.h"

#include "D3DUtil.h"

void NFGE::Graphics::GeometryShader::Initialize(const wchar_t* fileName)
{
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	// Compile our Geometry shader code
	HRESULT hr = D3DCompileFromFile(
		fileName,
		nullptr, nullptr,
		"GS",
		"gs_5_0", 0, 0, // which compiler
		&shaderBlob,	//
		&errorBlob);
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader. Error: %s", (const char*)errorBlob->GetBufferPointer());

	//Create geometry shader
	hr = Graphics::GetDevice()->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &geometryShader);
	ASSERT(SUCCEEDED(hr), "Failed to create Geometry shader.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void NFGE::Graphics::GeometryShader::Terminate()
{
	SafeRelease(geometryShader);
}

void NFGE::Graphics::GeometryShader::Bind()
{

	Graphics::GetContext()->GSSetShader(geometryShader, nullptr, 0);
}

void NFGE::Graphics::GeometryShader::UnBind()
{
	Graphics::GetContext()->GSSetShader(nullptr, nullptr, 0);
}


