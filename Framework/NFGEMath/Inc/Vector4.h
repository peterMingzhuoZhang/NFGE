#ifndef INCLUDED_NFGE_NFGEMATH_VECTOR4_H
#define INCLUDED_NFGE_NFGEMATH_VECTOR4_H

#include "Vector3.h"

namespace NFGE {
namespace Math {

struct Vector4
{
//union
//{
//	struct { float x, y, z, w; };
//	std::array<float, 4> v;
//};

	union 
	{
		float x; float r;
	};
	union
	{
		float y; float g;
	};
	union
	{
		float z; float b;
	};
	union
	{
		float w; float a;
	};

constexpr Vector4() noexcept : Vector4(0.0f) {}																		//|TODO:UNITTest
constexpr Vector4(float f) noexcept : x(f), y(f), z(f), w(f) {}														//|TODO:UNITTest
constexpr Vector4(float _x, float _y, float _z, float _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}					//|TODO:UNITTest
constexpr Vector4(const Vector3& v, float _w) noexcept : x(v.x), y(v.y), z(v.z), w(_w) {}							//|TODO:UNITTest
																													//|TODO:UNITTest
constexpr Vector4 operator-() const { return { -x, -y, -z, -w }; }													//|TODO:UNITTest
constexpr Vector4 operator+(const Vector4& v) const { return { x + v.x, y + v.y, z + v.z, w + v.w }; }				//|TODO:UNITTest
constexpr Vector4 operator-(const Vector4& v) const { return { x - v.x, y - v.y, z - v.z, w - v.w }; }				//|TODO:UNITTest
constexpr Vector4 operator*(float s) const { return { x * s, y * s, z * s, w * s }; }								//|TODO:UNITTest
Vector4 operator/(float s) const { return Vector4(x / s, y / s, z / s, w / s); }									//|TODO:UNITTest
																													//|TODO:UNITTest
Vector4& operator+=(const Vector4& v) { x += v.x; y += v.y; z += v.z, w += v.w; return *this; }						//|TODO:UNITTest
Vector4& operator-=(const Vector4& v) { x -= v.x; y -= v.y; z -= v.z, w -= v.w; return *this; }						//|TODO:UNITTest
Vector4& operator*=(float s) { x *= s; y *= s; z *= s, w *= s; return *this; }										//|TODO:UNITTest
Vector4& operator/=(float s) { x /= s; y /= s; z /= s, w /= s; return *this; }										//|TODO:UNITTest

float Length3D() const;
float LengthSquard3D() const;
float Length() const;
float LengthSquard() const;
void Normalize();
void Normalize3D();
void WTo1();
Vector4 GetWTo1() const;
float Dot3D(const Vector4& vector) const;
friend float Dot3D(const Vector4& vector, const Vector4& other);
Vector4 Cross3D(const Vector4& vector) const;
};																													//|TODO:UNITTest

} // namespace Math
} // namespace NFGE

#endif // !INCLUDED_NFGE_NFGEMATH_VECTOR4_H
