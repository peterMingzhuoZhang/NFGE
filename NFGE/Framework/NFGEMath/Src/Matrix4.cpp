#include "Precompiled.h"
#include "Matrix4.h"
#include "Vector4.h"
#include "Vector3.h"
#include "MathUtil.h"

using namespace NFGE::Math;

NFGE::Math::Matrix4::Matrix4(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44)
	:	_11(_11), _12(_12), _13(_13), _14(_14),
		_21(_21), _22(_22), _23(_23), _24(_24),
		_31(_31), _32(_32), _33(_33), _34(_34),
		_41(_41), _42(_42), _43(_43), _44(_44)
{

}
Matrix4::Matrix4(const Matrix4 & other)
{
	mV[0] = other.mV[0];
	mV[1] = other.mV[1];
	mV[2] = other.mV[2];
	mV[3] = other.mV[3];
	mV[4] = other.mV[4];
	mV[5] = other.mV[5];
	mV[6] = other.mV[6];
	mV[7] = other.mV[7];
	mV[8] = other.mV[8];
	mV[9] = other.mV[9];
	mV[10] = other.mV[10];
	mV[11] = other.mV[11];
	mV[12] = other.mV[12];
	mV[13] = other.mV[13];
	mV[14] = other.mV[14];
	mV[15] = other.mV[15];
}

Matrix4 & NFGE::Math::Matrix4::operator=(const Matrix4 & other)
{
	if (this == &other)
		return *this;
	mV[0] = other.mV[0];
	mV[1] = other.mV[1];
	mV[2] = other.mV[2];
	mV[3] = other.mV[3];
	mV[4] = other.mV[4];
	mV[5] = other.mV[5];
	mV[6] = other.mV[6];
	mV[7] = other.mV[7];
	mV[8] = other.mV[8];
	mV[9] = other.mV[9];
	mV[10] = other.mV[10];
	mV[11] = other.mV[11];
	mV[12] = other.mV[12];
	mV[13] = other.mV[13];
	mV[14] = other.mV[14];
	mV[15] = other.mV[15];
	return *this;
}

bool NFGE::Math::Matrix4::operator==(const Matrix4 & other) const
{
	for (size_t i = 0; i < 16; ++i)
	{
		if (!(mV[i] == other.mV[i]))
			return false;
	}
	return true;
}


bool NFGE::Math::Matrix4::operator!=(const Matrix4 & other) const
{
	for (size_t i = 0; i < 16; ++i)
	{
		if (!(mV[i] == other.mV[i]))
			return true;
	}
	return false;
}

bool NFGE::Math::Matrix4::IsIdentity() const
{
	return (_11 == 1.0f && _12 == 0.0f && _13 == 0.0f && _14 == 0.0f &&
			_21 == 0.0f && _22 == 1.0f && _23 == 0.0f && _24 == 0.0f &&
			_31 == 0.0f && _32 == 0.0f && _33 == 1.0f && _34 == 0.0f &&
			_41 == 0.0f && _42 == 0.0f && _43 == 0.0f && _44 == 1.0f);
}

bool NFGE::Math::Matrix4::IsZero() const
{
	for (size_t i = 0; i < 16; ++i)
	{
		if (fabsf(mV[i]) > (NFGE::Math::Epsilon)) return false;  //compare with the least value in float
	}
	return true;
}

void NFGE::Math::Matrix4::SetRows(const Vector4 & row1, const Vector4 & row2, const Vector4 & row3, const Vector4 & row4)
{
	_11 = row1.x; _12 = row1.y; _13 = row1.z; _14 = row1.w;
	_21 = row2.x; _22 = row2.y; _23 = row2.z; _24 = row2.w;
	_31 = row3.x; _32 = row3.y; _33 = row3.z; _34 = row3.w;
	_41 = row4.x; _42 = row4.y; _43 = row4.z; _44 = row4.w;
}

void NFGE::Math::Matrix4::GetRows(Vector4 & row1, Vector4 & row2, Vector4 & row3, Vector4 & row4) const
{
	row1.x = _11; row1.y = _12;  row1.z = _13; row1.w = _14;
	row2.x = _21; row2.y = _22;  row2.z = _23; row2.w = _24;
	row3.x = _31; row3.y = _32;  row3.z = _33; row3.w = _34;
	row4.x = _41; row4.y = _42;  row4.z = _43; row4.w = _44;
}

