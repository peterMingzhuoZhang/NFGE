#ifndef HELLOYBOT_PTMODEL_H
#define HELLOYBOT_PTMODEL_H

#include "NFGE/Inc/NFGE.h"
#include "PTSFSkeleton.h"

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;

struct PTModel : public Model
{
	bool mIsInitalized = false;
	Matrix4 mMatrices[3];
	VertexShader mVertexShader;
	PixelShader mPixelShader;
	ConstantBuffer mConstantBuffer;

	DirectionalLight mLight;
	std::vector<Material> mMaterials;

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
	using TransformBuffer = NFGE::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::DirectionalLight>;
	using MaterialBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::Material>;
	using BoneTransformBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Math::Matrix4[128]>;
	using OptionBuffer = NFGE::Graphics::TypedConstantBuffer<OptionsData>;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	BoneTransformBuffer mBoneTransformBuffer;
	OptionBuffer mOptionDateBuffer;

	Vector3 mPosition;
	Vector3 mRotation;
	Vector3 mScale;
	Vector3 currentFoward;

	Matrix4 mAdjustRotMat = Matrix4::sIdentity();

	PTSFSkeleton mSkeleton;

	void Load(const Vector3& position, const Vector3& rotation, const Vector3& scale, const char* modelFileName, const wchar_t* vertexShader, const wchar_t* pixelShader, const char* skeletonFile)
	{
		Model::Load(modelFileName);
		for (int i = 0; i < mMeshData.size(); ++i)
		{
			mMeshData[i].meshBuffer.Initialize(mMeshData[i].mesh);
			mMeshData[i].meshBuffer.SetTopology();

			mMaterials.emplace_back(Material{
				NFGE::Graphics::Colors::Gray,
				{ 0.7f, 0.7f, 0.7f, 1.0f },
				{ 0.2f, 0.2f, 0.2f, 1.0f },
				10.0f});
		}

		// load all the textureId from texture file name;
		for (int i = 0; i < mContext.modelMaterials.size(); ++i)
		{
			auto& currentMaterials = mContext.modelMaterials[i];
			uint32_t containTextureMask = currentMaterials.mAvliableTextureType;
			for (auto& textureHolder : currentMaterials.mTextureHolders)
			{
				if (textureHolder.mNumOfTextures > 0)
				{
					for (int i = 0; i < textureHolder.mNumOfTextures; ++i)
					{
						textureHolder.mTextureIds.emplace_back(TextureManager::Get()->LoadTexture(textureHolder.mFilenames[i], false));
					}
				}
			}
		}

		mPosition = position;
		mRotation = rotation;
		mScale = scale;
		currentFoward = Vector3(0.0f, 0.0f, 1.0f);

		mMatrices[0].Identity();
		mMatrices[1].Identity();
		mMatrices[2].Identity();
		
		mVertexShader.Initialize(vertexShader, BoneVertex::Format);
		mPixelShader.Initialize(pixelShader);

		mTransformBuffer.Initialize();
		mLightBuffer.Initialize();
		mMaterialBuffer.Initialize();
		mBoneTransformBuffer.Initialize();
		mOptionDateBuffer.Initialize();

		mLight.direction = NFGE::Math::Normalize({ 1.0f,-1.0f,1.0f });
		mLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
		mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
		mLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };

