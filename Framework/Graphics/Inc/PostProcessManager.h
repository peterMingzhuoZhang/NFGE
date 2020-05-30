#ifndef INCLUDE_NFGE_GRAPHICS_POSTPROCESSMANAGER_H
#define INCLUDE_NFGE_GRAPHICS_POSTPROCESSMANAGER_H

#include "PostProcessUnit.h"
#include "RenderTarget.h"
#include "Light.h"

namespace NFGE::Graphics
{
	class PostProcessManager
	{
		using PostProcessUnits = std::vector<PostProcessUnit*>;
		using PostProcessUnitsMap = std::unordered_map<std::string, std::unique_ptr<PostProcessUnit>>;

	public:
		static void StaticInitialize(std::filesystem::path rootPath, uint32_t width, uint32_t height, Camera* mainCamera, DirectionalLight* mainLight,  RenderTarget::Format format);
		static void StaticTerminate();
		static PostProcessManager* Get();

	public:
		PostProcessManager() = default;

		void Initialize(std::filesystem::path rootPath, uint32_t width, uint32_t height, Camera* mainCamera, DirectionalLight* mainLight, RenderTarget::Format format);
		void Terminate();

		template <class classType>
		bool AddPostProcessUnit(std::string name)
		{
			auto[iter, success] = mInventory.try_emplace(name, nullptr);
			if (success)
			{
				auto effect = std::make_unique<classType>();
				effect->Initialize();
				iter->second = std::move(effect);
			}
			return success;
		}

		bool AddPostProcessUnit(PostProcessType effects);

		PostProcessUnit* GetPostProcessUnit(std::string);
		PostProcessUnit* GetPostProcessUnit(PostProcessType effects);
		
		template<typename T>
		T* GetPostProcessUnit() 
		{
			T* ret = nullptr;

			switch (T::sGetType())
			{
			case NFGE::Graphics::PostProcessType::None:
				ret = dynamic_cast<T*>(GetPostProcessUnit("PostProcessUnit_None"));
				break;
			case NFGE::Graphics::PostProcessType::Pixelate:
				ret = dynamic_cast<T*>(GetPostProcessUnit("PostProcessUnit_Pixelate"));
				break;
			case NFGE::Graphics::PostProcessType::Shadow:
				ret = dynamic_cast<T*>(GetPostProcessUnit("PostProcessUnit_Shadow"));
				break;
			case NFGE::Graphics::PostProcessType::DepthBlur:
				ret = dynamic_cast<T*>(GetPostProcessUnit("PostProcessUnit_DepthBlur"));
				break;
			default:
				break;
			}

			return ret;
		}

		PostProcessUnitsMap& GetInventory() { return mInventory; };
		PostProcessUnits& GetActivePosProcessUnits() { return mCurrentActivePostProcess; };
		RenderTarget& GetMasterRenderTarget() { return mMasterRenderTarget; };
		RenderTarget* GetLastRenderTargetPtr() { return mLastRenderTarget; };
		Camera* GetCurrentUseCamera() const { return mCurrentCamera; }
		DirectionalLight* GetCurrentUseLight() const { return mCurrentLight; }
		bool GetIsUsingShadow() const { return mIsUsingShadow; }
		const std::filesystem::path& GetRootPath() const { return mRootPath; }

		void SetCurrentUsingCamera(Camera* camera) { mCurrentCamera = camera; }
		void SetIsUsingShadow(bool isUsingShadow) { mIsUsingShadow = isUsingShadow; }
		void SetLastRenderTargetPtr(RenderTarget* renderTargetPtr) { mLastRenderTarget = renderTargetPtr; };

		PostProcessUnit* ActivePostProcessUnit(PostProcessType postProcessType);

		void FinalToScreenRender();

		void OptionUI();

		
	private:
		PostProcessUnitsMap mInventory;

		PostProcessUnits mCurrentActivePostProcess;
		RenderTarget mMasterRenderTarget;
		RenderTarget* mLastRenderTarget;

		Camera* mCurrentCamera;
		DirectionalLight* mCurrentLight;

		bool mIsUsingShadow = false;
		std::filesystem::path mRootPath;

	};

}

#endif // !INCLUDE_NFGE_GRAPHICS_POSTPROCESSMANAGER_H
