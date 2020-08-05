#include "Precompiled.h"
#include "NFGEMath.h"

using namespace NFGE::Math;

namespace
{
	std::random_device sRandomDevice{};
	std::mt19937 sRandomEngine{ sRandomDevice() };
}

const Vector3 Vector3::XAxis{ 1.0f,0.0f,0.0f };
const Vector3 Vector3::YAxis{ 0.0f,1.0f,0.0f };
const Vector3 Vector3::ZAxis{ 0.0f,0.0f,1.0f };


Matrix4 NFGE::Math::Transform(Transform3D transformInfo, Vector3 toOrigin)
{
	Matrix4 retMatrix;
	// ToOri * Rot * Scal* She * Trans * -ToOri
	Matrix4 ToOri = Matrix4(); ToOri.Identity(); ToOri.Translation(toOrigin.x, toOrigin.y, toOrigin.z);
	Matrix4 Rot = Matrix4(); Rot.Identity(); Rot.Rotation(transformInfo.rX, transformInfo.rY, transformInfo.rZ);
	Matrix4 Scal = Matrix4(); Scal.Identity(); Scal.Scaling(Vector4(transformInfo.scX, transformInfo.scY, transformInfo.scZ, 1.0f));
	Matrix4 She = Matrix4(); She.Identity(); She.Rotation(transformInfo.shX, transformInfo.shY, transformInfo.shZ);
	Matrix4 Trans = Matrix4(); Trans.Identity(); Trans.Translation(transformInfo.tX, transformInfo.tY, transformInfo.tZ);
	Matrix4 ToOriNegi = Matrix4(); ToOriNegi.Identity(); ToOriNegi.Translation(-toOrigin.x, -toOrigin.y, -toOrigin.z);

	retMatrix = ToOri * Rot * Scal * She * Trans * ToOriNegi;

	return retMatrix;
}

Vector3 NFGE::Math::operator*(const Vector3 & vector, const Matrix4 & matrix)
{
	Vector4 ret
	(
		matrix._11 * vector.x + matrix._21 * vector.y + matrix._31 * vector.z + matrix._41 * 1.0f,
		matrix._12 * vector.x + matrix._22 * vector.y + matrix._32 * vector.z + matrix._42 * 1.0f,
		matrix._13 * vector.x + matrix._23 * vector.y + matrix._33 * vector.z + matrix._43 * 1.0f,
		matrix._14 * vector.x + matrix._24 * vector.y + matrix._34 * vector.z + matrix._44 * 1.0f
	);
	ret.WTo1();
	return Vector3(ret.x, ret.y, ret.z);
}

Vector4 NFGE::Math::operator*(const Vector4 & vector, const Matrix4 & matrix)
{
	return Vector4
	(
		matrix._11 * vector.x + matrix._21 * vector.y + matrix._31 * vector.z + matrix._41 * vector.w,
		matrix._12 * vector.x + matrix._22 * vector.y + matrix._32 * vector.z + matrix._42 * vector.w,
		matrix._13 * vector.x + matrix._23 * vector.y + matrix._33 * vector.z + matrix._43 * vector.w,
		matrix._14 * vector.x + matrix._24 * vector.y + matrix._34 * vector.z + matrix._44 * vector.w
	);
	/*return Vector4
	(
		matrix._11 * vector.x + matrix._12 * vector.y + matrix._13 * vector.z + matrix._14 * vector.w,
		matrix._21 * vector.x + matrix._22 * vector.y + matrix._23 * vector.z + matrix._24 * vector.w,
		matrix._31 * vector.x + matrix._32 * vector.y + matrix._33 * vector.z + matrix._34 * vector.w,
		matrix._41 * vector.x + matrix._42 * vector.y + matrix._43 * vector.z + matrix._44 * vector.w
	);*/
}

Matrix4  NFGE::Math::operator*(float scalar, const Matrix4 & matrix)
{
	Matrix4 retMatrix;
	for (size_t i = 0; i < 16; i++)
	{
		retMatrix.mV[i] = matrix.mV[i] * scalar;
	}
	return retMatrix;
}

