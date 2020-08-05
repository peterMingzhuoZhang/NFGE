#ifndef INCLUDED_HELLOROBOTICARM_PTGEOMETRY_SHELL_H
#define INCLUDED_HELLOROBOTICARM_PTGEOMETRY_SHELL_H

#include <NFGE/Inc/NFGE.h>

// Using selfDefine type --------------------------------------------------------------------------------------------------
struct TransformData
{
	NFGE::Math::Matrix4 world;
	NFGE::Math::Matrix4 wvp;
	NFGE::Math::Vector3 viewPositon;
	float padding;
};
using TransformBuffer = NFGE::Graphics::TypedConstantBuffer<TransformData>;
using LightBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::DirectionalLight>;
using MaterialBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::Material>;
//--------------------------------------------------------------------------------------------------------------------------

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

	NFGE::Math::Vector3 mPosition;
	NFGE::Math::Quaternion mRotation;
	NFGE::Math::Vector3 mScale;
protected:
	void BaseLoad(const wchar_t* vertexShader, const wchar_t* pixelShader)
	{
		mPosition = NFGE::Math::Vector3(0.0f);
		mRotation = NFGE::Math::Quaternion();
		mScale = NFGE::Math::Vector3(1.0f);

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
};

#endif // !INCLUDED_HELLOROBOTICARM_PTGEOMETRY_SHELL_H

