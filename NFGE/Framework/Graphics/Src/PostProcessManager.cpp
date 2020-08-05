#include "Precompiled.h"
#include "PostProcessManager.h"

using namespace NFGE::Graphics;
using namespace NFGE::Graphics::PostProcesses;

namespace
{
	std::unique_ptr<PostProcessManager> sInstance = nullptr;
}

void NFGE::Graphics::PostProcessManager::StaticInitialize(std::filesystem::path rootPath, uint32_t width, uint32_t height, Camera* mainCamera, DirectionalLight* mainLight, RenderTarget::Format format)
{
	ASSERT(sInstance == nullptr, "[PostProcessManager] System already initlizlized!");
	sInstance = std::make_unique<PostProcessManager>();
	sInstance->Initialize(rootPath, width, height, mainCamera, mainLight, format);
}

void NFGE::Graphics::PostProcessManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

PostProcessManager * NFGE::Graphics::PostProcessManager::Get()
{
	ASSERT(sInstance != nullptr, "[PostProcessManager] System already initlizlized!");
	return sInstance.get();
}

void NFGE::Graphics::PostProcessManager::Initialize(std::filesystem::path rootPath, uint32_t width, uint32_t height, Camera* mainCamera, DirectionalLight* mainLight, RenderTarget::Format format)
{
	mRootPath = rootPath;
	AddPostProcessUnit(PostProcessType::None);
	AddPostProcessUnit(PostProcessType::Pixelate);
	AddPostProcessUnit(PostProcessType::Shadow);
	AddPostProcessUnit(PostProcessType::DepthBlur);

	mMasterRenderTarget.Initialize(width, height, format);
	mCurrentCamera = mainCamera;
	mCurrentLight = mainLight;
}

void NFGE::Graphics::PostProcessManager::Terminate()
{
	for (auto&[key, value] : mInventory)
		value->Terminate();
	mMasterRenderTarget.Terminate();
}

bool NFGE::Graphics::PostProcessManager::AddPostProcessUnit(PostProcessType postProcessType)
{
	bool ret = false;
	switch (postProcessType)
	{
	case NFGE::Graphics::PostProcessType::None:
		ret = AddPostProcessUnit<PostProcessUnit_None>("PostProcessUnit_None");
		break;
	case NFGE::Graphics::PostProcessType::Pixelate:
		ret = AddPostProcessUnit<PostProcessUnit_Pixelate>("PostProcessUnit_Pixelate");
		break;
	case NFGE::Graphics::PostProcessType::Shadow:
		ret = AddPostProcessUnit<PostProcessUnit_Shadow>("PostProcessUnit_Shadow");
		break;
	case NFGE::Graphics::PostProcessType::DepthBlur:
		ret = AddPostProcessUnit<PostProcessUnit_DepthBlur>("PostProcessUnit_DepthBlur");
		break;
	default:
		break;
	}

	return ret;
}

PostProcessUnit * NFGE::Graphics::PostProcessManager::GetPostProcessUnit(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}

PostProcessUnit * NFGE::Graphics::PostProcessManager::GetPostProcessUnit(PostProcessType postProcessType)
{
	PostProcessUnit* ret = nullptr;

	switch (postProcessType)
	{
	case NFGE::Graphics::PostProcessType::None:
		ret = GetPostProcessUnit("PostProcessUnit_None");
		break;
	case NFGE::Graphics::PostProcessType::Pixelate:
		ret = GetPostProcessUnit("PostProcessUnit_Pixelate");
		break;
	case NFGE::Graphics::PostProcessType::Shadow:
		ret = GetPostProcessUnit("PostProcessUnit_Shadow");
		break;
	case NFGE::Graphics::PostProcessType::DepthBlur:
		ret = GetPostProcessUnit("PostProcessUnit_DepthBlur");
		break;
	default:
		break;
	}

	return ret;
}

void NFGE::Graphics::PostProcessManager::FinalToScreenRender()
{
	PostProcessUnit_None* processUnit = dynamic_cast<PostProcessUnit_None*>(GetPostProcessUnit("PostProcessUnit_None"));
	ASSERT(processUnit != nullptr, "[PostProcessManager] Fail to get PostProcessUnit_None from mInventory");

	processUnit->ToScreenRender();
}


PostProcessUnit* NFGE::Graphics::PostProcessManager::ActivePostProcessUnit(PostProcessType postProcessType)
{
	PostProcessUnit* ret = nullptr;
	switch (postProcessType)
	{
	case NFGE::Graphics::PostProcessType::None:
		ret = GetPostProcessUnit("PostProcessUnit_None");
		break;
	case NFGE::Graphics::PostProcessType::Pixelate:
		ret = GetPostProcessUnit("PostProcessUnit_Pixelate");
		break;
	case NFGE::Graphics::PostProcessType::Shadow:
		ret = GetPostProcessUnit("PostProcessUnit_Shadow");
		break;
	case NFGE::Graphics::PostProcessType::DepthBlur:
		ret = GetPostProcessUnit("PostProcessUnit_DepthBlur");
		break;
	default:
		break;
	}

	mCurrentActivePostProcess.push_back(ret);
	return ret;
}

void NFGE::Graphics::PostProcessManager::OptionUI()
{
	ImGui::Begin("PostProcessing Manager");

	if (ImGui::TreeNode("Add PostProcess Unit"))
	{
		for (auto& element : mInventory)
		{
			if (ImGui::Selectable(element.first.c_str(), false))
			{
				element.second.get()->Active();
				mCurrentActivePostProcess.push_back(element.second.get());
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Active PostProcess Unit"))
	{
		std::vector<PostProcessUnit*>::iterator iter = mCurrentActivePostProcess.begin();
		int index = 0;
		for (; iter != mCurrentActivePostProcess.end();)
		{
			std::string displayName = (*iter)->GetName() + "_" + std::to_string(index);
			if (ImGui::Selectable(displayName.c_str(), false))
			{
				(*iter)->Disable();
				iter = mCurrentActivePostProcess.erase(iter);
			}
			else
			{
				if (ImGui::TreeNode("Options"))
				{
					(*iter)->OptionUI();
					ImGui::TreePop();
				}
				++iter;
			}
			++index;
			
		}

		ImGui::TreePop();
	}


	ImGui::End();
}
