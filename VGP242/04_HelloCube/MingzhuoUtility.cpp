//Date: 2019-02-02
//By Mingzhuo Zhang

#include "MingzhuoUtility.h"


const float color_r_increase = 0.01f;
const float color_g_increase = 0.05f;
const float color_b_increase = 0.1f;

const float shiftUnit = 0.0005f;

MingzhuoUtility* MingzhuoUtility::instance = nullptr;



MingzhuoUtility::MingzhuoUtility()
	: chromColor(1.0f,1.0f,0.0f)
	, cameraTransform()
{

}


void MingzhuoUtility::Update()
{
	ChromColorUpdate();
	CameraTransformUpdate();
	Geo3DTransformUpdate();
}

void MingzhuoUtility::ChromColorUpdate()
{
	chromColor.x += color_r_increase;
	if (chromColor.x > 1.0f) chromColor.x = 0.0f;
	chromColor.y += color_g_increase;
	if (chromColor.y > 1.0f) chromColor.y = 0.0f;
	chromColor.z += color_b_increase;
	if (chromColor.z > 1.0f) chromColor.z = 0.0f;
}

void MingzhuoUtility::CameraTransformUpdate()
{
	cameraTransform.tZ = 0.0f;
	cameraTransform.tX = 0.0f;
	cameraTransform.rX = 0.0f;
	cameraTransform.rY = 0.0f;

	if (GetKeyState('W') & 0x8000)
	{
		cameraTransform.tZ = shiftUnit;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		cameraTransform.tX = -shiftUnit;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		cameraTransform.tZ = -shiftUnit;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		cameraTransform.tX = shiftUnit;
	}

	
	if (GetKeyState(VK_UP) & 0x8000) // up
	{
		cameraTransform.rX = shiftUnit* 5.0f;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)// left
	{
		cameraTransform.rY = -shiftUnit * 5.0f;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)// right
	{
		cameraTransform.rX = -shiftUnit* 5.0f;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)//down
	{
		cameraTransform.rY = shiftUnit* 5.0f;
	}
	
	/*cameraTransform.rY += shiftUnit;
	cameraTransform.rX += shiftUnit;*/
}

void MingzhuoUtility::Geo3DTransformUpdate()
{
	if ((GetKeyState('U') & 0x8000))
	{
		Geo3DTransform.scX += shiftUnit;
		Geo3DTransform.scY += shiftUnit;
		Geo3DTransform.scZ += shiftUnit;
	}
	
}



MingzhuoUtility * MingzhuoUtility::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new MingzhuoUtility();
	}
	return instance;
}

