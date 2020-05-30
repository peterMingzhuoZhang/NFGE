#include "Precompiled.h"
#include "DepthStencilStateManager.h"

using namespace NFGE::Graphics;

namespace
{
	std::unique_ptr<DepthStencilStateManager> sInstance = nullptr;
}

void NFGE::Graphics::DepthStencilStateManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "[DepthStencilStateManager] System already initlizlized!");
	sInstance = std::make_unique<DepthStencilStateManager>();
	sInstance->Initialize();
}

void NFGE::Graphics::DepthStencilStateManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

DepthStencilStateManager * NFGE::Graphics::DepthStencilStateManager::Get()
{
	ASSERT(sInstance != nullptr, "[DepthStencilStateManager] System already initlizlized!");
	return sInstance.get();
}

void NFGE::Graphics::DepthStencilStateManager::Initialize()
{
	AddDepthStencilState("Default", DepthStencilState::StateMode::DepthDefault);
	AddDepthStencilState("None", DepthStencilState::StateMode::DepthNone);
	AddDepthStencilState("Read", DepthStencilState::StateMode::DepthRead);
}

void NFGE::Graphics::DepthStencilStateManager::Terminate()
{
	for (auto&[key, value] : mInventory)
		value->Terminate();
}

bool NFGE::Graphics::DepthStencilStateManager::AddDepthStencilState(std::string name, DepthStencilState::StateMode stateMode)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		auto depthStencilState = std::make_unique<DepthStencilState>();
		depthStencilState->Initialize(stateMode);
		iter->second = std::move(depthStencilState);
	}
	return success;
}

DepthStencilState * NFGE::Graphics::DepthStencilStateManager::GetDepthStencilState(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}
