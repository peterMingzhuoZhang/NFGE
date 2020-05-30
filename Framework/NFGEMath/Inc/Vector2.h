#ifndef INCLUDED_NFGE_NFGEMATH_VECTOR2_H
#define INCLUDED_NFGE_NFGEMATH_VECTOR2_H

namespace NFGE {
namespace Math {

struct Vector2
{
	union 
	{
		struct { float x, y; };
		std::array<float, 2> v;
	};

	constexpr Vector2() noexcept: Vector2(0.0f) {}
	constexpr Vector2(float f) noexcept : x(f), y(f) {}
	constexpr Vector2(float _x, float _y) noexcept : x(_x), y(_y) {}

	constexpr Vector2 operator-() const					{ return { -x,-y }; }
	constexpr Vector2 operator+(const Vector2& v) const	{ return { x + v.x, y + v.y }; }
	constexpr Vector2 operator-(const Vector2& v) const	{ return { x - v.x, y - v.y }; }
	constexpr Vector2 operator*(float s) const			{ return { x * s, y * s}; }
	Vector2 operator/(float s) const { return Vector2(x / s, y / s); }

	Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
	Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
	Vector2& operator*=(float s) { x *= s; y *= s; return *this; }
	Vector2& operator/=(float s) { x /= s; y /= s;return *this; }
};

} // namespace Math
} // namespace NFGE

#endif // !INCLUDED_NFGE_NFGEMATH_VECTOR3_H
