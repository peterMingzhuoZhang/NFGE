#include "Precompiled.h"
#include "SkeletonModel.h"
#include "ModelIO.h"
#include "SkeletonIO.h"
#include "EffectManager.h"
#include "GeometryMeshManager.h"

using namespace NFGE::Math;


namespace
{
	NFGE::Graphics::DirectionalLight modelDefaultLight{
		{ 0.0f,-1.0f,0.0f },
		0.0f,
		NFGE::Graphics::Colors::Black,
		NFGE::Graphics::Colors::Black,
		NFGE::Graphics::Colors::Black
	};

	NFGE::Graphics::Color boneDefaultAmbient = NFGE::Graphics::Colors::Green;
	NFGE::Graphics::Color boneDefaultDiffuse = NFGE::Graphics::Colors::Blue;
}

void NFGE::Graphics::SkeletonModel::BoneMesh::Load(const NFGE::Math::Vector4 & ambientColor, const NFGE::Math::Vector4 & diffuseColor, DirectionalLight * light)
{
	mAmbientColor = ambientColor;
	mDiffuseColor = diffuseColor;
	mLight = light;
}

void NFGE::Graphics::SkeletonModel::BoneMesh::Render(const NFGE::Math::Matrix4 & toParentMat, NFGE::Math::Matrix4 & toWorldMat, const NFGE::Graphics::Camera & camera, bool isSelected)
{
	auto coneMesh = GeometryMeshManager::Get()->GetGeometryMesh(GeometryType::Cone);
	auto sphereMesh = GeometryMeshManager::Get()->GetGeometryMesh(GeometryType::Sphere);

	Vector3 parent = Vector3::Zero() *  NFGE::Math::Inverse(toParentMat);
	float scaleY = Magnitude(parent) - 2.0f;

	Quaternion qua = QuaternionFromTo({ 0.0f,1.0f,0.0f }, parent);

	Vector3 toBottom = { 0.0f,0.5f,0.0f };
	Vector3 toAboveSphere = { 0.0f,1.0f,0.0f };
	Vector3 adjustScale = { 2.0f, scaleY, 2.0f };

	NFGE::Math::Matrix4 coneAdjustMat = NFGE::Math::Matrix4::sTranslation(toBottom) * NFGE::Math::Matrix4::sScaling(adjustScale) * NFGE::Math::Matrix4::sTranslation(toAboveSphere) * MatrixRotationQuaternion(qua);

	coneMesh->UpdateAdjustMatrix(coneAdjustMat);

	coneMesh->Render(toWorldMat, camera, isSelected ? NFGE::Graphics::Colors::White : mDiffuseColor, mAmbientColor);
	sphereMesh->Render(toWorldMat, camera, isSelected ? NFGE::Graphics::Colors::White : mDiffuseColor, mAmbientColor);

}

void NFGE::Graphics::SkeletonModel::Load(const char * modelFileName, const char* skeletonFileName, DirectionalLight * directionLight)
{
	bool isBinaryFile = ModelIO::IsEndOf("NFModel", modelFileName);
	//Open file load mesh and close file
	FILE* inputFile;
	if (isBinaryFile)
	{
		inputFile = fopen(modelFileName, "rb");
		LoadFromFile(inputFile);
	}
	else
	{
		inputFile = fopen(modelFileName, "r");
		LoadFromFile(inputFile, NFGE::Core::IOOption::RV);
	}
	fclose(inputFile);

	for (int i = 0; i < static_cast<int>(mMeshData.size()); ++i)
	{
		mMeshData[i].meshBuffer.Initialize(mMeshData[i].mesh);
		mMeshData[i].meshBuffer.SetTopology();

		mContext.materials.emplace_back(Material{
			NFGE::Graphics::Colors::Gray,
			{ 0.7f, 0.7f, 0.7f, 1.0f },
			{ 0.2f, 0.2f, 0.2f, 1.0f },
			10.0f });

		mContext.materialIndices.push_back(mMeshData[i].materialIndex);
	}

	for (int i = 0; i < static_cast<int>(mContext.modelMaterials.size()); ++i)
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

	FILE* infile = fopen(skeletonFileName, "r");
	SkeletonIO::Load(mSkeleton, infile);
	fclose(infile);

	for (int i = 0; i < mSkeleton.bones.size(); ++i)
	{
		mContext.boneMatrix.emplace_back();
		mContext.tPosToParentMatrix.push_back(mSkeleton.bones[i]->toParentTransform);
	}

	if (directionLight == nullptr)
		mContext.light = &modelDefaultLight;
	else
		mContext.light = directionLight;

	mBoneMesh.Load(boneDefaultAmbient, boneDefaultAmbient, directionLight);
}

