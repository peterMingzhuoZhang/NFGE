#pragma once

#include "PTGeometry_Shell.h"

using namespace NFGE::Graphics;

//struct OptionsData
//{
//	float displacementWeight;
//	uint32_t meshTextureInfoMask;
//	float padding[2];
//};
//using OptionBuffer = NFGE::Graphics::TypedConstantBuffer<OptionsData>;


class PTPlane : public PTGemotry_Shell
{
public:
	void Load(const wchar_t* vertexShader, const wchar_t* pixelShader, float width, float length, const char* speclur, const char* diffuse, const char* displacement, const char* normal)
	{
		mMesh = NFGE::Graphics::MeshBuilder::CreatePlane(100, 100, 10.0f);

		PTGemotry_Shell::BaseLoad(vertexShader, pixelShader);

		mAdjestMatrix = NFGE::Math::Matrix4::sIdentity() * NFGE::Math::Matrix4::sScaling({ width,length, 1.0f }) * NFGE::Math::Matrix4::sRotationX(NFGE::Math::Constants::Pi * 0.5f);

		mOptionDateBuffer.Initialize();

		//mMeshMaterialTextures.LoadTexture(MeshTextureMaterial::SPECULAR,			TextureManager::Get()->LoadTexture(speclur));
		//mMeshMaterialTextures.LoadTexture(MeshTextureMaterial::DIFFUSE,			TextureManager::Get()->LoadTexture(diffuse));
		//mMeshMaterialTextures.LoadTexture(MeshTextureMaterial::DISPLACEMENT,	TextureManager::Get()->LoadTexture(displacement));
		mMeshMaterialTextures.LoadTexture(MeshTextureMaterial::NORMALS, TextureManager::Get()->LoadTexture(normal));
	}

	void Update(float deltaTime)
	{

	}

	void Render(const NFGE::Math::Matrix4& toWorldMatrix, const NFGE::Graphics::Camera& camera, const NFGE::Graphics::Color& diffuseColor = NFGE::Graphics::Colors::Gray, const NFGE::Graphics::Color& ambientColor = NFGE::Graphics::Colors::White)
	{
		//PTGemotry_Shell::Render(toWorldMatrix, camera, diffuseColor, ambientColor);
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
		mMaterial.specular = NFGE::Graphics::Colors::AntiqueWhite;
		mMaterial.power = 1.0f;

		mMaterialBuffer.Load(mMaterial);
		mMaterialBuffer.BindVS(2);
		mMaterialBuffer.BindPS(2);

		OptionsData optData;
		optData.displacementWeight = 1.0f;
		optData.meshTextureInfoMask = mMeshMaterialTextures.mAvliableTextureType;
		mOptionDateBuffer.Load(optData);
		mOptionDateBuffer.BindVS(3);
		mOptionDateBuffer.BindPS(3);

		//TextureManager::Get()->GetTexture(mMeshMaterialTextures.GetTexture(MeshTextureMaterial::DIFFUSE))->BindPS();
		//TextureManager::Get()->GetTexture(mMeshMaterialTextures.GetTexture(MeshTextureMaterial::SPECULAR))->BindPS(1);
		TextureManager::Get()->GetTexture(mMeshMaterialTextures.GetTexture(MeshTextureMaterial::NORMALS))->BindPS(5);
		//TextureManager::Get()->GetTexture(mMeshMaterialTextures.GetTexture(MeshTextureMaterial::DISPLACEMENT))->BindVS(8);
		SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();
		SamplerManager::Get()->GetSampler("LinearWrap")->BindVS();

		auto rs = NFGE::Graphics::RasterizerStateManager::Get()->GetRasterizerState("Solid");
		rs->Set();

		mMeshBuffer.Render();
		rs->Clear();

	}
	void UnLoad()
	{
		PTGemotry_Shell::BaseUnLoad();
		mOptionDateBuffer.Terminate();
	}
private:
	NFGE::Graphics::MeshTextureMaterial mMeshMaterialTextures;
	OptionBuffer mOptionDateBuffer;
};
