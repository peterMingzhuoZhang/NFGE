#include "Precompiled.h"
#include "SamplerManager.h"

using namespace NFGE::Graphics;

namespace
{
	std::unique_ptr<SamplerManager> sInstance = nullptr;
}

void SamplerManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "[SamplerManager] System already initlizlized!");
	sInstance = std::make_unique<SamplerManager>();
	sInstance->Initialize();
}

void SamplerManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

SamplerManager* NFGE::Graphics::SamplerManager::Get()
{
	ASSERT(sInstance != nullptr, "[SamplerManager] System already initlizlized!");
	return sInstance.get();
}

void SamplerManager::Initialize()
{
	AddSampler("PointClamp", Sampler::Filter::Point, Sampler::AddressMode::Clamp);
	AddSampler("PointWrap", Sampler::Filter::Point, Sampler::AddressMode::Wrap);
	AddSampler("LinearClamp", Sampler::Filter::Linear, Sampler::AddressMode::Clamp);
	AddSampler("LinearWrap", Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	AddSampler("AnisotropicClamp", Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	AddSampler("AnisotropicWrap", Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
}

void SamplerManager::Terminate()
{
	for (auto& [key, value] : mInventory)
		value->Terminate();
}

bool NFGE::Graphics::SamplerManager::AddSampler(std::string name, Sampler::Filter filter, Sampler::AddressMode addressMode)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		auto sampler = std::make_unique<Sampler>();
		sampler->Initialize(filter, addressMode);
		iter->second = std::move(sampler);
	}
	return success;
}

Sampler * NFGE::Graphics::SamplerManager::GetSampler(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}
