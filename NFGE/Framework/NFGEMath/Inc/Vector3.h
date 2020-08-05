#ifndef INCLUDED_NFGE_NFGEMATH_VECTOR3_H
#define INCLUDED_NFGE_NFGEMATH_VECTOR3_H

namespace NFGE {
namespace Math {

struct Vector3
{
	union 
	{
		struct { float x, y, z; };
		std::array<float, 3> v;
	};

	constexpr Vector3() noexcept: Vector3(0.0f) {}
	constexpr Vector3(float f) noexcept : x(f), y(f), z(f) {}
	constexpr Vector3(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}

	static Vector3 Zero() { return Vector3(); }
	static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }

	const static Vector3 XAxis;
	const static Vector3 YAxis;
	const static Vector3 ZAxis;

	constexpr Vector3 operator-() const					{ return { -x,-y, -z }; }
	constexpr Vector3 operator+(const Vector3& v) const	{ return { x + v.x, y + v.y, z + v.z }; }
	constexpr Vector3 operator-(const Vector3& v) const	{ return { x - v.x, y - v.y, z - v.z }; }
	constexpr Vector3 operator*(float s) const			{ return { x * s, y * s, z * s}; }
	constexpr bool operator==(const Vector3& v) const { return x == v.x && y == v.y && z == v.z; }
	Vector3 operator/(float s) const { return Vector3(x / s, y / s, z / s); }

	Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
	Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
};

} // namespace Math
} // namespace NFGE

#endif // !INCLUDED_NFGE_NFGEMATH_VECTOR3_H
