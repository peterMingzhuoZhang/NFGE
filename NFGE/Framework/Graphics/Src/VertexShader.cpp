#include "Precompiled.h"
#include "VertexShader.h"

#include "D3DUtil.h"

using namespace NFGE;
using namespace Graphics;

namespace
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetVertexLayout(uint32_t vertexFormat)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
		if (vertexFormat & VE_Position)
			desc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_Normal)
			desc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_Tangent)
			desc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_Color)
			desc.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_Texcoord)
			desc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_BlendIndex)
			desc.push_back({ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_BlendWeight)
			desc.push_back({ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		return desc;
	}
}

void NFGE::Graphics::VertexShader::Initialize(const wchar_t* fileName, uint32_t vertexFormat)
{
	auto device = Graphics::GetDevice();

	auto vertexLayout = GetVertexLayout(vertexFormat);
	// Compile our vertex shader code
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	uint32_t flags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	HRESULT hr = D3DCompileFromFile(
		//L"../Assets/Shaders/DoTransform.fx",
		fileName,
		nullptr, nullptr,
		"VS",
		"vs_5_0", flags, 0, // which compiler
		&shaderBlob,	//
		&errorBlob);
	ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader. Error: %s", (const char*)errorBlob->GetBufferPointer());

	// Create vertex shader and input layout descriptor
	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&vertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	hr = device->CreateInputLayout(
		vertexLayout.data(), static_cast<UINT>(vertexLayout.size()),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&inputLayout);
	ASSERT(SUCCEEDED(hr), "Failed to create input layout.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void NFGE::Graphics::VertexShader::Terminate()
{
	SafeRelease(inputLayout);
	SafeRelease(vertexShader);
}

void NFGE::Graphics::VertexShader::Bind()
{
	Graphics::GetContext()->IASetInputLayout(inputLayout);
	Graphics::GetContext()->VSSetShader(vertexShader, nullptr, 0);
}
