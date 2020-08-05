#ifndef INCLUDE_NFGE_NFGEMATH_QUATERNION_H
#define INCLUDE_NFGE_NFGEMATH_QUATERNION_H

namespace NFGE::Math
{
	struct Quaternion
	{
		float x, y, z, w;

		constexpr Quaternion() noexcept : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		constexpr Quaternion(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}

		static Quaternion Zero() { return Quaternion(0.0f, 0.0f, 0.0f, 0.0f); }
		static Quaternion Identity() { return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }

		//const static Quaternion Zero;
		//const static Quaternion Identity;

		inline Quaternion operator+(const Quaternion& rhs) const { return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
		inline Quaternion operator*(const Quaternion& rhs) const;
		inline Quaternion operator*(float s) const { return Quaternion(x * s, y * s, z * s, w * s); }
		inline Quaternion operator/(float s) const { return Quaternion(x / s, y / s, z / s, w / s); }
		inline bool operator==(const Quaternion& rhs) const { return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w); }

		static Quaternion ToQuaternion(float pitch, float yaw, float roll) // yaw (Z), pitch (Y), roll (X)
		{
			// Abbreviations for the various angular functions
			Quaternion q;
			q.x = sin(roll * 0.5f) * cos(pitch * 0.5f) * cos(yaw * 0.5f) - cos(roll * 0.5f) * sin(pitch * 0.5f) * sin(yaw * 0.5f);
			q.y = cos(roll * 0.5f) * sin(pitch * 0.5f) * cos(yaw * 0.5f) + sin(roll * 0.5f) * cos(pitch * 0.5f) * sin(yaw * 0.5f);
			q.z = cos(roll * 0.5f) * cos(pitch * 0.5f) * sin(yaw * 0.5f) - sin(roll * 0.5f) * sin(pitch * 0.5f) * cos(yaw * 0.5f);
			q.w = cos(roll * 0.5f) * cos(pitch * 0.5f) * cos(yaw * 0.5f) + sin(roll * 0.5f) * sin(pitch * 0.5f) * sin(yaw * 0.5f);
			return q;
		}
	};

	inline Quaternion Quaternion::operator*(const Quaternion& v) const
	{
		return Quaternion(w * v.x + x * v.w + y * v.z - z * v.y,
			w * v.y + y * v.w + z * v.x - x * v.z,
			w * v.z + z * v.w + x * v.y - y * v.x,
			w * v.w - x * v.x - y * v.y - z * v.z);
	}

	struct EditorQuaternion
	{
		Quaternion mQuaternion;
		Vector3 mEularAngle;
	};
}

#endif // !INCLUDE_NFGE_NFGEMATH_QUATERNION_H
