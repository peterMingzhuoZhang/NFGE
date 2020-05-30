#pragma once

#include "PTGeometry_Shell.h"

using namespace NFGE::Graphics;

//struct OptionsData
//{
//	float displacementWeight;
//	uint32_t meshTextureInfoMask;
//	float padding[2];
//};

struct OptionsData2
{
	float displacementWeight;
	uint32_t meshTextureInfoMask;
	float brightness;
	bool useShadow;
};
using OptionBuffer_ = NFGE::Graphics::TypedConstantBuffer<OptionsData2>;


class PTPlane : public PTGemotry_Shell
{
	using ShadowConstantBuffer = TypedConstantBuffer<Matrix4>;
public:
	void Load(const wchar_t* vertexShader, const wchar_t* pixelShader, float width, float length, const char* speclur, const char* diffuse, const char* displacement, const char* normal)
	{
		mMesh = NFGE::Graphics::MeshBuilder::CreatePlane(100,100,1.0f);

		PTGemotry_Shell::BaseLoad(vertexShader, pixelShader);

		mAdjestMatrix = NFGE::Math::Matrix4::sIdentity() * NFGE::Math::Matrix4::sScaling({ width,length, 1.0f }) * NFGE::Math::Matrix4::sRotationX(NFGE::Math::Constants::Pi * 0.5f);

		mOptionDateBuffer.Initialize();
		mShadowConstantBuffer.Initialize();

		//mMeshMaterialTextures.LoadTexture(MeshTextureMaterial::SPECULAR,			TextureManager::Get()->LoadTexture(speclur));
		//mMeshMaterialTextures.LoadTexture(MeshTextureMaterial::DIFFUSE,			TextureManager::Get()->LoadTexture(diffuse));
		//mMeshMaterialTextures.LoadTexture(MeshTextureMaterial::DISPLACEMENT,	TextureManager::Get()->LoadTexture(displacement));
		mMeshMaterialTextures.LoadTexture(MeshTextureMaterial::NORMALS,			TextureManager::Get()->LoadTexture(normal));
	}

	void Update(float deltaTime)
	{

	}

	void Render(const NFGE::Math::Matrix4& toWorldMatrix, const NFGE::Graphics::Camera& camera, const NFGE::Graphics::Camera& lightCamera, RenderTarget& depthMap, const NFGE::Graphics::Color& diffuseColor = NFGE::Graphics::Colors::Gray, const NFGE::Graphics::Color& ambientColor = NFGE::Graphics::Colors::White)
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

		OptionsData2 optData;
		optData.displacementWeight = 1.0f;
		optData.meshTextureInfoMask = mMeshMaterialTextures.mAvliableTextureType;
		optData.brightness = 0.0f;
		optData.useShadow = 1;
		mOptionDateBuffer.Load(optData);
		mOptionDateBuffer.BindVS(3);
		mOptionDateBuffer.BindPS(3);

		auto wvpLight = Transpose(mMatrices[0] * lightCamera.GetViewMatrix() * lightCamera.GetPerspectiveMatrix());
		mShadowConstantBuffer.Load(wvpLight);
		mShadowConstantBuffer.BindVS(4);

		depthMap.BindPS(5);

		RenderTarget dummy;
		dummy.UnbindPS(0);
		dummy.UnbindPS(1);
		dummy.UnbindVS(2);
		dummy.UnbindPS(4);
		//TextureManager::Get()->GetTexture(mMeshMaterialTextures.GetTexture(MeshTextureMaterial::DIFFUSE))->BindPS();
		//TextureManager::Get()->GetTexture(mMeshMaterialTextures.GetTexture(MeshTextureMaterial::SPECULAR))->BindPS(1);
		TextureManager::Get()->GetTexture(mMeshMaterialTextures.GetTexture(MeshTextureMaterial::NORMALS))->BindPS(3);
		//TextureManager::Get()->GetTexture(mMeshMaterialTextures.GetTexture(MeshTextureMaterial::DISPLACEMENT))->BindVS(8);
		SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();
		SamplerManager::Get()->GetSampler("LinearWrap")->BindVS();

		auto rs = NFGE::Graphics::RasterizerStateManager::Get()->GetRasterizerState("Solid");
		rs->Set();

		mMeshBuffer.Render();
		rs->Clear();
	}

	void Render(const NFGE::Math::Matrix4& toWorldMatrix, const NFGE::Graphics::Camera& camera, const NFGE::Math::Matrix4& viewMatrix, const NFGE::Math::Matrix4& projMat, RenderTarget& depthMap, const NFGE::Graphics::Color& diffuseColor = NFGE::Graphics::Colors::Gray, const NFGE::Graphics::Color& ambientColor = NFGE::Graphics::Colors::White)
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

		OptionsData2 optData;
		optData.displacementWeight = 1.0f;
		optData.meshTextureInfoMask = mMeshMaterialTextures.mAvliableTextureType;
		optData.brightness = 0.0f;
		optData.useShadow = 1;
		mOptionDateBuffer.Load(optData);
		mOptionDateBuffer.BindVS(3);
		mOptionDateBuffer.BindPS(3);

		auto wvpLight = Transpose(mMatrices[0] * viewMatrix * projMat);
		mShadowConstantBuffer.Load(wvpLight);
		mShadowConstantBuffer.BindVS(4);

		depthMap.BindPS(5);

		RenderTarget dummy;
		dummy.UnbindPS(0);
		dummy.UnbindPS(1);
		dummy.UnbindVS(2);
		dummy.UnbindPS(4);
		//TextureManager::Get()->GetTexture(mMeshMaterialTextures.GetTexture(MeshTextureMaterial::DIFFUSE))->BindPS();
		//TextureManager::Get()->GetTexture(mMeshMaterialTextures.GetTexture(MeshTextureMaterial::SPECULAR))->BindPS(1);
		TextureManager::Get()->GetTexture(mMeshMaterialTextures.GetTexture(MeshTextureMaterial::NORMALS))->BindPS(3);
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
		mShadowConstantBuffer.Terminate();
	}
private:
	NFGE::Graphics::MeshTextureMaterial mMeshMaterialTextures;
	OptionBuffer_ mOptionDateBuffer;
	ShadowConstantBuffer mShadowConstantBuffer;
};
