#include "Precompiled.h"
#include "MeshBuffer.h"
#include "D3DUtil.h"
#include "Camera.h"
#include "Effect.h"

NFGE::Graphics::MeshBuffer::~MeshBuffer()
{

}

void NFGE::Graphics::MeshBuffer::Initialize(const void* inputVertices, uint32_t vertexCount, uint32_t vertexSize, bool dynamic)
{
	InitVertexBuffer(vertexCount, vertexSize, inputVertices, dynamic);
}

void NFGE::Graphics::MeshBuffer::Initialize(const void * inputVertices, uint32_t vertexCount, uint32_t vertexSize, const uint32_t * inputIndexes, uint32_t indexCount, bool dynamic)
{
	InitVertexBuffer(vertexCount, vertexSize, inputVertices, dynamic);
	InitIndexBuffer(indexCount, inputIndexes);
}

void NFGE::Graphics::MeshBuffer::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mIndexBuffer);
}

void NFGE::Graphics::MeshBuffer::SetTopology(Topology topology)
{
	switch (topology)
	{
	case NFGE::Graphics::MeshBuffer::Topology::Points:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case NFGE::Graphics::MeshBuffer::Topology::Lines:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case NFGE::Graphics::MeshBuffer::Topology::Triangles:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	default:
		break;
	}
}

void NFGE::Graphics::MeshBuffer::Update(const void * vertexData, uint32_t numVertices)
{
	mVerticesCount = numVertices;

	auto context = Graphics::GetContext();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertexData, numVertices * mVertexSize);
	context->Unmap(mVertexBuffer, 0);
}

void NFGE::Graphics::MeshBuffer::Render() const
{
	auto context = Graphics::GetContext();

	// Set vertex buffer
	ASSERT(mVertexBuffer != nullptr, "Failed to bind. mVertexBuffer not initialize yet. ");
	UINT stride = mVertexSize;
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	
	// Set primitive topology
	context->IASetPrimitiveTopology(mTopology);

	// Check if using index buffer not. 
	if (mIndexBuffer != nullptr)
	{
		// Set index buffer
		context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		// Draw indexed mesh
		context->DrawIndexed(mIndexCount, 0, 0);
	}
	else
	{
		// Draw mesh
		context->Draw(mVerticesCount, 0);
	}
}

void NFGE::Graphics::MeshBuffer::Render(Effect * effect, EffectContext* context, const Camera& camera) const
{
	effect->Set(context);
	effect->Bind(camera);
	Render();
}

void NFGE::Graphics::MeshBuffer::InitVertexBuffer(uint32_t vertexCount, uint32_t vertexSize, const void* inputVertices, bool dynamic)
{
	// Allocate memory in VRAM for our vertex buffer and initialize
	// with our vertex data.
	mVerticesCount = vertexCount;
	mVertexSize = vertexSize;
	D3D11_BUFFER_DESC vertexBufferDesc{};
	//ASSERT((inputVertices != nullptr), "Detected 0 input vertices, when initialize MeshBuffer.");
	vertexBufferDesc.ByteWidth = mVerticesCount * vertexSize;
	vertexBufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = inputVertices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = Graphics::GetDevice()->CreateBuffer(&vertexBufferDesc, inputVertices ? &initData : nullptr, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");

	SetTopology(MeshBuffer::Topology::Triangles);
}

void NFGE::Graphics::MeshBuffer::InitIndexBuffer(uint32_t indexCount, const uint32_t * inputIndexes)
{
	mIndexCount = indexCount;

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(uint32_t) * mIndexCount;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = inputIndexes;

	HRESULT hr = Graphics::GetDevice()->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");
}