void NFGE::Graphics::SkeletonModel::Load(const std::filesystem::path & modelFlileName, const std::filesystem::path& skeletonFileName, DirectionalLight * directionLight)
{
	Load(modelFlileName.string().c_str(), skeletonFileName.string().c_str(), directionLight);
}

void NFGE::Graphics::SkeletonModel::UnLoad()
{
	for (auto& meshData : mMeshData)
	{
		meshData.meshBuffer.Terminate();
	}
}

void NFGE::Graphics::SkeletonModel::Update(float deltaTime)
{
	mContext.UpdateTransform(mSkeleton.root->index);
}

void NFGE::Graphics::SkeletonModel::Update(Animator & animator, float deltaTime)
{
	for (int i = 0; i < mSkeleton.bones.size(); i++)
	{
		bool hasAnimation;
		mSkeleton.bones[i]->toParentTransform = animator.GetTranform(i, hasAnimation);

		if (!hasAnimation)
			mSkeleton.bones[i]->toParentTransform = mContext.tPosToParentMatrix[i];
	}
	mContext.UpdateTransform(mSkeleton.root->index);
}

void NFGE::Graphics::SkeletonModel::OldRender(const NFGE::Graphics::Camera& camera)
{
	auto effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::SkeletonModelMesh);
	
	for (auto& meshData : mMeshData)
	{
		meshData.meshBuffer.Render(effect, &mContext, camera);
	}
}

void NFGE::Graphics::SkeletonModel::Render(const NFGE::Graphics::Camera& camera)
{
	auto effectManager = EffectManager::Get();
	auto effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::SkeletonModelMesh);

	for (auto& meshData : mMeshData)
	{
		effectManager->RegisterRenderObject(effect, &mContext, &meshData.meshBuffer, camera);
	}
}

void NFGE::Graphics::SkeletonModel::DebugUI(const NFGE::Graphics::Camera& camera)
{
	RenderSkeleton(mSkeleton.root, camera);
}

bool NFGE::Graphics::SkeletonModel::SaveToFile(FILE * file, NFGE::Core::IOOption option) const
{
	switch (option)
	{
	case NFGE::Core::IOOption::Binary:
		return ModelIO::SaveBianry(*this, file);
		break;
	case NFGE::Core::IOOption::RV:
		return ModelIO::SaveRV(*this, file);
		break;
	default:
		return ModelIO::SaveBianry(*this, file);
		break;
	}
}

bool NFGE::Graphics::SkeletonModel::LoadFromFile(FILE * file, NFGE::Core::IOOption option)
{
	switch (option)
	{
	case NFGE::Core::IOOption::Binary:
		return ModelIO::LoadBianry(*this, file);
		break;
	case NFGE::Core::IOOption::RV:
		return ModelIO::LoadRV(*this, file);
		break;
	default:
		return ModelIO::LoadBianry(*this, file);
		break;
	}
}

void NFGE::Graphics::SkeletonModel::RenderSkeleton(Bone * bone, const Camera & camera)
{
	auto finalToWorld = mContext.boneMatrix[bone->index] * mContext.finalAdjustMatrix *mContext.finalToWorld;
	mBoneMesh.Render(bone->toParentTransform, finalToWorld, camera, false);
	for (auto child : bone->children)
	{
		RenderSkeleton(child, camera);
	}
}

