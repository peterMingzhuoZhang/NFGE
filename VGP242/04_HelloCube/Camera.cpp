
#include "Camera.h"
#include "MingzhuoUtility.h"

extern int windowWidth;
extern int windowHeight;

PTCamera* PTCamera::instance = nullptr;


void PTCamera::Init()
{
	
	if (mProjectionPlane == 0.0f)
	{
		SetFrustum(90.0f, 0.1f, 1000.0f);
	}
	
	SetConverting_Matrixs();
	SetProjection_Matrix();
}

void PTCamera::SetCamera(float lookFromX, float lookFromY, float lookFromZ, float lookAtX, float lookAtY, float lookAtZ)
{
	mLookUp = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	if (::IsZero( mLookFrom.x) && ::IsZero(mLookFrom.y) && ::IsZero(mLookFrom.z))
	{
		mLookFrom = Vector4(lookFromX, lookFromY, lookFromZ, 1.0f);
		Vector4 LookAt_point = Vector4(lookAtX, lookAtY, lookAtZ, 1.0f);
		mLookAt = (LookAt_point - mLookFrom);
		mLookAt.w = 1.0f;
	}
	
	

	
	// Creating a camera matrix with rotation
	Transform3D transform = MingzhuoUtility::GetInstance()->GetCameraTransform();
	mCameraRotationMatrix.Identity(); 
	mCameraRotationMatrix.Rotation(transform.rX, transform.rY, transform.rZ);
	
	//
	mLookAt =  mCameraRotationMatrix  * mLookAt;
	mLookAt.WTo1();
	mLookAt.Normalize3D();
	mLookSide = mLookUp.Cross3D(mLookAt);
	mLookSide.WTo1();
	mLookSide.Normalize3D();
	
	// Calculate lookUp
	mLookUp = mLookAt.Cross3D(mLookSide);
	mLookUp.WTo1();
	mLookUp.Normalize3D();

	// 
	mCameraTranslationMatrix.Identity();
	mCameraTranslationMatrix.Translation(
		transform.tZ * mLookAt.x + transform.tX * mLookSide.x,
		transform.tZ * mLookAt.y + transform.tX * mLookSide.y,
		transform.tZ * mLookAt.z + transform.tX * mLookSide.z
	);

	mLookFrom = mLookFrom * mCameraTranslationMatrix;
	mLookFrom.WTo1();
}

PTCamera * PTCamera::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new PTCamera();
	}
	return instance;
}


void PTCamera::SetFrustum(float fov, float nearPlane, float farPlane)
{
	ASSERT(fov != 0.0f, "[Camera] can not set fov to 0");
	if (mFOV != fov)
	{
		mProjectionPlane = 1 / tanf(DEG2RAD(fov) * 0.5f);
	}

	mNear = mProjectionPlane - nearPlane;
	mFar = mProjectionPlane + farPlane;
}

void PTCamera::SetNear(float nearPlane)
{
	//ASSERT(mProjectionPlane != 0.0f);
	mNear = mProjectionPlane - nearPlane;
}

void PTCamera::SetFar(float farPlane)
{
	//ASSERT(mProjectionPlane != 0.0f);
	mFar = mProjectionPlane + farPlane;
}

void PTCamera::SetProjection_Matrix()
{
	float ar =static_cast<float>(static_cast<float>(windowHeight) / static_cast<float>(windowWidth));

	mProjection_matrix = Matrix4
	(
		mProjectionPlane / ar,	0.0f,				0.0f,										0.0f,
		0.0f,					mProjectionPlane,	0.0f,										0.0f,
		0.0f,					0.0f,				(mFar + mNear) /( mFar - mNear),			1.0f,
		0.0f,					0.0f,				(-2.0f * mNear * mFar) / (mFar - mNear),	0.0f
	);

	/*mProjection_matrix = Matrix4
	(
		mProjectionPlane / ar,	0.0f,				0.0f,										0.0f,
		0.0f,					mProjectionPlane,	0.0f,			
		0.0f,
		0.0f,					0.0f,				2.0f / (mFar - mNear),			1.0f,
		0.0f,					0.0f,				-(mNear + mFar) / (mFar - mNear),	0.0f
	);*/
	
}

void PTCamera::SetConverting_Matrixs()
{
	mViewToWorld_matrix = Matrix4
	(
		mLookSide.x,	mLookSide.y,	mLookSide.z, 0.0f,
		mLookUp.x,		mLookUp.y,		mLookUp.z, 0.0f,
		mLookAt.x,		mLookAt.y,		mLookAt.z, 0.0f,
		mLookFrom.x, mLookFrom.y, mLookFrom.z, 1.0f
	);

	/*mWorldToView_matrix = Matrix4
	(
		mLookSide.x,	mLookUp.x,		mLookAt.x,		0.0f,
		mLookSide.y,	mLookUp.y,		mLookAt.y,		0.0f,
		mLookSide.z,	mLookUp.z,		mLookAt.z,		0.0f,
		-mLookFrom.x,	-mLookFrom.y,	-mLookFrom.z,	1.0f
	);*/
	//Transform3D transform = CMingzhuoUtility::GetInstance()->GetCameraTransform();
	//Matrix4 inverseCameraTranslationMatrix; inverseCameraTranslationMatrix.Identity(); inverseCameraTranslationMatrix.Translation
	//(
	//	-(transform.tZ * mLookAt.x),// + transform.tX * mLookSide.x, 
	//	-(transform.tZ * mLookAt.y),// + transform.tX * mLookSide.y,
	//	-(transform.tZ * mLookAt.z)// + transform.tX * mLookSide.z
	//);
	//mWorldToView_matrix = mWorldToView_matrix /** inverseCameraTranslationMatrix*/;


	mWorldToView_matrix = Matrix4
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-mLookFrom.x, -mLookFrom.y, -mLookFrom.z, 1.0f
	);

	//Matrix4 toCameraOri; toCameraOri.Identity(); toCameraOri.Translation(-mLookFrom.x, -mLookFrom.y, -mLookFrom.z);
	//Matrix4 comeFromCameraOri; comeFromCameraOri.Identity(); comeFromCameraOri.Translation(mLookFrom.x, mLookFrom.y, mLookFrom.z);
	Matrix4 rotationTemp = Matrix4
	(
	mLookSide.x,	mLookUp.x,		mLookAt.x,		0.0f,
	mLookSide.y,	mLookUp.y,		mLookAt.y,		0.0f,
	mLookSide.z,	mLookUp.z,		mLookAt.z,		0.0f,
	0.0f,	0.0f,	0.0f,	1.0f
	);

	mWorldToView_matrix = mWorldToView_matrix * rotationTemp;

}

Matrix4 PTCamera::GetWorldToView_Matrix() const
{
	return mWorldToView_matrix;
}

Matrix4 PTCamera::GetViewToWorld_Matrix() const
{
	return mViewToWorld_matrix;
}

Matrix4 PTCamera::GetProjection_Matrix() const
{
	return mProjection_matrix;
}