		mSkeleton.Load(skeletonFile);

	}
	void Unload()
	{
		Model::UnLoad();
		mTransformBuffer.Terminate();
		mLightBuffer.Terminate();
		mMaterialBuffer.Terminate();
		mBoneTransformBuffer.Terminate();
		for (auto& meshData : mMeshData)
		{
			meshData.meshBuffer.Terminate();
		}
		mVertexShader.Terminate();
		mPixelShader.Terminate();
		mConstantBuffer.Terminate();

	}

	void Update(Animator& animator, float deltaTime)
	{
		for (int i = 0; i < mSkeleton.mSkeleton.bones.size(); i++)
		{
			bool hasAnimation;
			mSkeleton.mSkeleton.bones[i]->toParentTransform = animator.GetTranform(i, hasAnimation);

			if (!hasAnimation)
				mSkeleton.mSkeleton.bones[i]->toParentTransform = mSkeleton.tPosToParentMatrix[i];
		}
		mSkeleton.UpdateToParentTransform(mSkeleton.mSkeleton.root->index);		// Refresh ToParentTransform from root
	}

	void Render(const Camera& camera)
	{
		mVertexShader.Bind();
		mPixelShader.Bind();

		SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();
		RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

		mMatrices[0].Identity();
		Matrix4 rotX, rotY, rotZ;
		rotX.Identity();
		rotY.Identity();
		rotZ.Identity();
		(rotX.RotationX(mRotation.x));
		(rotY.RotationY(mRotation.y));
		(rotZ.RotationZ(mRotation.z));

		NFGE::Math::Matrix4 rotMat;
		
		auto right = NFGE::Math::Cross(NFGE::Math::Vector3::YAxis, currentFoward);
		auto up = NFGE::Math::Cross(currentFoward, right);
		float yRotationAngle = NFGE::Math::IsZero(currentFoward.z) ? 0.0f : atanf(currentFoward.x / currentFoward.z);
		float xRotationAngle = NFGE::Math::IsZero(currentFoward.z) ? 0.0f : atanf(-currentFoward.y / currentFoward.z);
		if (currentFoward.z < 0.0f)
		{
			yRotationAngle += NFGE::Math::Constants::Pi;
			xRotationAngle += NFGE::Math::Constants::Pi;
		}
		
		mAdjustRotMat = rotX * rotY * rotZ;
		rotMat = mAdjustRotMat * NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionLookRotation(currentFoward, Vector3::YAxis));


		Matrix4 scaleMat;
		scaleMat.Identity();
		scaleMat.sScaling(mScale);
		mMatrices[0] = (rotMat * scaleMat * mMatrices[0].Translation(mPosition.x, mPosition.y, mPosition.z));
		OnRender(camera);
		mMatrices[1] = camera.GetViewMatrix();
		mMatrices[2] = camera.GetPerspectiveMatrix();

		TransformData transformData;
		transformData.world = Transpose(mMatrices[0]);
		transformData.wvp = Transpose(mMatrices[0] * mMatrices[1] * mMatrices[2]);
		transformData.viewPositon = camera.GetPosition();
		mTransformBuffer.Load(transformData);
		mTransformBuffer.BindVS(0);
		mTransformBuffer.BindPS(0);

		//mLightBuffer.Load(mLight);
		//mLightBuffer.BindVS(1);
		//mLightBuffer.BindPS(1);

		Matrix4 boneTransformMatrix[128];
		auto& bones = mSkeleton.mSkeleton.bones;
		for (int i = 0; i < bones.size(); i++)
		{
			boneTransformMatrix[i] = Transpose(bones[i]->offsetTransform * mSkeleton.boneMatrix[i]);
			//boneTransformMatrix[i] = Transpose(bones[i]->offsetTransform * bones[i]->toParentTransform * Inverse(bones[i]->offsetTransform)); // Clear the translate in toParentTransform
		}
		mBoneTransformBuffer.Load(boneTransformMatrix);
		mBoneTransformBuffer.BindVS(4);

		for (int i = 0; i < mMeshData.size(); ++i)
		{
			using TextureType = MeshTextureMaterial::ModelTextureType;
			auto& currentMeshbuffer =  mMeshData[i].meshBuffer;
			auto& currentModelMaterial = mContext.modelMaterials[mMeshData[i].materialIndex];
			
			uint32_t currentContainTexture = currentModelMaterial.mAvliableTextureType;
			//currentContainTexture &= ~Model::MeshTextureMaterial::TextureType_DIFFUSE;
			if (currentContainTexture & MeshTextureMaterial::TextureType_DIFFUSE)
				TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::DIFFUSE].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::DIFFUSE));
			if (currentContainTexture & MeshTextureMaterial::TextureType_SPECULAR)
				TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::SPECULAR].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::SPECULAR));
			if (currentContainTexture & MeshTextureMaterial::TextureType_AMBIENT)
				TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::AMBIENT].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::AMBIENT));
			if (currentContainTexture & MeshTextureMaterial::TextureType_EMISSIVE)
				TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::EMISSIVE].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::EMISSIVE));
			if (currentContainTexture & MeshTextureMaterial::TextureType_HEIGHT)
				TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::HEIGHT].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::HEIGHT));
			if (currentContainTexture & MeshTextureMaterial::TextureType_NORMALS)
				TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::NORMALS].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::NORMALS));
			if (currentContainTexture & MeshTextureMaterial::TextureType_SHININESS)
				TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::SHININESS].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::SHININESS));
			if (currentContainTexture & MeshTextureMaterial::TextureType_OPACITY)
				TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::OPACITY].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::OPACITY));
			if (currentContainTexture & MeshTextureMaterial::TextureType_DISPLACEMENT)
				TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::DISPLACEMENT].mTextureIds[0])->BindVS(static_cast<uint32_t>(TextureType::DISPLACEMENT));
			//TODO:: Bind more if needed

			OptionsData optData;
			optData.meshTextureInfoMask = currentContainTexture;
			mOptionDateBuffer.Load(optData);
			mOptionDateBuffer.BindVS(3);
			mOptionDateBuffer.BindPS(3);

			mMaterialBuffer.Load(mMaterials[i]);
			mMaterialBuffer.BindVS(2);
			mMaterialBuffer.BindPS(2);

			currentMeshbuffer.Render();
		}
	}
	virtual void OnRender(const Camera& camera) {};
};

#endif // !HELLOYBOT_PTMODEL_H
