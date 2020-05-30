#ifndef INCLUDE_NFGE_NFGEMATH_MATRIX4_H
#define INCLUDE_NFGE_NFGEMATH_MATRIX4_H

namespace NFGE {
namespace Math {

	struct Vector3;
	struct Vector4;

	struct Transform3D
	{
		float tX, tY, tZ;
		float rX, rY, rZ;
		float scX, scY, scZ;
		float shX, shY, shZ;
		Transform3D()
			:tX(0.0f), tY(0.0f), tZ(0.0f)
			, rX(0.0f), rY(0.0f), rZ(0.0f)
			, scX(1.0f), scY(1.0f), scZ(1.0f)
			, shX(0.0f), shY(0.0f), shZ(0.0f)
		{}
	};
struct Matrix4
{
	// copy operations
	Matrix4() = default;
	Matrix4(	float _11, float _12, float _13, float _14,
				float _21, float _22, float _23, float _24,
				float _31, float _32, float _33, float _34,
				float _41, float _42, float _43, float _44
	);
	Matrix4(const Matrix4& other);
	Matrix4& operator=(const Matrix4& other);

	// comparison
	bool operator==(const Matrix4& other) const;
	bool operator!=(const Matrix4& other) const;
	bool IsIdentity() const;
	bool IsZero() const;

	// manipulators
	void SetRows(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4 & row4);
	void GetRows(Vector4& row1, Vector4& row2, Vector4& row3, Vector4& row4) const;
	Vector4 GetRow(unsigned int i) const;

	void SetColumns(const Vector4& col1, const Vector4& col2, const Vector4& col3, const Vector4& col4);
	void GetColumns(Vector4& col1, Vector4& col2, Vector4& col3, Vector4 & col4) const;
	Vector4 GetColumn(unsigned int i) const;

	void Clean();     // Set elements close to zero equal to zero
	void Identity();  // Set to identity matrix

	Matrix4& Transpose();   // Set self to transpose
	// transformations
	Matrix4& Rotation(float xRotation, float yRotation, float zRotation);

	Matrix4& Scaling(const Vector4& scale);
	Matrix4& Scaling3D(const Vector4& scale);

	// added transformations
	Matrix4& Translation(float tx, float ty, float tz);
	Matrix4& Shear(float shx, float shy, float shz); // Order matters  Current order x->y->z

	Matrix4& RotationX(float angle);
	Matrix4& RotationY(float angle);
	Matrix4& RotationZ(float angle);
	Matrix4& Rotation(const Vector3& axis,float angle);

	static Matrix4 sZero() { return Matrix4(); }
	static Matrix4 sIdentity() { return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 sTranslation(float x, float y, float z) { return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, x, y, z, 1.0f); }
	static Matrix4 sTranslation(const Vector3& v);
	static Matrix4 sRotationX(float rad) { return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cosf(rad), sinf(rad), 0.0f, 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 sRotationY(float rad) { return Matrix4(cosf(rad), 0.0f, -sinf(rad), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sinf(rad), 0.0f, cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 sRotationZ(float rad) { return Matrix4(cosf(rad), sinf(rad), 0.0f, 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 sScaling(float s) { return Matrix4(s, 0.0f, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 sScaling(float sx, float sy, float sz) { return Matrix4(sx, 0.0f, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 0.0f, sz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 sScaling(const Vector3& s);


	// operators
	// addition and subtraction
	Matrix4 operator+(const Matrix4& other) const;
	Matrix4& operator+=(const Matrix4& other);
	Matrix4 operator-(const Matrix4& other) const;
	Matrix4& operator-=(const Matrix4& other);

	Matrix4 operator-() const;

	// multiplication
	Matrix4& operator*=(const Matrix4& matrix);
	Matrix4 operator*(const Matrix4& matrix) const;

	// column vector multiplier
	Vector4 operator*(const Vector4& vector) const;

	Matrix4& operator*=(float scalar);
	Matrix4 operator*(float scalar) const;

	// Member
	union 
	{
		struct // row+colume subscript
		{
			float  _11, _12, _13, _14;
			float  _21, _22, _23, _24;
			float  _31, _32, _33, _34;
			float  _41, _42, _43, _44;
		};
		std::array<float, 16> mV;
	};


};



} // namespace NFGE 
} // namespace Math 

#endif // !INCLUDE_NFGE_NFGEMATH_MATRIX4_H
