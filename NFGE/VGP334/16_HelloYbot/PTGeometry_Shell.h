#ifndef INCLUDED_HELLOYBOT_PTGEOMETRY_SHELL_H
#define INCLUDED_HELLOYBOT_PTGEOMETRY_SHELL_H

#include <NFGE/Inc/NFGE.h>

// Using selfDefine type --------------------------------------------------------------------------------------------------
//struct TransformData
//{
//	NFGE::Math::Matrix4 world;
//	NFGE::Math::Matrix4 wvp;
//	NFGE::Math::Vector3 viewPositon;
//	float padding;
//};
//using TransformBuffer = NFGE::Graphics::TypedConstantBuffer<TransformData>;
using LightBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::DirectionalLight>;
using MaterialBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::Material>;
//--------------------------------------------------------------------------------------------------------------------------
using namespace NFGE::Graphics;
class PTGemotry_Shell
{
protected:
	NFGE::Graphics::Mesh mMesh;
	NFGE::Graphics::MeshBuffer mMeshBuffer;

	NFGE::Math::Matrix4 mMatrices[3];
	NFGE::Graphics::Material mMaterial;
	TransformBuffer mTransformBuffer;
	MaterialBuffer mMaterialBuffer;
	NFGE::Graphics::ConstantBuffer mConstantBuffer;

	NFGE::Graphics::VertexShader mVertexShader;
	NFGE::Graphics::PixelShader mPixelShader;

	NFGE::Math::Matrix4 mAdjestMatrix;
protected:
	void BaseLoad(const wchar_t* vertexShader, const wchar_t* pixelShader)
	{
		mAdjestMatrix = NFGE::Math::Matrix4::sIdentity();

		mMatrices[0].Identity();
		mMatrices[1].Identity();
		mMatrices[2].Identity();
		mMeshBuffer.Initialize(mMesh);
		mMeshBuffer.SetTopology();
		mVertexShader.Initialize(vertexShader, NFGE::Graphics::Vertex::Format);
		mPixelShader.Initialize(pixelShader);

		mTransformBuffer.Initialize();
		mMaterialBuffer.Initialize();

		mMaterial.ambient = NFGE::Graphics::Colors::LightGray;
		mMaterial.diffuse = NFGE::Graphics::Colors::LightSlateGray;
		mMaterial.specular = { 0.8f, 0.8f, 0.8f, 1.0f };
		mMaterial.power = 10.0f;
	}
	void BaseUnLoad()
	{
		mMeshBuffer.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();
		mTransformBuffer.Terminate();
		mMaterialBuffer.Terminate();
	}
	void Render(const NFGE::Math::Matrix4& toWorldMatrix, const NFGE::Graphics::Camera& camera, const NFGE::Graphics::Color& diffuseColor = NFGE::Graphics::Colors::Gray, const NFGE::Graphics::Color& ambientColor = NFGE::Graphics::Colors::DarkGray)
	{

		mVertexShader.Bind();
		mPixelShader.Bind();

		NFGE::Graphics::RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

		mMatrices[0].Identity();
		mMatrices[0] = (mAdjestMatrix * toWorldMatrix);
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

		mMeshBuffer.Render();
		rs->Clear();
	}
public:
	void UpdateAdjustMatrix(const NFGE::Math::Matrix4& mat)
	{
		mAdjestMatrix = mat;
	}
};

#endif // !INCLUDED_HELLOYBOT_PTGEOMETRY_SHELL_H

