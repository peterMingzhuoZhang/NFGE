#include "Precompiled.h"
#include "Vector4.h"

using namespace NFGE::Math;

//-------------------------------------------------------------------------------
float Vector4::Length3D() const
{
	return (float)sqrt(LengthSquard3D());
}


//-------------------------------------------------------------------------------
// @ Vector4::LengthSquard3D()
//-------------------------------------------------------------------------------
// Calulate length square in 3d (w as homogenus coordinate)
//-------------------------------------------------------------------------------
float Vector4::LengthSquard3D() const
{
	Vector4 vector1 = GetWTo1();
	return (vector1.x * vector1.x + vector1.y * vector1.y + vector1.z * vector1.z);
}

float Vector4::Length() const
{
	return (float)sqrt(LengthSquard());
}

float Vector4::LengthSquard() const
{
	return (x * x + y * y + z * z + w + w);
}


//-------------------------------------------------------------------------------
// @ Vector4::Normalize()
//-------------------------------------------------------------------------------
// Normalize the vector4 to unit vector
//-------------------------------------------------------------------------------
void Vector4::Normalize()
{
	float length = Length();
	if (length == 0) return;

	x /= length;
	y /= length;
	z /= length;
	w /= length;
}

void Vector4::Normalize3D()
{
	float length = Length3D();
	if (length == 0) return;

	x /= length;
	y /= length;
	z /= length;
	w = 1.0f;
}

void Vector4::WTo1()
{
	if (w == 1.0f || w == 0.0f) return;

	x /= w;
	y /= w;
	z /= w;
	w = 1.0f;
}

Vector4 Vector4::GetWTo1() const
{
	if (w == 0.0f) return Vector4();
	if (w == 1.0f) return *this;

	return Vector4(x / w, y / w, z / w, 1.0f);
}

float Vector4::Dot3D(const Vector4 & vector) const
{
	Vector4 vector1 = GetWTo1();
	Vector4 vector2 = vector.GetWTo1();
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

Vector4 Vector4::Cross3D(const Vector4 & vector2) const
{
	Vector4 cross;

	cross.x = (y * vector2.z) - (vector2.y * z);
	cross.y = (vector2.x * z) - (x * vector2.z);    //(-j)
	cross.z = (x * vector2.y) - (vector2.x * y);
	cross.w = 1.0f;

	return cross;
}
