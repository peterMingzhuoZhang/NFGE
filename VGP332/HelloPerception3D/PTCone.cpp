#include "PTCone.h"

void PTCone::Initialize(int row, int col, float radius, const wchar_t * vertexShader, const wchar_t * pixelShader)
{
	
	mRadius = radius;

	mMesh = NFGE::Graphics::MeshBuilder::CreateConePN(row, col, mRadius,(float)row);
	mRotation = NFGE::Math::Vector3{ 90.0f,0.0f,0.0f };
	BaseInitliaze(vertexShader, pixelShader);

	mIsInitialize = true;
}



void PTCone::OnRender()
{
	//Do some special render for PTCone.
}

void PTCone::OnUpdate(float deltaTime)
{
}