Vector4 NFGE::Math::Matrix4::GetRow(unsigned int i) const
{
	ASSERT(i < 4, "[Matrix] Get row index out of bound.");
	return Vector4(mV[i * 4], mV[i * 4 + 1], mV[i * 4 + 2], mV[i * 4 + 3]);
}

void NFGE::Math::Matrix4::SetColumns(const Vector4 & col1, const Vector4 & col2, const Vector4 & col3, const Vector4 & col4)
{
	_11 = col1.x; _12 = col2.x; _13 = col3.x; _14 = col4.x;
	_21 = col1.y; _22 = col2.y; _23 = col3.y; _24 = col4.y;
	_31 = col1.z; _32 = col2.z; _33 = col3.z; _34 = col4.z;
	_41 = col1.w; _42 = col2.w; _43 = col3.w; _44 = col4.w;
}

void NFGE::Math::Matrix4::GetColumns(Vector4 & col1, Vector4 & col2, Vector4 & col3, Vector4 & col4) const
{
	col1.x = _11; col2.x = _12;  col3.x = _13; col4.x = _14;
	col1.y = _21; col2.y = _22;  col3.y = _23; col4.y = _24;
	col1.z = _31; col2.z = _32;  col3.z = _33; col4.z = _34;
	col1.w = _41; col2.w = _42;  col3.w = _43; col4.w = _44;
}

Vector4 NFGE::Math::Matrix4::GetColumn(unsigned int i) const
{
	ASSERT(i < 4, "[Matrix] Get col index out of bound.");
	return Vector4(mV[i], mV[i + 4], mV[i + 8], mV[i + 12]);
}

void NFGE::Math::Matrix4::Clean()
{
	for (size_t i = 0; i < 16; i++)
	{
		if (::IsZero(mV[i]))
			mV[i] = 0.0f;
	}
}

void Matrix4::Identity()
{
	mV[0] = 1.0f;		mV[4] = 0.0f;		mV[8] = 0.0f;		mV[12] = 0.0f;
	mV[1] = 0.0f;		mV[5] = 1.0f;		mV[9] = 0.0f;		mV[13] = 0.0f;
	mV[2] = 0.0f;		mV[6] = 0.0f;		mV[10] = 1.0f;		mV[14] = 0.0f;
	mV[3] = 0.0f;		mV[7] = 0.0f;		mV[11] = 0.0f;		mV[15] = 1.0f;
}

Matrix4 & Matrix4::Transpose()
{
	float temp;
	temp = _12; _12 = _21; _21 = temp;
	temp = _13; _13 = _31; _31 = temp;
	temp = _14; _14 = _41; _41 = temp;
	temp = _23; _23 = _32; _32 = temp;
	temp = _24; _24 = _42; _42 = temp;
	temp = _34; _34 = _43; _43 = temp;
	return *this;
}

Matrix4 & NFGE::Math::Matrix4::Rotation(float xRotation, float yRotation, float zRotation)
{
	// first convert angles to radians
	xRotation = DEG2RAD(xRotation);
	yRotation = DEG2RAD(yRotation);
	zRotation = DEG2RAD(zRotation);

	// pre-calc the cos's and sin's
	float cosX = cosf(xRotation);
	float sinX = sinf(xRotation);
	float cosY = cosf(yRotation);
	float sinY = sinf(yRotation);
	float cosZ = cosf(zRotation);
	float sinZ = sinf(zRotation);

	_11 = (cosY * cosZ);							_12 = -(sinX * sinY * cosZ) + (cosX * sinZ);	_13 = (cosX * sinY * cosZ) + (sinX * sinZ);		_14 = 0.0f;
	_21 = -(cosY * sinZ);							_22 = (sinX * sinY * sinZ) + (cosX * cosZ);		_23 = -(cosX * sinY * sinZ) + (sinX * cosZ);	_24 = 0.0f;
	_31 = -(sinY);									_32 = -(sinX * cosY);							_33 = (cosX * cosY);							_34 = 0.0f;
	_41 = 0.0f;										_42 = 0.0f;										_43 = 0.0f;										_44 = 1.0f;
	return *this;
}

