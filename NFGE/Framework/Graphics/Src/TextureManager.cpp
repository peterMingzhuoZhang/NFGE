#include "Precompiled.h"
#include "TextureManager.h"

using namespace NFGE::Graphics;

namespace
{
	std::unique_ptr<TextureManager> sInstance = nullptr;
}

void NFGE::Graphics::TextureManager::StaticInitialize(std::filesystem::path rootPath)
{
	ASSERT(sInstance == nullptr, "[TextureManager] System already initlizlized!");
	sInstance = std::make_unique<TextureManager>();
	sInstance->SetRootPath(std::move(rootPath));
}

void NFGE::Graphics::TextureManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance.reset();
	}
}

TextureManager* NFGE::Graphics::TextureManager::Get()
{
	ASSERT(sInstance != nullptr, "[TextureManager] System already initlizlized!");
	return sInstance.get();
}

NFGE::Graphics::TextureManager::~TextureManager()
{
	// TODO Do some error checks here, but we need a way to unload sepecific textures first
	// Maybe add the concept of texture group
	for (auto& [key, value] : mInventory)
	{
		value->Terminate();
	}
}

void NFGE::Graphics::TextureManager::SetRootPath(std::filesystem::path rootPath)
{
	ASSERT(std::filesystem::is_directory(rootPath), "[TextureManager] %s is not a directory!", (char*)rootPath.c_str());
	mRootPath = std::move(rootPath);
	//if (mRootPath.back() != L'/')
	//	mRootPath += L'/';
}

TextureId NFGE::Graphics::TextureManager::LoadTexture(std::filesystem::path filename, bool isUsingRootPath)
{
	//auto hash = std::filesystem::hash_value(filename);
	//if (mInventory.find(hash) == mInventory.end())
	//{
	//	auto texture = std::make_unique<Texture>();
	//
	//	if (isUsingRootPath)
	//		texture->Initialize(mRootPath / filename);
	//	else
	//		texture->Initialize(filename);
	//
	//	mInventory.emplace(hash, std::move(texture));
	//}
	//return hash;

	auto hash = std::filesystem::hash_value(filename);
	auto[iter, success] = mInventory.insert({ hash, nullptr });
	if (success)
	{
		iter->second = std::make_unique<Texture>();
		if (isUsingRootPath)
			iter->second->Initialize(mRootPath / filename);
		else
			iter->second->Initialize(filename);
	}
	else
	{
		int a = 0;
	}
	return hash;
}

void TextureManager::Clear()
{
	for (auto& item : mInventory)
	{
		if (item.second)
		{
			item.second->Terminate();
			item.second.release();
			item.second.reset();
		}
	}
	mInventory.clear();
}

Texture * NFGE::Graphics::TextureManager::GetTexture(TextureId textureId)
{
	
	auto iter = mInventory.find(textureId); 
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}

uint32_t NFGE::Graphics::TextureManager::GetSpriteWidth(TextureId textureId)
{
	Texture* texture = TextureManager::Get()->GetTexture(textureId);
	return texture ? texture->GetWidth() : 0u;
}

uint32_t NFGE::Graphics::TextureManager::GetSpriteHeight(TextureId textureId)
{
	Texture* texture = TextureManager::Get()->GetTexture(textureId);
	return texture ? texture->GetHeight() : 0u;
}

void * NFGE::Graphics::TextureManager::GetSprite(TextureId textureId)
{
	Texture* texture = TextureManager::Get()->GetTexture(textureId);
	return texture ? GetShaderResourceView(*texture) : nullptr;
}
