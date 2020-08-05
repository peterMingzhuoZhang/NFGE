#ifndef INCLUDED_NFGE_GRAPHICS_MESH_H
#define INCLUDED_NFGE_GRAPHICS_MESH_H

#include "VertexType.h"

namespace NFGE {
namespace Graphics {

template <typename T>
struct MeshBase
{
	using VertexType = T;

public:
	void Allocate(uint32_t numVertices, uint32_t numIndices)
	{
		vertices.resize(numVertices);
		indices.resize(numIndices);
		mNumVertices = numVertices;
		mNumIndices = numIndices;
	}

	void Destroy()
	{
		vertices.clear();
		indices.clear();
		mNumVertices = 0;
		mNumIndices = 0;
	}

	const VertexType& GetVertex(uint32_t index) const
	{
		ASSERT(index < mNumVertices, "[Mesh] Index out of range.");
		return vertices[index];
	}

	VertexType& GetVertex(uint32_t index)
	{
		return const_cast<VertexType&>(static_cast<const MeshBase*>(this)->GetVertex(index));
	}

	const uint32_t& GetIndex(uint32_t index) const
	{
		ASSERT(index < mNumIndices, "[Mesh] Index out of range.");
		return indices[index];
	}

	uint32_t& GetIndex(uint32_t index)
	{
		return const_cast<uint32_t&>(static_cast<const MeshBase*>(this)->GetIndex(index));
	}

	const std::vector<VertexType>& GetVertices() const { return vertices; }
	std::vector<uint32_t> GetIndices() const { return indices; }
	uint32_t GetVertexCount() const { return mNumVertices; }
	uint32_t GetIndexCount() const { return mNumIndices; }


public:
	std::vector<VertexType> vertices;
	std::vector<uint32_t> indices;

	uint32_t mNumVertices{ 0 };
	uint32_t mNumIndices{ 0 };
};

using MeshPC = MeshBase<VertexPC>;
using MeshPX = MeshBase<VertexPX>;
using MeshPN = MeshBase<VertexPN>;
using MeshPNX = MeshBase<VertexPNX>;
using Mesh = MeshBase<Vertex>;
using BoneMesh = MeshBase<BoneVertex>;

} // namespace NFGE
} // namespace Graphics

#endif // !INCLUDED_NFGE_GRAPHICS_MESH_H
