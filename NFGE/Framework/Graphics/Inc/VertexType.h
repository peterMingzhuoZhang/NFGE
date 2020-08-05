#ifndef INCLUDED_NFGE_GRAPHICS_VERTEXTYPE_H
#define INCLUDED_NFGE_GRAPHICS_VERTEXTYPE_H

#include "Common.h"
#include "Colors.h"

namespace NFGE {
namespace Graphics 
{

// Vertex element flags
constexpr uint32_t VE_Position		= 0x1 << 0;
constexpr uint32_t VE_Normal		= 0x1 << 1;
constexpr uint32_t VE_Tangent		= 0x1 << 2;
constexpr uint32_t VE_Color			= 0x1 << 3;
constexpr uint32_t VE_Texcoord		= 0x1 << 4;
constexpr uint32_t VE_BlendIndex	= 0x1 << 5;
constexpr uint32_t VE_BlendWeight	= 0x1 << 6;

#define VERTEX_FORMAT(fmt)\
	static constexpr uint32_t Format = fmt

struct VertexPC
{
	VERTEX_FORMAT(VE_Position | VE_Color);

	VertexPC() = default;

	constexpr VertexPC(const Math::Vector3& position, const Color& color)
		: position(position)
		, color(color)
	{}

	Math::Vector3 position;
	Color color;
};

struct VertexPX
{
	VERTEX_FORMAT(VE_Position | VE_Texcoord);
	Math::Vector3 position;
	Math::Vector2 texcoord;
};

struct VertexPN
{
	VERTEX_FORMAT(VE_Position | VE_Normal);
	Math::Vector3 position;
	Math::Vector3 normal;
};

struct VertexPNC
{
	VERTEX_FORMAT(VE_Position | VE_Normal | VE_Color);
	Math::Vector3 position;
	Math::Vector3 normal;
	Color color;
};

struct VertexPNX
{
	VERTEX_FORMAT(VE_Position | VE_Normal | VE_Texcoord);
	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector2 texcoord;
};

struct Vertex
{
	VERTEX_FORMAT(VE_Position | VE_Normal | VE_Tangent | VE_Texcoord);
	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector3 tangent;
	Math::Vector2 texcoord;
};

struct BoneVertex
{
	VERTEX_FORMAT(VE_Position | VE_Normal | VE_Tangent | VE_Texcoord | VE_BlendIndex | VE_BlendWeight);
	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector3 tangent;
	Math::Vector2 texcoord;
	int boneIndices[4] = { 0 };
	float boneWeights[4] = { 0.0f };
};

} // namespace NFGE
} // namespace Graphics

#endif // !INCLUDED_NFGE_GRAPHICS_VERTEXTYPE_H
