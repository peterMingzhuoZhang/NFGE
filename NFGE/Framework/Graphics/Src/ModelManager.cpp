#include "Precompiled.h"
#include "ModelManager.h"

using namespace NFGE::Graphics;

namespace
{
	std::unique_ptr<ModelManager> sInstance = nullptr;
}

void NFGE::Graphics::ModelManager::StaticInitialize(std::filesystem::path rootPath)
{
	ASSERT(sInstance == nullptr, "[TextureManager] System already initlizlized!");
	sInstance = std::make_unique<ModelManager>();
	sInstance->SetRootPath(std::move(rootPath));
}

void NFGE::Graphics::ModelManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance.reset();
	}
}

ModelManager* NFGE::Graphics::ModelManager::Get()
{
	ASSERT(sInstance != nullptr, "[TextureManager] System already initlizlized!");
	return sInstance.get();
}

NFGE::Graphics::ModelManager::~ModelManager()
{
	// TODO Do some error checks here, but we need a way to unload sepecific textures first
	// Maybe add the concept of texture group
	for (auto& [key, value] : mInventory)
	{
		value->UnLoad();
	}
}

void NFGE::Graphics::ModelManager::SetRootPath(std::filesystem::path rootPath)
{
	ASSERT(std::filesystem::is_directory(rootPath), "[ModelManager] %s is not a directory!", (char*)rootPath.c_str());
	mRootPath = std::move(rootPath);
	//if (mRootPath.back() != L'/')
	//	mRootPath += L'/';
}

ModelId NFGE::Graphics::ModelManager::LoadModel(std::filesystem::path filename, bool isUsingRootPath)
{
	auto hash = std::filesystem::hash_value(filename);
	auto[iter, success] = mInventory.insert({ hash, nullptr });
	if (success)
	{
		iter->second = std::make_unique<Model>();
		if (isUsingRootPath)
			iter->second->Load(mRootPath / filename);
		else
			iter->second->Load(filename);
	}
	return hash;
}

void ModelManager::Clear()
{
	for (auto& item : mInventory)
	{
		if (item.second)
		{
			item.second->UnLoad();
			item.second.release();
			item.second.reset();
		}
	}
	mInventory.clear();
}

Model * NFGE::Graphics::ModelManager::GetModel(ModelId textureId)
{
	
	auto iter = mInventory.find(textureId); 
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}
