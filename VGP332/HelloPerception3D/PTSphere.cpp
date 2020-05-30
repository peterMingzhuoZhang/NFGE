#include "PTSphere.h"

void PTSphere::Initialize(int row, int col, float radius, const wchar_t* vertexShader, const wchar_t* pixelShader)
{
	

	mRadius = radius;
	mMesh = NFGE::Graphics::MeshBuilder::CreateSpherePN(row, col, mRadius);
	BaseInitliaze(vertexShader, pixelShader);

	mIsInitialize = true;
}



void PTSphere::OnRender()
{
	//Do some special render for PTCone.
}

void PTSphere::OnUpdate(float deltaTime)
{
}
