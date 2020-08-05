//Date: 2019-02-02
//By Mingzhuo Zhang
#ifndef HELLOCUBE_MINGZHUOUTIL_H
#define HELLOCUBE_MINGZHUOUTIL_H

#include "NFGEMath/Inc/NFGEMath.h"
#include "Core/Inc/Core.h"

using namespace NFGE::Math;

class MingzhuoUtility
{
	
public:
	MingzhuoUtility();
	~MingzhuoUtility(){}

	//Get & set
	Vector3 GetChromColor() const { return chromColor; }
	Transform3D GetCameraTransform() const { return cameraTransform; }
	
	//functions
	void Update();
	void ChromColorUpdate();
	void CameraTransformUpdate();
	void Geo3DTransformUpdate();

	//static instance retrive
	static MingzhuoUtility* GetInstance();
	
private:
	static MingzhuoUtility* instance;

	Vector3 chromColor;
public:
	Transform3D cameraTransform;
	Transform3D Geo3DTransform;
	
};


#endif // !HELLOCUBE_MINGZHUOUTIL_H