Matrix4 & Matrix4::Scaling(const Vector4 & scale)
{
	_11 = scale.x;	_12 = 0.0f;		_13 = 0.0f;		_14 = 0.0f;
	_21 = 0.0f;		_22 = scale.y;	_23 = 0.0f;		_24 = 0.0f;
	_31 = 0.0f;		_32 = 0.0f;		_33 = scale.z;	_34 = 0.0f;
	_41 = 0.0f;		_42 = 0.0f;		_43 = 0.0f;		_44 = scale.w;
	return *this;
}

Matrix4 & Matrix4::Scaling3D(const Vector4 & scale)
{
	Vector4 scaleVector4 = scale.GetWTo1();

	_11 = scaleVector4.x;	_12 = 0.0f;				_13 = 0.0f;				_14 = 0.0f;
	_21 = 0.0f;				_22 = scaleVector4.y;	_23 = 0.0f;				_24 = 0.0f;
	_31 = 0.0f;				_32 = 0.0f;				_33 = scaleVector4.z;	_34 = 0.0f;
	_41 = 0.0f;				_42 = 0.0f;				_43 = 0.0f;				_44 = scaleVector4.w;
	return *this;
}


Matrix4 & Matrix4::Translation(float tx, float ty, float tz)
{
	_11 = 1.0f;		_12 = 0.0f;		_13 = 0.0f;		_14 = 0.0f;
	_21 = 0.0f;		_22 = 1.0f;		_23 = 0.0f;		_24 = 0.0f;
	_31 = 0.0f;		_32 = 0.0f;		_33 = 1.0f;		_34 = 0.0f;
	_41 = tx;		_42 = ty;		_43 = tz;		_44 = 1.0f;
	return *this;
}

Matrix4 & Matrix4::Shear(float x, float y, float z) // Order matters  Current order x->y->z
{
	_11 = 1.0f;			_12 = y;					_13 = y * z + z;						_14 = 0.0f;
	_21 = x;			_22 = x * y + 1.0f;			_23 = x*z+x*y*z + z;					_24 = 0.0f;
	_31 = x;			_32 = x * y + y;			_33 = x * z + x * y*z + y * z + 1.0f;	_34 = 0.0f;
	_41 = 0.0f;			_42 = 0.0f;					_43 = 0.0f;								_44 = 1.0f;
	return *this;
}

Matrix4 & Matrix4::RotationX(float angle)
{
	angle = DEG2RAD(angle);
	_11 = 1.0f;		_12 = 0.0f;			_13 = 0.0f;			_14 = 0.0f;
	_21 = 0.0f;		_22 = cosf(angle);	_23 = sinf(angle);	_24 = 0.0f;
	_31 = 0.0f;		_32 = -sinf(angle);	_33 = cosf(angle);	_34 = 0.0f;
	_41 = 0.0f;		_42 = 0.0f;			_43 = 0.0f;			_44 = 1.0f;
	return *this;
}

Matrix4 & Matrix4::RotationY(float angle)
{
	angle = DEG2RAD(angle);
	_11 = cosf(angle);	_12 = 0.0f;		_13 = -sinf(angle);		_14 = 0.0f;
	_21 = 0.0f;			_22 = 1.0f;		_23 = 0.0f;				_24 = 0.0f;
	_31 = sinf(angle);	_32 = 0.0f;		_33 = cosf(angle);		_34 = 0.0f;
	_41 = 0.0f;			_42 = 0.0f;		_43 = 0.0f;				_44 = 1.0f;
	return *this;
}

Matrix4 & Matrix4::RotationZ(float angle)
{
	angle = DEG2RAD(angle);
	_11 = cosf(angle);	_12 = sinf(angle);	_13 = 0.0f;	_14 = 0.0f;
	_21 = -sinf(angle);	_22 = cosf(angle);	_23 = 0.0f;	_24 = 0.0f;
	_31 = 0.0f;			_32 = 0.0f;			_33 = 1.0f;	_34 = 0.0f;
	_41 = 0.0f;			_42 = 0.0f;			_43 = 0.0f;	_44 = 1.0f;
	return *this;
}