Vector3 NFGE::Math::TransfromCoord(const Vector3 & vector, const Matrix4 & matrix)
{
	return Vector3
	(
		matrix._11 * vector.x + matrix._21 * vector.y + matrix._31 * vector.z + matrix._41 * 1.0f,
		matrix._12 * vector.x + matrix._22 * vector.y + matrix._32 * vector.z + matrix._42 * 1.0f,
		matrix._13 * vector.x + matrix._23 * vector.y + matrix._33 * vector.z + matrix._43 * 1.0f
		
	);
}

Vector3 NFGE::Math::TransfromNormal(const Vector3 & vector, const Matrix4 & matrix)
{
	return Vector3
	(
		matrix._11 * vector.x + matrix._21 * vector.y + matrix._31 * vector.z,
		matrix._12 * vector.x + matrix._22 * vector.y + matrix._32 * vector.z,
		matrix._13 * vector.x + matrix._23 * vector.y + matrix._33 * vector.z

	);
}

Quaternion NFGE::Math::RotMatToQuaternion(const Matrix4 & rotMat)
{
	float m00 = rotMat._11;
	float m01 = rotMat._12;
	float m02 = rotMat._13;
	float m10 = rotMat._21;
	float m11 = rotMat._22;
	float m12 = rotMat._23;
	float m20 = rotMat._31;
	float m21 = rotMat._32;
	float m22 = rotMat._33;

	//float qw, qx, qy, qz;
	//float tr = m00 + m11 + m22;

	//if (tr > 0) {
	//	float S = sqrtf(tr + 1.0f) * 2; // S=4*qw 
	//	qw = 0.25f * S;
	//	qx = (m21 - m12) / S;
	//	qy = (m02 - m20) / S;
	//	qz = (m10 - m01) / S;
	//}
	//else if ((m00 > m11)&(m00 > m22)) {
	//	float S = sqrtf(1.0f + m00 - m11 - m22) * 2; // S=4*qx 
	//	qw = (m21 - m12) / S;
	//	qx = 0.25f * S;
	//	qy = (m01 + m10) / S;
	//	qz = (m02 + m20) / S;
	//}
	//else if (m11 > m22) {
	//	float S = sqrtf(1.0f + m11 - m00 - m22) * 2; // S=4*qy
	//	qw = (m02 - m20) / S;
	//	qx = (m01 + m10) / S;
	//	qy = 0.25f * S;
	//	qz = (m12 + m21) / S;
	//}
	//else {
	//	float S = sqrtf(1.0f + m22 - m00 - m11) * 2; // S=4*qz
	//	qw = (m10 - m01) / S;
	//	qx = (m02 + m20) / S;
	//	qy = (m12 + m21) / S;
	//	qz = 0.25f * S;
	//}

	//return Quaternion{qx, qy, qz, qw};
	Quaternion q;
	float trace = m00 + m11 + m22; // I removed + 1.0f; see discussion with Ethan
	if (trace > 0) {// I changed M_EPSILON to 0
		float s = 0.5f / sqrtf(trace + 1.0f);
		q.w = 0.25f / s;
		q.x = (m21 - m12) * s;
		q.y = (m02 - m20) * s;
		q.z = (m10 - m01) * s;
	}
	else {
		if (m00 > m11 && m00 > m22) {
			float s = 2.0f * sqrtf(1.0f + m00 - m11 - m22);
			q.w = (m21 - m12) / s;
			q.x = 0.25f * s;
			q.y = (m01 + m10) / s;
			q.z = (m02 + m20) / s;
		}
		else if (m11 > m22) {
			float s = 2.0f * sqrtf(1.0f + m11 - m00 - m22);
			q.w = (m02 - m20) / s;
			q.x = (m01 + m10) / s;
			q.y = 0.25f * s;
			q.z = (m12 + m21) / s;
		}
		else {
			float s = 2.0f * sqrtf(1.0f + m22 - m00 - m11);
			q.w = (m10 - m01) / s;
			q.x = (m02 + m20) / s;
			q.y = (m12 + m21) / s;
			q.z = 0.25f * s;
		}
	}
	return q;
}

