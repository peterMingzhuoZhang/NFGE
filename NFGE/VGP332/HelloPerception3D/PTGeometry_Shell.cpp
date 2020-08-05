#include "PTGeometry_Shell.h"

void PTGemotry_Shell::Update(float deltaTime)
{
	OnUpdate(deltaTime);
}

void PTGemotry_Shell::Render(const NFGE::Math::Vector3 & pos, const NFGE::Math::Vector3 & forward, const NFGE::Math::Vector3 & scale, const NFGE::Graphics::Camera & camera, const NFGE::Graphics::Color & diffuseColor, const NFGE::Graphics::Color & ambientColor)
{
	mPosition = pos;

	NFGE::Math::Matrix4 meshToLocalRotMat;
	meshToLocalRotMat.Identity();
	meshToLocalRotMat.Rotation(mRotation.x, mRotation.y, mRotation.z);
	NFGE::Math::Matrix4 rotMat = meshToLocalRotMat * NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionFromTo({0.0f,0.0f,1.0f}, forward));

	mVertexShader.Bind();
	mPixelShader.Bind();

	NFGE::Graphics::RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

	mMatrices[0].Identity();
	NFGE::Math::Matrix4 scl;
	scl.Identity();

	scl = scl.sScaling(scale);
	mMatrices[0] = (scl * rotMat * mMatrices[0].Translation(mPosition.x, mPosition.y, mPosition.z));

	mMatrices[1] = (camera.GetViewMatrix());
	mMatrices[2] = (camera.GetPerspectiveMatrix());

	TransformData transformData;
	transformData.world = Transpose(mMatrices[0]);
	transformData.wvp = Transpose(mMatrices[0] * mMatrices[1] * mMatrices[2]);
	transformData.viewPositon = camera.GetPosition();
	mTransformBuffer.Load(transformData);
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mMaterial.diffuse = diffuseColor;
	mMaterial.ambient = ambientColor;
	mMaterialBuffer.Load(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	auto rs = NFGE::Graphics::RasterizerStateManager::Get()->GetRasterizerState("Solid");
	rs->Set();

	//- OnRender ------------------------------------------------------------
	OnRender();
	//-----------------------------------------------------------------------

	mMeshBuffer.Render();
	rs->Clear();
}

void PTGemotry_Shell::Terminate()
{
	mMeshBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
	mMaterialBuffer.Terminate();
	OnTerminate();
}

void PTGemotry_Shell::OnTerminate()
{
}

void PTGemotry_Shell::BaseInitliaze(const wchar_t * vertexShader, const wchar_t * pixelShader)
{
	mPosition = NFGE::Math::Vector3(0.0f);
	
	mMatrices[0].Identity();
	mMatrices[1].Identity();
	mMatrices[2].Identity();
	mMeshBuffer.Initialize(mMesh);
	mMeshBuffer.SetTopology();
	mVertexShader.Initialize(vertexShader, NFGE::Graphics::VertexPN::Format);
	mPixelShader.Initialize(pixelShader);

	mTransformBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mMaterial.ambient = NFGE::Graphics::Colors::LightGray;
	mMaterial.diffuse = NFGE::Graphics::Colors::LightSlateGray;
	mMaterial.specular = { 0.8f, 0.8f, 0.8f, 1.0f };
	mMaterial.power = 10.0f;

}