Matrix4 & NFGE::Math::Matrix4::Rotation(const Vector3& axis, float angle)
{
	angle = DEG2RAD(angle);
	_11 = 1 + (1 - cos(angle))*(axis.x * axis.x - 1);	_12 = -axis.z * sin(angle) + (1 - cos(angle))*axis.x * axis.y;		_13 = axis.y * sin(angle) + (1 - cos(angle))*axis.x*axis.z;		_14 = 0.0f;
	_21 = axis.z * sin(angle) + (1 - cos(angle))*axis.x*axis.y;			_22 = 1 + (1 - cos(angle))*(axis.y*axis.y - 1);		_23 = -axis.x * sin(angle) + (1 - cos(angle))*axis.y*axis.z;				_24 = 0.0f;
	_31 = -axis.y * sin(angle) + (1 - cos(angle))*axis.x*axis.z;	_32 = axis.x * sin(angle) + (1 - cos(angle))*axis.y*axis.z;		_33 = 1 + (1 - cos(angle))*(axis.z*axis.z - 1);		_34 = 0.0f;
	_41 = 0.0f;			_42 = 0.0f;		_43 = 0.0f;				_44 = 1.0f;
	return *this;
}

Matrix4 NFGE::Math::Matrix4::sTranslation(const Vector3 & v)
{
	return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, v.x, v.y, v.z, 1.0f);
}

Matrix4 NFGE::Math::Matrix4::sScaling(const Vector3 & s)
{
	return Matrix4(s.x, 0.0f, 0.0f, 0.0f, 0.0f, s.y, 0.0f, 0.0f, 0.0f, 0.0f, s.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4 Matrix4::operator+(const Matrix4 & other) const
{
	Matrix4 retMatrix;
	for (size_t i = 0; i < 16; i++)
	{
		retMatrix.mV[i] = mV[i] + other.mV[i];
	}
	return retMatrix;
}

Matrix4 & Matrix4::operator+=(const Matrix4 & other)
{
	for (size_t i = 0; i < 16; i++)
	{
		mV[i] += other.mV[i];
	}
	return *this;
}

Matrix4 Matrix4::operator-(const Matrix4 & other) const
{
	Matrix4 retMatrix;
	for (size_t i = 0; i < 16; i++)
	{
		retMatrix.mV[i] = mV[i] - other.mV[i];
	}
	return retMatrix;
}

Matrix4 & Matrix4::operator-=(const Matrix4 & other)
{
	for (size_t i = 0; i < 16; i++)
	{
		mV[i] -= other.mV[i];
	}
	return *this;
}
Matrix4 Matrix4::operator-() const
{
	Matrix4 retMatrix;
	for (size_t i = 0; i < 16; i++)
	{
		retMatrix.mV[i] = -mV[i];
	}
	return retMatrix;
}

Matrix4 & Matrix4::operator*=(const Matrix4 & other)
{
	Matrix4 temp(*this);
	for (size_t i = 0; i < 16; i++)
	{
		size_t r = i / 4;
		size_t c = i % 4;
		float sum = 0.0f;
		for (size_t j = 0; j < 4; j++)
		{
			sum += temp.mV[(r * 4 + j)] * other.mV[(j * 4 + c)];
		}
		mV[i] = sum;
	}

	return *this;
}

Matrix4 Matrix4::operator*(const Matrix4 & other) const
{
	Matrix4 retMatrix(*this);
	for (size_t i = 0; i < 16; i++)
	{
		size_t r = i / 4;
		size_t c = i % 4;
		float sum = 0.0f;
		for (size_t j = 0; j < 4; j++)
		{
			sum += mV[(r * 4 + j)] * other.mV[(j * 4 + c)];
		}
		retMatrix.mV[i] = sum;
	}

	return retMatrix;
}

Vector4 Matrix4::operator*(const Vector4 & vector) const
{
	return Vector4
	(
		_11 * vector.x + _12 * vector.y + _13 * vector.z + _14 * vector.w,
		_21 * vector.x + _22 * vector.y + _23 * vector.z + _24 * vector.w,
		_31 * vector.x + _32 * vector.y + _33 * vector.z + _34 * vector.w,
		_41 * vector.x + _42 * vector.y + _43 * vector.z + _44 * vector.w
	);
}

Matrix4 & Matrix4::operator*=(float scalar)
{
	for (size_t i = 0; i < 16; i++)
	{
		mV[i] *= scalar;
	}
	return *this;
}

Matrix4 Matrix4::operator*(float scalar) const
{
	Matrix4 retMatrix;
	for (size_t i = 0; i < 16; i++)
	{
		retMatrix.mV[i] = mV[i] * scalar;
	}
	return retMatrix;
}


