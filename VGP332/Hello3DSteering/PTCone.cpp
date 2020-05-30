#include "PTCone.h"

void PTCone::Initialize(int row, int col, float radius, const wchar_t * vertexShader, const wchar_t * pixelShader)
{
	mPosition = NFGE::Math::Vector3(0.0f);
	mRadius = radius;

	mMesh = NFGE::Graphics::MeshBuilder::CreateConePN(row, col, mRadius,(float)row);

	mMatrices[0].Identity();
	mMatrices[1].Identity();
	mMatrices[2].Identity();
	mMeshBuffer.Initialize(mMesh);
	mMeshBuffer.SetTopology();
	mVertexShader.Initialize(vertexShader, NFGE::Graphics::VertexPN::Format);
	mPixelShader.Initialize(pixelShader);

	mTransformBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mMaterial.ambient = NFGE::Graphics::Colors::HotPink;
	mMaterial.diffuse = NFGE::Graphics::Colors::LightSlateGray;
	mMaterial.specular = { 0.8f, 0.8f, 0.8f, 1.0f };
	mMaterial.power = 10.0f;

	mIsInitialize = true;
}

void PTCone::Terminate()
{
	mMeshBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
	mMaterialBuffer.Terminate();
}

void PTCone::Update()
{
}

void PTCone::Render(const NFGE::Math::Vector3 & pos, const NFGE::Math::Matrix4 & rotMat, const NFGE::Math::Vector3 & scale, const NFGE::Graphics::Camera & camera)
{
	mPosition = pos;
	mRotation = rotMat;

	mVertexShader.Bind();
	mPixelShader.Bind();

	NFGE::Graphics::RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

	mMatrices[0].Identity();
	NFGE::Math::Matrix4 scl;
	scl.Identity();

	scl = scl.sScaling(scale);
	mMatrices[0] = (scl * mRotation * mMatrices[0].Translation(mPosition.x, mPosition.y, mPosition.z));

	mMatrices[1] = (camera.GetViewMatrix());
	mMatrices[2] = (camera.GetPerspectiveMatrix());

	TransformData transformData;
	transformData.world = Transpose(mMatrices[0]);
	transformData.wvp = Transpose(mMatrices[0] * mMatrices[1] * mMatrices[2]);
	transformData.viewPositon = camera.GetPosition();
	mTransformBuffer.Load(transformData);
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mMaterialBuffer.Load(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	auto rs = NFGE::Graphics::RasterizerStateManager::Get()->GetRasterizerState("Solid");
	rs->Set();
	mMeshBuffer.Render();
	rs->Clear();
}
