#include "AIWorld_3D.h"

void AIWorld_3D::Initialize(float width, float height, float depth)
{
	mLight.direction = NFGE::Math::Normalize({ 1.0f,-1.0f,1.0f });
	mLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	mLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mLightBuffer.Initialize();

	mWorldWidth = width;
	mWorldHeight = height;
	mWorldDepth = depth;

	world.AddWall3D(NFGE::Math::Plane{  1.0f,0.0f,0.0f , 0.0f });
	world.AddWall3D(NFGE::Math::Plane{ -1.0f,0.0f,0.0f , -mWorldWidth});
	world.AddWall3D(NFGE::Math::Plane{ 0.0f,1.0f,0.0f , 0.0f });
	world.AddWall3D(NFGE::Math::Plane{ 0.0f,-1.0f,0.0f , -mWorldHeight});
	world.AddWall3D(NFGE::Math::Plane{ 0.0f,0.0f,1.0f , 0.0f });
	world.AddWall3D(NFGE::Math::Plane{ 0.0f,0.0f,-1.0f , -mWorldWidth});

	mCellBoxColor = NFGE::Graphics::Colors::DarkGray;
	mAlpha = 1.0f;
}

void AIWorld_3D::Terminate()
{
	mLightBuffer.Terminate();
}

void AIWorld_3D::Update(float deltaTime)
{
	world.Update(deltaTime);
}

void AIWorld_3D::Update(float neighborRange, float deltaTime)
{
	world.Update(neighborRange, deltaTime);
}

void AIWorld_3D::Render()
{
	mLightBuffer.Load(mLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	float minX = 0.0f;
	float maxX = mWorldWidth;
	float minY = 0.0f;
	float maxY = mWorldHeight;
	float minZ = 0.0f;
	float maxZ = mWorldDepth;

	auto currentColor = Math::Vector4{ mCellBoxColor.x,mCellBoxColor.y,mCellBoxColor.z, mAlpha };

	for (float j = minY; j <= maxY; j += world.mGrid.mSize)
	{
		for (float i = minX; i <= maxX; i += world.mGrid.mSize)
		{
			NFGE::Graphics::SimpleDraw::AddLine({ i,j, minZ }, { i,j,maxZ }, currentColor);
		}

		for (float i = minZ; i <= maxZ; i += world.mGrid.mSize)
		{
			NFGE::Graphics::SimpleDraw::AddLine({ minX,j, i }, { maxX,j,i }, currentColor);
		}
	}

	for (float j = minZ; j <= maxZ; j += world.mGrid.mSize)
	{
		for (float i = minX; i <= maxX; i += world.mGrid.mSize)
		{
			NFGE::Graphics::SimpleDraw::AddLine({ i,minY, j }, { i,maxY,j }, currentColor);
		}

		for (float i = minY; i <= maxY; i += world.mGrid.mSize)
		{
			NFGE::Graphics::SimpleDraw::AddLine({ minX,i, j }, { maxX,i,j }, currentColor);
		}
	}
}

void AIWorld_3D::DebugDraw()
{
	ImGui::Begin("World");
	ImGui::DragFloat("Cell Box Alpha:", &mAlpha,0.02f,0.0f,1.0f);
	auto& cameraDir = NFGE::sApp.GetMainCamera().GetDirection();
	auto& cameraPos = NFGE::sApp.GetMainCamera().GetPosition();
	ImGui::Text("Camera Pos, %f, %f, %f", cameraPos.x, cameraPos.y, cameraPos.z);
	ImGui::Text("Camera Dir, %f, %f, %f", cameraDir.x, cameraDir.y, cameraDir.z);
	if (ImGui::Button("Camera Stander"))
	{
		NFGE::sApp.GetMainCamera().SetPosition({ 650.0f,630.0f,-200.0f });
		NFGE::sApp.GetMainCamera().SetDirection({ 0.0f,-0.7f,0.6f });
	}
	if (ImGui::Button("Camera TopDown"))
	{
		NFGE::sApp.GetMainCamera().SetPosition({ 600.0f,750.0f,350.0f });
		NFGE::sApp.GetMainCamera().SetDirection({ 0.00001f,-0.9f,0.01f });
	}
	ImGui::End();
}
