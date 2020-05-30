#include "Precompiled.h"
#include "RasterizerStateManager.h"

using namespace NFGE::Graphics;

namespace
{
	std::unique_ptr<RasterizerStateManager> sInstance = nullptr;
}

void RasterizerStateManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "[BlendStateManager] System already initlizlized!");
	sInstance = std::make_unique<RasterizerStateManager>();
	sInstance->Initialize();
}

void RasterizerStateManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

RasterizerStateManager* NFGE::Graphics::RasterizerStateManager::Get()
{
	ASSERT(sInstance != nullptr, "[RasterizerStateManager] System already initlizlized!");
	return sInstance.get();
}

void NFGE::Graphics::RasterizerStateManager::Initialize()
{
	AddRasterizerState("Solid", RasterizerState::CullMode::Back, RasterizerState::FillMode::Solid);
	AddRasterizerState("Wireframe", RasterizerState::CullMode::Back, RasterizerState::FillMode::Wireframe);
	AddRasterizerState("CullFrontSolid", RasterizerState::CullMode::Front, RasterizerState::FillMode::Solid);
	AddRasterizerState("CullFrontWireframe", RasterizerState::CullMode::Front, RasterizerState::FillMode::Wireframe);
	AddRasterizerState("CullNoneSolid", RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	AddRasterizerState("CullNoneWireframe", RasterizerState::CullMode::None, RasterizerState::FillMode::Wireframe);
}

void NFGE::Graphics::RasterizerStateManager::Terminate()
{
	for (auto&[key, value] : mInventory)
		value->Terminate();
}

bool NFGE::Graphics::RasterizerStateManager::AddRasterizerState(std::string name, RasterizerState::CullMode Cullmode, RasterizerState::FillMode fillMode)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		auto rasterizerState = std::make_unique<RasterizerState>();
		rasterizerState->Initialize(Cullmode, fillMode);
		iter->second = std::move(rasterizerState);
	}
	return success;
}

RasterizerState * NFGE::Graphics::RasterizerStateManager::GetRasterizerState(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}