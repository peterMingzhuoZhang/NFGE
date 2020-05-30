#include "Precompiled.h"
#include "BlendStateManager.h"

using namespace NFGE::Graphics;

namespace
{
	std::unique_ptr<BlendStateManager> sInstance = nullptr;
}

const std::string Blend::Opaque = "Opaque";
const std::string Blend::AlphaBlend = "AlphaBlend";
const std::string Blend::AlphaPremutiplied = "AlphaPremultiplied";
const std::string Blend::Additive = "Additive";

void BlendStateManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "[BlendStateManager] System already initlizlized!");
	sInstance = std::make_unique<BlendStateManager>();
	sInstance->Initialize();
}

void BlendStateManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

BlendStateManager* NFGE::Graphics::BlendStateManager::Get()
{
	ASSERT(sInstance != nullptr, "[BlendStateManager] System not initlizlized!");
	return sInstance.get();
}

void NFGE::Graphics::BlendStateManager::Initialize()
{
	AddBlendState("Opaque", BlendState::Mode::Opaque);
	AddBlendState("AlphaBlend", BlendState::Mode::AlphaBlend);
	AddBlendState("AlphaPremultiplied", BlendState::Mode::AlphaPremultiplied);
	AddBlendState("Additive", BlendState::Mode::Additive);
}

void NFGE::Graphics::BlendStateManager::Terminate()
{
	for (auto&[key, value] : mInventory)
		value->Terminate();
}

bool NFGE::Graphics::BlendStateManager::AddBlendState(std::string name, BlendState::Mode mode)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		auto blendState = std::make_unique<BlendState>();
		blendState->Initialize(mode);
		iter->second = std::move(blendState);
	}
	return success;
}

BlendState * NFGE::Graphics::BlendStateManager::GetBlendState(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}
