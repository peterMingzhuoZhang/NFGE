#include "Precompiled.h"
#include "Camera.h"

#include "GraphicsSystem.h"

using namespace NFGE;
using namespace NFGE::Graphics;
using namespace NFGE::Math;

Math::Matrix4 Graphics::ComputeViewMatrix(const Math::Vector3 & r, const Math::Vector3 & u, const Math::Vector3 & l, const Math::Vector3 & p)
{
	const float dx = -Math::Dot(r, p);
	const float dy = -Math::Dot(u, p);
	const float dz = -Math::Dot(l, p);

	return {
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		dx, dy, dz, 1.0f
	};
}

Math::Matrix4 NFGE::Graphics::ComputePerspectiveMatrix(float n, float f, float fov, float aspect)
{
	const float h = 1 / tan(fov * 0.5f);
	const float w = h / aspect;
	const float zf = f;
	const float zn = n;
	const float d = zf / (zf - zn);

	return {
		w,    0.0f, 0.0f,    0.0f,
		0.0f, h,    0.0f,    0.0f,
		0.0f,0.0f, d,        1.0f,
		0.0f, 0.0f, -zn * d, 0.0f
	};

}

Math::Matrix4 Camera::GetViewMatrix() const
{
	const Math::Vector3 l = mDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross({ 0.0f, 1.0f, 0.0f }, l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
	return ComputeViewMatrix(r, u, l, mPosition);
}

Math::Matrix4 Camera::GetPerspectiveMatrix(float aspectRatio) const
{
	if (aspectRatio == 0.0f)
	{
		auto width = GraphicsSystem::Get()->GetBackBufferWidth();
		auto height = GraphicsSystem::Get()->GetBackBufferHeight();
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}
	return ComputePerspectiveMatrix(mNearPlane, mFarPlane, mFov, aspectRatio);
}

Math::Matrix4 Camera::GetOrthographicMatrix(float width, float height) const
{
	const float w = width;
	const float h = height;
	const float zf = mFarPlane;
	const float zn = mNearPlane;

	return Math::Matrix4
	{
		2.0f / w, 0.0f,0.0f,0.0f,
		0.0f, 2.0f / h, 0.0f ,0.0f,
		0.0f, 0.0f, 1.0f / (zf - zn), 0.0f,
		0.0f, 0.0f, zn / (zn - zf), 1.0f
	};
}

void NFGE::Graphics::Camera::Walk(float distance)
{
	mPosition += mDirection * distance;
}

void NFGE::Graphics::Camera::Strafe(float distance)
{
	Math::Vector3 StrafeDir = Math::Cross(Math::Vector3{ 0.0f, 1.0f, 0.0f }, mDirection);
	mPosition += Math::Normalize(StrafeDir) * distance;
}

void NFGE::Graphics::Camera::Rise(float distance)
{
	mPosition += Math::Vector3(0.0f, 1.0f, 0.0f) * distance;
}

void NFGE::Graphics::Camera::Yaw(float deg)
{
	Matrix4 rotation{};
	rotation.Identity();
	mDirection = Math::Normalize(mDirection * rotation.RotationY(deg));
}

void NFGE::Graphics::Camera::Pitch(float deg)
{
	Matrix4 rotationToAngle{};
	rotationToAngle.Identity();
	Matrix4 rotationToAxis{};
	rotationToAxis.Identity();

	Vector3 axis = Cross(Vector3::YAxis, mDirection);
	Math::Normalize(axis);
	mDirection = Math::Normalize(mDirection * rotationToAxis.Rotation(axis, deg));
}
