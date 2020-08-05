#include "PTDynamicCone.h"

void PTDynamicCone::Initialize(int row, int col, float radius, float height, float dynamicDuration, const wchar_t * vertexShader, const wchar_t * pixelShader)
{
	
	mRadius = radius;
	mDynamicCycleDuration = dynamicDuration;
	ASSERT(!NFGE::Math::IsZero(mDynamicCycleDuration), "[PTDynamicCone]--- Can not have dynamic duration as ZERO!");

	mMesh = NFGE::Graphics::MeshBuilder::CreateConePN(row, col, mRadius, height);
	mRotation = NFGE::Math::Vector3{ 90.0f,0.0f,0.0f };
	BaseInitliaze(vertexShader, pixelShader);
	mDynamicValueBuffer.Initialize();

	mIsInitialize = true;
}



void PTDynamicCone::OnRender()
{
	//Do some special render for PTCone.
	DynamicValueData dynamicValueData;
	dynamicValueData.currentTimePorpotion = (mDynamicCycleCounter / mDynamicCycleDuration );
	dynamicValueData.effectRange = 2.0f;
	dynamicValueData.bottomY = mMesh.vertices.back().position.y;

	mDynamicValueBuffer.Load(dynamicValueData);
	mDynamicValueBuffer.BindVS(3);

}

void PTDynamicCone::OnUpdate(float deltaTime)
{
	if (mDynamicCycleCounter >= mDynamicCycleDuration)
	{
		mDynamicCycleCounter = 0.0f;
	}
	else
	{
		mDynamicCycleCounter += deltaTime;
	}
}

void PTDynamicCone::OnTerminate()
{
	mDynamicValueBuffer.Terminate();
}