Quaternion NFGE::Math::QuaternionLookRotation(const Vector3 & forward, const Vector3 & up)
{
	Vector3 z = Normalize(forward);
	Vector3 x = Normalize(Cross(up, z));
	Vector3 y = Normalize(Cross(z, x));
	Matrix4 mat
	(
		x.x, x.y, x.z, 0.0f,
		y.x, y.y, y.z, 0.0f,
		z.x, z.y, z.z, 0.0f,
		0.0f,0.0f,0.0f, 1.0f
	);

	return RotMatToQuaternion(NFGE::Math::Transpose(mat));

	//Vector3 z = Normalize(forward);
	//Vector3 x = Normalize(Cross(up, z));
	//Vector3 y = Normalize(Cross(z, x));
	//Matrix4 mat
	//(
	//	x.x, x.y, x.z, 0.0f,
	//	y.x, y.y, y.z, 0.0f,
	//	z.x, z.y, z.z, 0.0f,
	//	0.0f, 0.0f, 0.0f, 1.0f
	//);

	////// TODO: Need matrix to quaternion
	////// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/

	//float qw = (sqrtf(0.5f*(1 + mat._11 + mat._22 + mat._33)));
	//float fourXqw = 4 * qw;
	//return Quaternion((mat._32 - mat._23) / (fourXqw), (mat._13 - mat._31) / (fourXqw), (mat._12 - mat._21) / (fourXqw), qw);

}

Quaternion NFGE::Math::QuaternionRotationAxis(const Vector3 & axis, float rad)
{
	const float c = cos(rad * 0.5f);
	const float s = sin(rad * 0.5f);
	const Math::Vector3 a = Math::Normalize(axis);
	return Quaternion(a.x * s, a.y * s, a.z * s, c);
}

Quaternion NFGE::Math::QuaternionFromTo(const Vector3 & from, const Vector3 & to)
{
	Vector3 up = Cross(from, to);
	float theta = acosf(Dot(Normalize(from), Normalize(to)));

	return QuaternionRotationAxis(up, theta);
}

Matrix4 NFGE::Math::MatrixRotationAxis(const Vector3 & axis, float rad)
{
	const Vector3 u = Normalize(axis);
	const float x = u.x;
	const float y = u.y;
	const float z = u.z;
	const float s = sin(rad);
	const float c = cos(rad);

	return Matrix4
	(
		c + (x * x * (1.0f - c)),
		x * y * (1.0f - c) + z * s,
		x * z * (1.0f - c) - y * s,
		0.0f,

		x * y * (1.0f - c) - z * s,
		c + (y * y * (1.0f - c)),
		y * z * (1.0f - c) + x * s,
		0.0f,

		x * z * (1.0f - c) + y * s,
		y * z * (1.0f - c) - x * s,
		c + (z * z * (1.0f - c)),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	);
}

