#ifndef HELLOMODEL_PTMODEL_H
#define HELLOMODEL_PTMODEL_H

#include "NFGE/Inc/NFGE.h"
using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;

struct PTModel : public Model
{
	//Matrix4 mMatrices[3];
	//VertexShader mVertexShader;
	//PixelShader mPixelShader;
	//ConstantBuffer mConstantBuffer;

	DirectionalLight* mLight;
	Material mMaterial;

	//struct TransformData
	//{
	//	NFGE::Math::Matrix4 world;
	//	NFGE::Math::Matrix4 wvp;
	//	NFGE::Math::Vector3 viewPositon;
	//	float padding;
	//};
	//struct OptionsData
	//{
	//	float displacementWeight;
	//	uint32_t meshTextureInfoMask;
	//	float padding[2];
	//};
	//using TransformBuffer = NFGE::Graphics::TypedConstantBuffer<TransformData>;
	//using LightBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::DirectionalLight>;
	//using MaterialBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::Material>;
	//using OptionBuffer = NFGE::Graphics::TypedConstantBuffer<OptionsData>;
	//
	//TransformBuffer mTransformBuffer;
	//LightBuffer mLightBuffer;
	//MaterialBuffer mMaterialBuffer;
	//OptionBuffer mOptionDateBuffer;

	//Vector3 mPosition;
	//Vector3 mRotation;
	//Vector3 mScale;
	//Vector3 currentFoward;

	void Load(const Vector3& position, const Vector3& rotation, const Vector3& scale, const char* modelFileName, const wchar_t* vertexShader, const wchar_t* pixelShader)
	{
		Model::Load(modelFileName);
		

		// load all the textureId from texture file name;
		
		mContext.AdjustPosition = position;
		mContext.AdjustRotation = rotation;
		mContext.AdjustScale = scale;
		mContext.currentFoward = Vector3(0.0f, 0.0f, 1.0f);
		//mPosition = position;
		//mRotation = rotation;
		//mScale = scale;
		//currentFoward = Vector3(0.0f, 0.0f, 1.0f);

		//mMatrices[0].Identity();
		//mMatrices[1].Identity();
		//mMatrices[2].Identity();
		//
		//mVertexShader.Initialize(vertexShader, Vertex::Format);
		//mPixelShader.Initialize(pixelShader);

		//mTransformBuffer.Initialize();
		//mLightBuffer.Initialize();
		//mMaterialBuffer.Initialize();
		//mOptionDateBuffer.Initialize();
		//
		mLight = &NFGE::sApp.GetMainLight();
		mLight->direction = NFGE::Math::Normalize({ 1.0f,-1.0f,1.0f });
		mLight->ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
		mLight->diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
		mLight->specular = { 0.5f, 0.5f, 0.5f, 1.0f };
		mContext.light = mLight;

		mMaterial.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
		mMaterial.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
		mMaterial.specular = { 0.2f, 0.2f, 0.2f, 1.0f };
		mMaterial.power = 10.0f;
		mContext.materials[0] = mMaterial;

	}
	void Unload()
	{
		Model::UnLoad();
		//mTransformBuffer.Terminate();
		//mLightBuffer.Terminate();
		//mMaterialBuffer.Terminate();
		//
		//mVertexShader.Terminate();
		//mPixelShader.Terminate();
		//mConstantBuffer.Terminate();

	}
	void Render(Camera& camera)
	{
		OnRender(camera);
		Model::Render(camera);
		//mVertexShader.Bind();
		//mPixelShader.Bind();
		//
		//SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();
		//RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
		//
		//mMatrices[0].Identity();
		//Matrix4 rotX, rotY, rotZ;
		//rotX.Identity();
		//rotY.Identity();
		//rotZ.Identity();
		//(rotX.RotationX(mRotation.x));
		//(rotY.RotationY(mRotation.y));
		//(rotZ.RotationZ(mRotation.z));
		//
		//NFGE::Math::Matrix4 rotMat;
		//
		//
		//
		//rotMat = rotX * rotY * rotZ * NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionLookRotation(currentFoward, Vector3::YAxis));
		//
		//
		//Matrix4 scaleMat;
		//scaleMat.Identity();
		//scaleMat.sScaling(mScale);
		//mMatrices[0] = (rotMat * scaleMat * mMatrices[0].Translation(mPosition.x, mPosition.y, mPosition.z));
		//OnRender(camera);
		//mMatrices[1] = camera.GetViewMatrix();
		//mMatrices[2] = camera.GetPerspectiveMatrix();
		//
		//TransformData transformData;
		//transformData.world = Transpose(mMatrices[0]);
		//transformData.wvp = Transpose(mMatrices[0] * mMatrices[1] * mMatrices[2]);
		//transformData.viewPositon = camera.GetPosition();
		//mTransformBuffer.Load(transformData);
		//mTransformBuffer.BindVS(0);
		//mTransformBuffer.BindPS(0);
		//
		//mLightBuffer.Load(mLight);
		//mLightBuffer.BindVS(1);
		//mLightBuffer.BindPS(1);
		//
		//mMaterialBuffer.Load(mMaterial);
		//mMaterialBuffer.BindVS(2);
		//mMaterialBuffer.BindPS(2);



		//for (int i = 0; i < static_cast<int>(mMeshData.size()); ++i)
		//{
		//	using TextureType = MeshTextureMaterial::ModelTextureType;
		//	auto& currentMeshbuffer =  mMeshData[i].meshBuffer;
		//	auto& currentModelMaterial = mContext.modelMaterials[mMeshData[i].materialIndex];
		//	
		//	uint32_t currentContainTexture = currentModelMaterial.mAvliableTextureType;
		//	//currentContainTexture &= ~Model::MeshTextureMaterial::TextureType_DIFFUSE;
		//	if (currentContainTexture & MeshTextureMaterial::TextureType_DIFFUSE)
		//		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::DIFFUSE].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::DIFFUSE));
		//	if (currentContainTexture & MeshTextureMaterial::TextureType_SPECULAR)
		//		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::SPECULAR].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::SPECULAR));
		//	if (currentContainTexture & MeshTextureMaterial::TextureType_AMBIENT)
		//		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::AMBIENT].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::AMBIENT));
		//	if (currentContainTexture & MeshTextureMaterial::TextureType_EMISSIVE)
		//		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::EMISSIVE].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::EMISSIVE));
		//	if (currentContainTexture & MeshTextureMaterial::TextureType_HEIGHT)
		//		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::HEIGHT].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::HEIGHT));
		//	if (currentContainTexture & MeshTextureMaterial::TextureType_NORMALS)
		//		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::NORMALS].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::NORMALS));
		//	if (currentContainTexture & MeshTextureMaterial::TextureType_SHININESS)
		//		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::SHININESS].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::SHININESS));
		//	if (currentContainTexture & MeshTextureMaterial::TextureType_OPACITY)
		//		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::OPACITY].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::OPACITY));
		//	if (currentContainTexture & MeshTextureMaterial::TextureType_DISPLACEMENT)
		//		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::DISPLACEMENT].mTextureIds[0])->BindVS(static_cast<uint32_t>(TextureType::DISPLACEMENT));
		//	//TODO:: Bind more if needed
		//
		//	OptionsData optData;
		//	optData.meshTextureInfoMask = currentContainTexture;
		//	mOptionDateBuffer.Load(optData);
		//	mOptionDateBuffer.BindVS(3);
		//	mOptionDateBuffer.BindPS(3);
		//
		//	currentMeshbuffer.Render();
		//}
	}
	virtual void OnRender(const Camera& camera) {};
};

#endif // !HELLOMODEL_PTMODEL_H
