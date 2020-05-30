#include "Precompiled.h"
#include "Model.h"
#include"MeshIO.h"
#include "EffectManager.h"
#include "ModelIO.h"

const std::array<const char*, 18> NFGE::Graphics::MeshTextureMaterial::ModelTextureTypeName = {
	{
		"DIFFUSE",
		"SPECULAR",
		"AMBIENT",
		"EMISSIVE",
		"HEIGHT",
		"NORMALS",
		"SHININESS",
		"OPACITY",
		"DISPLACEMENT",
		"LIGHTMAP",
		"REFLECTION",
		"BASE_COLOR",
		"NORMAL_CAMERA",
		"EMISSION_COLOR",
		"MATALNESS",
		"DIFFUSE_ROUGHNESS",
		"AMBIENT_OCCLUSION",
		"UNKNOWN"
	}
};

namespace
{
	NFGE::Graphics::DirectionalLight modelDefaultLight{
		{0.0f,-1.0f,0.0f},
		0.0f,
		NFGE::Graphics::Colors::Black,
		NFGE::Graphics::Colors::Black,
		NFGE::Graphics::Colors::Black
	};
}

void NFGE::Graphics::Model::Load(const char* modelFileName, DirectionalLight* directionLight)
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

		mContext.materials.emplace_back(Material{ Colors::Gray, Colors::Gray, Colors::Gray, 1.0f });
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

	if (directionLight == nullptr)
		mContext.light = &modelDefaultLight;
	else
		mContext.light = directionLight;
}

void NFGE::Graphics::Model::Load(const std::filesystem::path & fileName, DirectionalLight* directionLight)
{
	Load(fileName.string().c_str(), directionLight);
}

void NFGE::Graphics::Model::UnLoad()
{
	for (auto& meshData : mMeshData)
	{
		meshData.meshBuffer.Terminate();
	}
}

void NFGE::Graphics::Model::Update(float deltaTime)
{
}

void NFGE::Graphics::Model::OldRender(const NFGE::Graphics::Camera& camera)
{
	auto effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::ModelMesh);

	for (auto& meshData : mMeshData)
	{
		meshData.meshBuffer.Render(effect, &mContext, camera);
	}
}

void NFGE::Graphics::Model::Render(const NFGE::Graphics::Camera & camera)
{
	auto effectManager = EffectManager::Get();
	auto effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::ModelMesh);
	for (auto& meshData : mMeshData)
	{
		effectManager->RegisterRenderObject( effect, &mContext, &meshData.meshBuffer, camera);
	}
	
}

void NFGE::Graphics::Model::DebugUI()
{
}

bool NFGE::Graphics::Model::SaveToFile(FILE * file, NFGE::Core::IOOption option) const
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

bool NFGE::Graphics::Model::LoadFromFile(FILE * file, NFGE::Core::IOOption option)
{
	switch(option)
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
