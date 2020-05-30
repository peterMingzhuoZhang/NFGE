#ifndef INCLUDED_NFGE_GRAPHICS_MESHBUFFER_H
#define INCLUDED_NFGE_GRAPHICS_MESHBUFFER_H

#include "VertexType.h"
#include "GraphicsSystem.h"
#include "Mesh.h"

namespace NFGE {
namespace Graphics {

class Camera;
class Effect;
struct EffectContext;
class MeshBuffer
{
public:
	enum class Topology { Points, Lines, Triangles };
	MeshBuffer() = default;
	~MeshBuffer();

	void Initialize(const void* inputVertices, uint32_t vertexCount, uint32_t vertexSize, bool dynamic = false);
	void Initialize(const void* inputVertices, uint32_t vertexCount, uint32_t vertexSize, const uint32_t* inputIndexes, uint32_t indexCount, bool dynamic = false);

	template <typename T>
	void Initialize(const T& mesh, bool dynamic = false);

	void Terminate();

	void SetTopology(Topology topology = Topology::Triangles);

	void Update(const void* vertexData, uint32_t numVertices);

	void Render() const;
	void Render(Effect* effect, EffectContext* context, const Camera& camera) const;

	int GetSize() const { return mVerticesCount; }
private:


	uint32_t mVerticesCount{ 0 };
	uint32_t mIndexCount{ 0 };
	uint32_t mVertexSize{ 0 };

	ID3D11Buffer* mVertexBuffer{ nullptr };
	ID3D11Buffer* mIndexBuffer{ nullptr };

	D3D11_PRIMITIVE_TOPOLOGY mTopology;

	void InitVertexBuffer(uint32_t vertexCount, uint32_t vertexSize, const void* inputVertices, bool dynamic);
	void InitIndexBuffer(uint32_t indexCount, const uint32_t* inputIndexes);
};

template <typename T>
void MeshBuffer::Initialize(const T& mesh, bool dynamic)
{
	Initialize(mesh.vertices.data(), static_cast<uint32_t>(std::size(mesh.vertices)), sizeof(T::VertexType), mesh.indices.data(), static_cast<uint32_t>(std::size(mesh.indices)),dynamic);
}

} // NFGE
} // Graphics

#endif // !INCLUDED_NFGE_GRAPHICS_MESHBUFFER_H

