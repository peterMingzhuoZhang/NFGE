#include "Precompiled.h"
#include "EffectManager.h"
#include "PostProcessManager.h"
#include "MeshBuffer.h"
#include "Effect.h"

using namespace NFGE::Graphics;
using namespace NFGE::Graphics::Effects;

namespace
{
	std::unique_ptr<EffectManager> sInstance = nullptr;
}
int EffectManager::InitialRenderObjectCapacity = 10000;

void NFGE::Graphics::EffectManager::StaticInitialize(std::filesystem::path rootPath)
{
	ASSERT(sInstance == nullptr, "[EffectManager] System already initlizlized!");
	sInstance = std::make_unique<EffectManager>();
	sInstance->Initialize(rootPath);
}

void NFGE::Graphics::EffectManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

EffectManager * NFGE::Graphics::EffectManager::Get()
{
	ASSERT(sInstance != nullptr, "[EffectManager] System already initlizlized!");
	return sInstance.get();
}

void NFGE::Graphics::EffectManager::Initialize(std::filesystem::path rootPath)
{
	mRootPath = rootPath;
	AddEffect<OnScreenEffect>("OnScreenEffect");
	AddEffect<ColorMeshEffect>("ColorMeshEffect");
	AddEffect<TextureMeshEffect>("TextureMeshEffect");
	AddEffect<StandardMeshEffect>("StandardMeshEffect");
	AddEffect<ModelMeshEffect>("ModelMeshEffect");
	AddEffect<SkeletonModelMeshEffect>("SkeletonModelMeshEffect");

	mRenderObjectList.reserve(InitialRenderObjectCapacity);
}

void NFGE::Graphics::EffectManager::Terminate()
{
	for (auto&[key, value] : mInventory)
		value->Terminate();
}

bool NFGE::Graphics::EffectManager::AddEffect(EffectType effects)
{
	bool ret = false;
	switch (effects)
	{
	case NFGE::Graphics::EffectType::OnScreen:
		ret = AddEffect<OnScreenEffect>("OnScreenEffect");
		break;
	case NFGE::Graphics::EffectType::ColorMesh:
		ret = AddEffect<ColorMeshEffect>("ColorMeshEffect");
		break;
	case NFGE::Graphics::EffectType::TextureMesh:
		ret = AddEffect<TextureMeshEffect>("TextureMeshEffect");
		break;
	case NFGE::Graphics::EffectType::StandardMesh:
		ret = AddEffect<StandardMeshEffect>("StandardMeshEffect");
		break;
	case NFGE::Graphics::EffectType::ModelMesh:
		ret = AddEffect<ModelMeshEffect>("ModelMeshEffect");
		break;
	case NFGE::Graphics::EffectType::SkeletonModelMesh:
		ret = AddEffect<SkeletonModelMeshEffect>("SkeletonModelMesh");
		break;
	default:
		break;
	}

	return ret;
}

Effect * NFGE::Graphics::EffectManager::GerEffect(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}

Effect * NFGE::Graphics::EffectManager::GerEffect(EffectType effects)
{
	Effect* ret = nullptr;

	switch (effects)
	{
	case NFGE::Graphics::EffectType::OnScreen:
		ret = GerEffect("OnScreenEffect");
		break;
	case NFGE::Graphics::EffectType::ColorMesh:
		ret = GerEffect("ColorMeshEffect");
		break;
	case NFGE::Graphics::EffectType::TextureMesh:
		ret = GerEffect("TextureMeshEffect");
		break;
	case NFGE::Graphics::EffectType::StandardMesh:
		ret = GerEffect("StandardMeshEffect");
		break;
	case NFGE::Graphics::EffectType::ModelMesh:
		ret = GerEffect("ModelMeshEffect");
		break;
	case NFGE::Graphics::EffectType::SkeletonModelMesh:
		ret = GerEffect("SkeletonModelMeshEffect");
		break;
	default:
		break;
	}

	return ret;
}

void NFGE::Graphics::EffectManager::RegisterRenderObject(Effect* usingEffect, EffectContext * contextPtr, MeshBuffer * meshBufferPtr, const Camera & camera)
{
	mRenderObjectList.emplace_back(RenderContext{ usingEffect, contextPtr, meshBufferPtr, camera});
	for (auto usingPostProcessUnit : PostProcessManager::Get()->GetActivePosProcessUnits())
	{
		usingPostProcessUnit->RegisterAffectObject(&mRenderObjectList.back());
	}
}

void NFGE::Graphics::EffectManager::ClearRenderObject()
{
	mRenderObjectList.clear();
	for (auto& element : PostProcessManager::Get()->GetInventory())
	{
		element.second->ClearRenderObject();
	}
}

void NFGE::Graphics::EffectManager::PreparePostProcessEffect()
{
	for (auto postProcessUnit : PostProcessManager::Get()->GetActivePosProcessUnits())
	{
		postProcessUnit->PreProcess();
	}
}

void NFGE::Graphics::EffectManager::MainRender()
{
	for (auto& element : mRenderObjectList)
	{
		element.mMeshBuffer->Render(element.mUsingEffect, element.mEffectContext, element.mCamera);
	}
}

void NFGE::Graphics::EffectManager::RunPostProcessEffect()
{
	for (auto postProcessUnit : PostProcessManager::Get()->GetActivePosProcessUnits())
	{
		postProcessUnit->PostProcess();
	}
}
