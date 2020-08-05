#ifndef INCLUDED_NFGE_GRAPHIC_CAMERA_H
#define INCLUDED_NFGE_GRAPHIC_CAMERA_H

namespace NFGE {
namespace Graphics {

Math::Matrix4 ComputeViewMatrix(
	const Math::Vector3& right, 
	const Math::Vector3& up, 
	const Math::Vector3& look, 
	const Math::Vector3& position);
Math::Matrix4 ComputePerspectiveMatrix(float n, float f, float fov, float aspect);

// This camera assumes YAxis as up direction. You
// cannnot look straight up or straighjt down.

class Camera
{
public:
	void SetPosition(const Math::Vector3& position) { mPosition = position; }
	void SetDirection(const Math::Vector3& direction) { mDirection = NFGE::Math::Normalize(direction); }
	void SetLookUp(const Math::Vector3& target);

	// 3 degrees of freedom for translations
	void Walk(float distance);
	void Strafe(float distance);
	void Rise(float distance);

	// 2 degrees of freedom for rotation
	void Yaw(float deg);
	void Pitch(float deg);

	void SetFOV(float fov) { mFov = fov; }
	void SetNearPlane(float nearPlane) { mNearPlane = nearPlane; }
	void SetFarPlane(float farPlane) { mFarPlane = farPlane; }

	const Math::Vector3& GetPosition() const { return mPosition; }
	const Math::Vector3& GetDirection() const { return mDirection; }
	float GetFOV() const { return mFov; }
	float GetNearPlane() const { return mNearPlane; }
	float GetFarPlane() const { return mFarPlane; }

	Math::Matrix4 GetViewMatrix() const;
	Math::Matrix4 GetPerspectiveMatrix(float aspectRatio = 0.0f) const;
	Math::Matrix4 GetOrthographicMatrix(float width, float height) const;

private:
	Math::Vector3 mPosition{ 0.0f };
	Math::Vector3 mDirection{ 0.0f, 0.0f, 1.0f };

	float mFov{ 60.0f * Math::Constants::DegToRad };
	float mNearPlane{ 1.0f};
	float mFarPlane{1000.0f};
};

} // namespace NFGE
} // namespace Graphics



#endif // !INCLUDED_NFGE_GRAPHIC_CAMERA_H
