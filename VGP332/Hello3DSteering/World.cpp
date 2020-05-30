#include "World.h"

void Hello3DSteering::World::Initialize(float width, float height, float depth)
{
	mLight.direction = NFGE::Math::Normalize({ 1.0f,-1.0f,1.0f });
	mLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	mLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mLightBuffer.Initialize();

	mWorldWidth = width;
	mWorldHeight = height;
	mWorldDepth = depth;
}

void Hello3DSteering::World::Terminate()
{
	mLightBuffer.Terminate();
}

void Hello3DSteering::World::Update()
{
}

void Hello3DSteering::World::Render()
{
	mLightBuffer.Load(mLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	for (float i = 0.0f; i <= 1280.0f; i += 80.0f)
	{
		NFGE::Graphics::SimpleDraw::AddLine({ i,0.0f, 0.0f }, { i,0.0f,720.0f }, NFGE::Graphics::Colors::DarkGray);
	}

	for (float i = 0.0f; i <= 720.0f; i += 80.0f)
	{
		NFGE::Graphics::SimpleDraw::AddLine({ 0.0f,0.0f, i }, { 1280.0f,0.0f,i}, NFGE::Graphics::Colors::DarkGray);
	}
}
