#ifndef INCLUDED_NFGE_GRAPHICS_SIMPLEDRAW_H
#define INCLUDED_NFGE_GRAPHICS_SIMPLEDRAW_H

#include "Camera.h"
#include "Colors.h"

namespace NFGE::Graphics::SimpleDraw {

	void StaticInitialize(std::filesystem::path rootPath, uint32_t maxVertexCount = 1000000);
	void StaticTerminate();

	// SimpleDraw --- In world --- WireFrame style 
	void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Graphics::Color& color);
	void AddLine(float x0, float y0, float z0, float x1, float y1, float z1, const Math::Vector4& color);
	void AddAABB(const Math::AABB& aabb, const Math::Vector4& color, bool isFace = false);
	void AddAABB(const Math::Vector3& min, const Math::Vector3& max, const Math::Vector4& color, bool isFace = false);
	void AddAABB(const Math::Vector3& center, float radius, const Math::Vector4& color, bool isFace = false);
	void AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Math::Vector4& color, bool isFace = false);
	void AddOBB(const Math::OBB& obb, const Math::Vector4& color);
	void AddCircle_FaceY(float x, float y, float z, float r, const Math::Vector4& color, uint32_t Slices = 8);
	void AddSphere(const Math::Sphere& sphere, const Math::Vector4& color, uint32_t slices = 8, uint32_t rings = 4);
	void AddSphere(const Math::Vector3& center, float radius, const Math::Vector4& color, uint32_t slices = 8, uint32_t rings = 4);
	void AddSphere(float x, float y, float z, float radius, const Math::Vector4& color, uint32_t slices = 8, uint32_t rings = 4);
	void AddTransform(const Math::Matrix4& transform);

	// SimpleDraw --- In world --- Solid style
	void AddTriangle(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, const Graphics::Color& color);

	// SimepleDraw --- In Screen --- WireFrame style
	void AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Math::Vector4& color);
	void AddScreenLine(float x0, float y0, float x1, float y1, const Math::Vector4& color);
	void AddScreenRect(const Math::Rect& rect, const Math::Vector4& color);
	void AddScreenRect(const Math::Vector2& min, const Math::Vector2& max, const Math::Vector4& color);
	void AddScreenRect(float left, float top, float right, float bottom, const Math::Vector4& color);
	void AddScreenCircle(const Math::Circle& circle, const Math::Vector4& color, uint32_t Slices = 8);
	void AddScreenCircle(const Math::Vector2& center, float r, const Math::Vector4& color, uint32_t Slices = 8);
	void AddScreenCircle(float x, float y, float r, const Math::Vector4& color, uint32_t Slices = 8);
	void AddScreenDiamond(const Math::Vector2& center, float size, const Math::Vector4& color);
	void AddScreenDiamond(float x, float y, float size, const Math::Vector4& color);

	// SimepleDraw --- In Screen --- Solid style
	void AddScreenTriangle(const Math::Vector2& v0, const Math::Vector2& v1, const Math::Vector2& v2, const Graphics::Color& color);

	void Render(const Camera& camera);

} // namespace NFGE::Graphics::SimpleDraw 

#endif // !INCLUDED_NFGE_GRAPHICS_SIMPLEDRAW_H
