#include "Zory_RobotArmMesh.h"

namespace
{
	const wchar_t* MeshShaderFileName = L"../../Assets/Shaders/Standard_noTexture.fx";
}

void Zory_RobotArmMesh::Load()
{
	mMesh = NFGE::Graphics::MeshBuilder::CreateCube();
	mMeshBuffer.Initialize(mMesh);
	mMeshBuffer.SetTopology();

	mMaterial.ambient = NFGE::Graphics::Colors::LightGray;
	mMaterial.diffuse = NFGE::Graphics::Colors::LightSlateGray;
	mMaterial.specular = { 0.8f, 0.8f, 0.8f, 1.0f };
	mMaterial.power = 10.0f;
	mMaterialBuffer.Initialize();

	mMatrices[0].Identity();
	mMatrices[1].Identity();
	mMatrices[2].Identity();
	mTransformBuffer.Initialize();

	mVertexShader.Initialize(MeshShaderFileName, NFGE::Graphics::Vertex::Format);
	mPixelShader.Initialize(MeshShaderFileName);

	mPosition = NFGE::Math::Vector3(0.0f,5.0f,0.0f);
	mRotation = NFGE::Math::Quaternion();
	mScale = NFGE::Math::Vector3(1.0f,5.0f,1.0f);

	NFGE::Math::Matrix4 mat = NFGE::Math::MatrixRotationQuaternion(mRotation);			//|
	mat._11 *= mScale.x;																//|
	mat._22 *= mScale.y;																//|
	mat._33 *= mScale.z;																//|
	mat._41 = mPosition.x;																//|--- Step 2
	mat._42 = mPosition.y;																//|
	mat._43 = mPosition.z;																//|
																						//|
	mChildSpwanPosition = NFGE::Math::Vector3{ 0.0f,1.0f,0.0f } * mat;					//|
}

void Zory_RobotArmMesh::Update(float deltaTime)
{
	
}

void Zory_RobotArmMesh::Render(const NFGE::Math::Matrix4 & toWorldMatrix, const NFGE::Graphics::Camera & camera)
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	NFGE::Math::Matrix4 rotMat = NFGE::Math::MatrixRotationQuaternion(mRotation);
	NFGE::Math::Matrix4 scl = NFGE::Math::Matrix4::sScaling(mScale);
	mMatrices[0] = (scl * rotMat * mMatrices[0].Translation(mPosition.x, mPosition.y, mPosition.z) * toWorldMatrix);
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

void Zory_RobotArmMesh::UnLoad()
{
	mMeshBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
	mMaterialBuffer.Terminate();
}
