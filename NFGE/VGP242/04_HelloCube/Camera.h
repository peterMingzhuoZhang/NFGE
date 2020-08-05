#ifndef HELLOCUBE_CAMERA_H
#define HELLOCUBE_CAMERA_H


#include <NFGEMath/Inc/NFGEMath.h>

using namespace NFGE::Math;



#pragma once
class PTCamera
{
public:
	PTCamera()	{}
	~PTCamera()	{}

	// Init functions
	void Init();

	// Set functions
	void SetCamera(float lookFromX, float lookFromY, float lookFromZ, float lookAtX, float lookAtY, float lookAtZ);
	void SetFrustum(float fov, float nearPlane = -0.1f, float farPlane = 1000.0f);
	void SetNear(float nearPlane);
	void SetFar(float nearPlane);
	void SetProjection_Matrix();
	void SetConverting_Matrixs();

	// Get functions
	Vector4 GetOrigin() const { return mLookFrom; }
	float GetNear() const { return mNear; }
	float GetFar() const { return mFar; }
	Matrix4 GetWorldToView_Matrix() const;
	Matrix4 GetViewToWorld_Matrix() const;
	Matrix4 GetProjection_Matrix() const;
	Vector3 GetCameraInterest()const { return Vector3(mLookAt.x, mLookAt.y, mLookAt.z); }

	// Static get function
	static PTCamera* GetInstance();
private:
	// Singleton instance
	static PTCamera* instance;

	//camera attribute -------------
	Vector4 mLookFrom;
	Vector4 mLookAt;
	Vector4 mLookUp;
	Vector4 mLookSide;
	
	float mFOV;
	float mProjectionPlane;
	float mNear;
	float mFar;
	
	//camera assotiate matrix ------
	Matrix4 mWorldToView_matrix;
	Matrix4 mViewToWorld_matrix;
	Matrix4 mProjection_matrix;

	//camera transform matrix -----
	Matrix4 mCameraRotationMatrix;
	Matrix4 mCameraTranslationMatrix;

	//CMatrix44 mProjectionToDNC_Matric;
};



#endif // !HELLOCUBE_CAMERA_H