Matrix4 NFGE::Math::MatrixRotationQuaternion(const Quaternion & q)
{
	return Matrix4
	(
		1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
		(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
		(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
		0.0f,

		(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
		(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
		0.0f,

		(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
		(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	);
}

Vector3 NFGE::Math::GetEular(const Quaternion & quaternion)
{
	float t0 = 2.0f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z);
	float t1 = 1.0 - 2.0 * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);
	float roll = atan2(t0, t1);
	float t2 = +2.0 * (quaternion.w * quaternion.y - quaternion.z * quaternion.x);
	if (t2 > 1.0f)
		t2 = 1.0;
	if (t2 < -1.0f)
		t2 = -1.0;
	float pitch = asin(t2);
	float t3 = +2.0 * (quaternion.w * quaternion.z + quaternion.x * quaternion.y);
	float t4 = +1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
	float yaw = atan2(t3, t4);
	
	return Vector3(pitch, yaw, roll);
}

//----------------------------------------------------------------------------------------------------

Quaternion NFGE::Math::Slerp(Quaternion q0, Quaternion q1, float t)
{
	// Find the dot product
	float dot = (q0.x * q1.x) + (q0.y * q1.y) + (q0.z * q1.z) + (q0.w * q1.w);

	// Determine the direction of the rotation.
	if (dot < 0.0f)
	{
		dot = -dot;
		q1.x = -q1.x;
		q1.y = -q1.y;
		q1.z = -q1.z;
		q1.w = -q1.w;
	}
	else if (dot > 0.999f)
	{
		return Normalize(Lerp(q0, q1, t));
	}

	float theta = acosf(dot);
	float sintheta = sinf(theta);
	float scale0 = sinf(theta * (1.0f - t)) / sintheta;
	float scale1 = sinf(theta * t) / sintheta;

	// Perform the slerp
	return Quaternion
	(
		(q0.x * scale0) + (q1.x * scale1),
		(q0.y * scale0) + (q1.y * scale1),
		(q0.z * scale0) + (q1.z * scale1),
		(q0.w * scale0) + (q1.w * scale1)
	);
}

//----------------------------------------------------------------------------------------------------

NFGE::Math::Vector3 NFGE::Math::GetBarycentric(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& point)
{
	NFGE::Math::Vector3 lambda;
	float denom = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
	lambda.x = ((b.y - c.y) * (point.x - c.x) + (c.x - b.x) * (point.y - c.y)) / denom;
	lambda.y = ((c.y - a.y) * (point.x - c.x) + (a.x - c.x) * (point.y - c.y)) / denom;
	lambda.z = 1.0f - lambda.x - lambda.y;
	return lambda;
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::PointInTriangle(const Vector2& point, const Vector2& a, const Vector2& b, const Vector2& c)
{
	NFGE::Math::Vector3 barycentric = GetBarycentric(a, b, c, point);
	return
		barycentric.x >= 0.0f && barycentric.x <= 1.0f &&
		barycentric.y >= 0.0f && barycentric.y <= 1.0f &&
		barycentric.z >= 0.0f && barycentric.z <= 1.0f;
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::PointInRect(const Vector2& point, const Rect& rect)
{
	if (point.x > rect.right || point.x < rect.left ||
		point.y > rect.bottom || point.y < rect.top)
	{
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::PointInCircle(const Vector2& point, const Circle& circle)
{
	const Vector2 centerToPoint = point - circle.center;
	const float distSqr = Dot(centerToPoint, centerToPoint);
	const float radiusSqr = circle.radius * circle.radius;
	return distSqr < radiusSqr;
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::PointInSphere(const Vector3& point, const Sphere& sphere)
{
	const Vector3 centerToPoint = point - sphere.center;
	const float distSqr = Dot(centerToPoint, centerToPoint);
	const float radiusSqr = sphere.radius * sphere.radius;
	return distSqr < radiusSqr;
}
//----------------------------------------------------------------------------------------------------

bool NFGE::Math::Intersect(const LineSegment& a, const LineSegment& b)
{
	// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

	float ua = ((a.to.x - a.from.x) * (b.from.y - a.from.y)) - ((a.to.y - a.from.y) * (b.from.x - a.from.x));
	float ub = ((b.to.x - b.from.x) * (b.from.y - a.from.y)) - ((b.to.y - b.from.y) * (b.from.x - a.from.x));
	float denom = ((a.to.y - a.from.y) * (b.to.x - b.from.x)) - ((a.to.x - a.from.x) * (b.to.y - b.from.y));

	// First check for special cases
	if (denom == 0.0f)
	{
		if (ua == 0.0f && ub == 0.0f)
		{
			// The line segments are the same
			return true;
		}
		else
		{
			// The line segments are parallel
			return false;
		}
	}

	ua /= denom;
	ub /= denom;

	if (ua < 0.0f || ua > 1.0f || ub < 0.0f || ub > 1.0f)
	{
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------- Mingzhuo zhang Editing --------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------|||||||||||||||||||||||||||----------------------------------------------------------------------
//------------------------------------vvvvvvvvvvvvvvvvvvvvvvvvvvv----------------------------------------------------------------------
bool NFGE::Math::Intersect(const LineSegment & a, const LineSegment & b, Vector2 & intersectPoint)
{
	// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

	float ua = ((a.to.x - a.from.x) * (b.from.y - a.from.y)) - ((a.to.y - a.from.y) * (b.from.x - a.from.x));
	float ub = ((b.to.x - b.from.x) * (b.from.y - a.from.y)) - ((b.to.y - b.from.y) * (b.from.x - a.from.x));
	float denom = ((a.to.y - a.from.y) * (b.to.x - b.from.x)) - ((a.to.x - a.from.x) * (b.to.y - b.from.y));

	// First check for special cases
	if (denom == 0.0f)
	{
		if (ua == 0.0f && ub == 0.0f)
		{
			// The line segments are the same
			return true;
		}
		else
		{
			// The line segments are parallel
			return false;
		}
	}

	ua /= denom;
	ub /= denom;

	if (ua < 0.0f || ua > 1.0f || ub < 0.0f || ub > 1.0f)
	{
		return false;
	}

	intersectPoint = b.from + (b.from - b.to) * -ua;
	return true;
}

//------------------------------------^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^--------------------------------------------------------------
//------------------------------------|||||||||||||||||||||||||||||||||||--------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------- End Edite -------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::Intersect(const Circle& c0, const Circle& c1)
{
	const float fRadii = c0.radius + c1.radius;
	const float fRadiiSquared = fRadii * fRadii;
	const float fDistanceSquared = DistanceSqr(c0.center, c1.center);
	return (fDistanceSquared < fRadiiSquared);
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::Intersect(const Rect& r0, const Rect& r1)
{
	if (r0.left > r1.right)
		return false;
	else if (r0.top > r1.bottom)
		return false;
	else if (r0.right < r1.left)
		return false;
	else if (r0.bottom < r1.top)
		return false;
	return true;
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::Intersect(const LineSegment& l, const Circle& c)
{
	return Intersect(c, l);
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::Intersect(const Circle& c, const LineSegment& l, Vector2* closestPoint)
{
	Vector2 startToCenter = c.center - l.from;
	Vector2 startToEnd = l.to - l.from;
	float len = Magnitude(startToEnd);
	Vector2 dir = startToEnd / len;

	// Find the closest point to the line segment
	float projection = Dot(startToCenter, dir);
	Vector2 point;
	if (projection > len)
	{
		point = l.to;
	}
	else if (projection < 0.0f)
	{
		point = l.from;
	}
	else
	{
		point = l.from + (dir * projection);
	}

	// Check if the closest point is within the circle
	if (!PointInCircle(point, c))
		return false;

	if (closestPoint)
		*closestPoint = point;

	return true;
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::Intersect(const Circle& c, const Rect& r)
{
	return Intersect(r, c);
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::Intersect(const Rect& r, const Circle& c)
{
	Vector2 closestPoint;
	closestPoint.x = Clamp(c.center.x, r.left, r.right);
	closestPoint.y = Clamp(c.center.y, r.top, r.bottom);

	const float distance = Distance(closestPoint, c.center);
	if (distance > c.radius)
	{
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::Intersect(const Ray& ray, const Vector3& a, const Vector3& b, const Vector3& c, float& distance)
{
	// Reference: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

	// Find vectors for two edges sharing V1
	Vector3 e1 = b - a;
	Vector3 e2 = c - a;

	// Begin calculating determinant - also used to calculate u parameter
	Vector3 P = Cross(ray.dir, e2);

	// If determinant is near zero, ray lies in plane of triangle
	float det = Dot(e1, P);
	// NOT CULLING
	if (IsZero(det))
	{
		return false;
	}

	float inv_det = 1.0f / det;

	// Calculate distance from V1 to ray origin
	Vector3 T = ray.org - a;

	// Calculate u parameter and test bound
	float u = Dot(T, P) * inv_det;

	// The intersection lies outside of the triangle
	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	// Prepare to test v parameter
	Vector3 Q = Cross(T, e1);

	// Calculate V parameter and test bound
	float v = Dot(ray.dir, Q) * inv_det;

	// The intersection lies outside of the triangle
	if (v < 0.0f || u + v  > 1.0f)
	{
		return false;
	}

	// Ray intersection
	float t = Dot(e2, Q) * inv_det;
	if (t <= 0.0f)
	{
		// No hit, no win
		return false;
	}

	distance = t;
	return true;
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::Intersect(const Ray& ray, const Plane& plane, float& distance)
{
	const float orgDotN = Dot(ray.org, plane.n);
	const float dirDotN = Dot(ray.dir, plane.n);

	// Check if ray is parallel to the plane
	if (IsZero(dirDotN))
	{
		if (IsZero(orgDotN - plane.d))
		{
			distance = 0.0f;
			return true;
		}
		else
		{
			return false;
		}
	}

	// Compute distance
	distance = (plane.d - orgDotN) / dirDotN;
	return true;
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::Intersect(const Ray& ray, const AABB& aabb, float& distEntry, float& distExit)
{
	// https://truesculpt.googlecode.com/hg-history/Release%25200.8/Doc/ray_box_intersect.pdf

	// Returns true if ray intersects bounding box
	// Sets d1 to entry distance, d2 to exit distance (along ray.dir)

	Vector3 boxMin = aabb.center - aabb.extend;
	Vector3 boxMax = aabb.center + aabb.extend;
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	float divx = 1.0f / ray.dir.x;
	float divy = 1.0f / ray.dir.y;
	float divz = 1.0f / ray.dir.z;

	if (divx >= 0.0f)
	{
		tmin = (boxMin.x - ray.org.x) * divx;
		tmax = (boxMax.x - ray.org.x) * divx;
	}
	else
	{
		tmin = (boxMax.x - ray.org.x) * divx;
		tmax = (boxMin.x - ray.org.x) * divx;
	}
	if (divy >= 0.0f)
	{
		tymin = (boxMin.y - ray.org.y) * divy;
		tymax = (boxMax.y - ray.org.y) * divy;
	}
	else
	{
		tymin = (boxMax.y - ray.org.y) * divy;
		tymax = (boxMin.y - ray.org.y) * divy;
	}

	if ((tmin > tymax) || (tymin > tmax))
	{
		return false;
	}

	if (tymin > tmin)
	{
		tmin = tymin;
	}

	if (tymax < tmax)
	{
		tmax = tymax;
	}

	if (divz >= 0.0f)
	{
		tzmin = (boxMin.z - ray.org.z) * divz;
		tzmax = (boxMax.z - ray.org.z) * divz;
	}
	else
	{
		tzmin = (boxMax.z - ray.org.z) * divz;
		tzmax = (boxMin.z - ray.org.z) * divz;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
	{
		return false;
	}

	if (tzmin > tmin)
	{
		tmin = tzmin;
	}

	if (tzmax < tmax)
	{
		tmax = tzmax;
	}

	distEntry = tmin;
	distExit = tmax;
	return true;
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::Intersect(const Ray& ray, const OBB& obb, float& distEntry, float& distExit)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Translation(obb.center.x, obb.center.y, obb.center.z);
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the ray into the OBB's local space
	Vector3 org = TransformCoord(ray.org, matWorldInv);
	Vector3 dir = TransformNormal(ray.dir, matWorldInv);

	AABB aabb(Vector3::Zero(), obb.extend);
	return Math::Intersect(Ray(org, dir), aabb, distEntry, distExit);
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::Intersect(const Vector3& point, const AABB& aabb)
{
	const Vector3 test = point - aabb.center;
	if (abs(test.x) > aabb.extend.x) return false;
	if (abs(test.y) > aabb.extend.y) return false;
	if (abs(test.z) > aabb.extend.z) return false;
	return true;
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::Intersect(const Vector3& point, const OBB& obb)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Translation(obb.center.x, obb.center.y, obb.center.z);
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the point into the OBB's local space
	Vector3 localPoint = TransformCoord(point, matWorldInv);
	AABB aabb(Vector3::Zero(), obb.extend);

	// Test against local AABB
	return Math::Intersect(localPoint, aabb);
}

bool NFGE::Math::Intersect(const AABB & aabb1, const AABB & aabb2)
{
	const Vector3 half = aabb1.extend * 0.5f;
	const Vector3 bl_f = { aabb1.center.x - half.x, aabb1.center.y - half.y, aabb1.center.z - half.z };
	const Vector3 br_f = { aabb1.center.x + half.x, aabb1.center.y - half.y, aabb1.center.z - half.z };
	const Vector3 tl_f = { aabb1.center.x - half.x, aabb1.center.y + half.y, aabb1.center.z - half.z };
	const Vector3 tr_f = { aabb1.center.x + half.x, aabb1.center.y + half.y, aabb1.center.z - half.z };
	const Vector3 bl_b = { aabb1.center.x - half.x, aabb1.center.y - half.y, aabb1.center.z + half.z };
	const Vector3 br_b = { aabb1.center.x + half.x, aabb1.center.y - half.y, aabb1.center.z + half.z };
	const Vector3 tl_b = { aabb1.center.x - half.x, aabb1.center.y + half.y, aabb1.center.z + half.z };
	const Vector3 tr_b = { aabb1.center.x + half.x, aabb1.center.y + half.y, aabb1.center.z + half.z };
	
	if (Intersect(bl_f, aabb2)) return true;
	if (Intersect(br_f, aabb2)) return true;
	if (Intersect(tl_f, aabb2)) return true;
	if (Intersect(tr_f, aabb2)) return true;
	if (Intersect(bl_b, aabb2)) return true;
	if (Intersect(br_b, aabb2)) return true;
	if (Intersect(tl_b, aabb2)) return true;
	if (Intersect(tr_b, aabb2)) return true;
	return false;
}

//----------------------------------------------------------------------------------------------------

void NFGE::Math::GetCorners(const OBB& obb, std::vector<Vector3>& corners)
{
	// Compute the local-to-world matrices
	Matrix4 matTrans = Translation(obb.center.x, obb.center.y, obb.center.z);
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;

	// Create a local AABB
	corners.clear();
	corners.push_back(Vector3(-obb.extend.x, -obb.extend.y, -obb.extend.z));
	corners.push_back(Vector3(-obb.extend.x, -obb.extend.y, obb.extend.z));
	corners.push_back(Vector3(obb.extend.x, -obb.extend.y, obb.extend.z));
	corners.push_back(Vector3(obb.extend.x, -obb.extend.y, -obb.extend.z));
	corners.push_back(Vector3(-obb.extend.x, obb.extend.y, -obb.extend.z));
	corners.push_back(Vector3(-obb.extend.x, obb.extend.y, obb.extend.z));
	corners.push_back(Vector3(obb.extend.x, obb.extend.y, obb.extend.z));
	corners.push_back(Vector3(obb.extend.x, obb.extend.y, -obb.extend.z));

	// Transform AABB into world space to form the OBB
	const uint32_t kNumCorners = (uint32_t)corners.size();
	for (uint32_t i = 0; i < kNumCorners; ++i)
	{
		corners[i] = TransformCoord(corners[i], matWorld);
	}
}

//----------------------------------------------------------------------------------------------------

bool NFGE::Math::GetContactPoint(const Ray& ray, const OBB& obb, Vector3& point, Vector3& normal)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Translation(obb.center.x, obb.center.y, obb.center.z);
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the ray into the OBB's local space
	Vector3 org = TransformCoord(ray.org, matWorldInv);
	Vector3 dir = TransformNormal(ray.dir, matWorldInv);
	Ray localRay(org, dir);

	Plane planes[] =
	{
		Plane(0.0f,  0.0f, -1.0f, obb.extend.z),
		Plane(0.0f,  0.0f,  1.0f, obb.extend.z),
		Plane(0.0f, -1.0f,  0.0f, obb.extend.y),
		Plane(0.0f,  1.0f,  0.0f, obb.extend.y),
		Plane(-1.0f,  0.0f,  0.0f, obb.extend.x),
		Plane(1.0f,  0.0f,  0.0f, obb.extend.x)
	};

	uint32_t numIntersections = 0;
	for (uint32_t i = 0; i < 6; ++i)
	{
		const float d = Dot(org, planes[i].n);
		if (d > planes[i].d)
		{
			float distance = 0.0f;
			if (Intersect(localRay, planes[i], distance) && distance >= 0.0f)
			{
				Vector3 pt = org + (dir * distance);
				if (abs(pt.x) <= obb.extend.x + 0.01f &&
					abs(pt.y) <= obb.extend.y + 0.01f &&
					abs(pt.z) <= obb.extend.z + 0.01f)
				{
					point = pt;
					normal = planes[i].n;
					++numIntersections;
				}
			}
		}
	}

	if (numIntersections == 0)
	{
		return false;
	}

	point = TransformCoord(point, matWorld);
	normal = TransformNormal(normal, matWorld);
	return true;
}

//----------------------------------------------------------------------------------------------------

Vector3 NFGE::Math::GetClosestPoint(const Ray& ray, const Vector3& point)
{
	Vector3 orgToPoint = point - ray.org;
	float d = Dot(orgToPoint, ray.dir);
	return ray.org + (ray.dir * d);
}

//----------------------------------------------------------------------------------------------------

Vector3 NFGE::Math::Mean(const Vector3* v, uint32_t count)
{
	Vector3 mean(0.0f, 0.0f, 0.0f);
	for (uint32_t i = 0; i < count; ++i)
	{
		mean += v[i];
	}
	return mean / (float)count;
}

int NFGE::Math::Random()
{
	return std::uniform_int_distribution<>{ 0, (std::numeric_limits<int>::max)() }(sRandomEngine);
}

//----------------------------------------------------------------------------------------------------

int NFGE::Math::Random(int min, int max)
{
	return std::uniform_int_distribution<>{ min, max }(sRandomEngine);
}

//----------------------------------------------------------------------------------------------------

float NFGE::Math::RandomFloat()
{
	return std::uniform_real_distribution<float>{ 0, 1.0f }(sRandomEngine);
}

//----------------------------------------------------------------------------------------------------

float NFGE::Math::RandomFloat(float min, float max)
{
	return std::uniform_real_distribution<float>{ min, max }(sRandomEngine);
}

//----------------------------------------------------------------------------------------------------

Vector2 NFGE::Math::RandomVector2()
{
	return Vector2
	(
		RandomFloat(),
		RandomFloat()
	);
}

//----------------------------------------------------------------------------------------------------

Vector2 NFGE::Math::RandomVector2(const Vector2& min, const Vector2& max)
{
	return Vector2
	(
		RandomFloat(min.x, max.x),
		RandomFloat(min.y, max.y)
	);
}

//----------------------------------------------------------------------------------------------------

Vector2 NFGE::Math::RandomUnitCircle(bool normalized)
{
	Vector2 randVal{
		RandomFloat(-1.0f, 1.0f) + 0.001f,
		RandomFloat(-1.0f, 1.0f)
	};
	if (normalized)
		return Normalize(randVal);
	return randVal;
}

//----------------------------------------------------------------------------------------------------

Vector3 NFGE::Math::RandomVector3()
{
	return Vector3
	(
		RandomFloat(),
		RandomFloat(),
		RandomFloat()
	);
}

//----------------------------------------------------------------------------------------------------

Vector3 NFGE::Math::RandomVector3(const Vector3& min, const Vector3& max)
{
	return Vector3
	(
		RandomFloat(min.x, max.x),
		RandomFloat(min.y, max.y),
		RandomFloat(min.z, max.z)
	);
}

//----------------------------------------------------------------------------------------------------

Vector3 NFGE::Math::RandomUnitSphere()
{
	return Normalize(Vector3(
		RandomFloat(-1.0f, 1.0f) + 0.001f,
		RandomFloat(-1.0f, 1.0f),
		RandomFloat(-1.0f, 1.0f))
	);
}

//----------------------------------------------------------------------------------------------------

float NFGE::Math::Ease::EaseNone(float t)
{
	return Math::Interpolation::LinearSpline(0.0f, 1.0f, t);
}

float NFGE::Math::Ease::EaseInSine(float t)
{
	// -----------------------------------------
	// --- https://easings.net/en#easeInSine ---
	// -----------------------------------------
	return Math::Interpolation::CubicBezier(0.47f, 0.0f, 0.745f, 0.715f, t);
}

float NFGE::Math::Ease::EaseOutCirc(float t)
{
	// -----------------------------------------
	// --- https://easings.net/en#easeOutCirc ---
	// -----------------------------------------
	return Math::Interpolation::CubicBezier(0.075f, 0.82f, 0.165f, 1.0f, t);
}

float NFGE::Math::Ease::EaseOutMushroom(float t)
{
	// -----------------------------------------
	// --- https://easings.net/en#easeOutCirc ---
	// -----------------------------------------
	return Math::Interpolation::CubicBezier(.86f, 0.0f, 0.07f, 1.0f, t);
}