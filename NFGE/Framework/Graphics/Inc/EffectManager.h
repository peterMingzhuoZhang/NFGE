#ifndef INCLUDE_NFGE_GRAPHICS_EFFECTMANAGER_H
#define INCLUDE_NFGE_GRAPHICS_EFFECTMANAGER_H


#include "PostProcessManager.h"
#include "Effect.h"

namespace NFGE::Graphics
{
	struct RenderContext
	{
		Effect* mUsingEffect = nullptr;
		EffectContext* mEffectContext = nullptr;
		MeshBuffer* mMeshBuffer;
		const Camera& mCamera;
	};

	class EffectManager
	{
	public:
		static void StaticInitialize(std::filesystem::path rootPath);
		static void StaticTerminate();
		static EffectManager* Get(); 
		static int InitialRenderObjectCapacity;
	public:
		EffectManager() = default;
		
		void Initialize(std::filesystem::path rootPath);
		void Terminate();

		template <class EffectType>
		bool AddEffect(std::string name)
		{
			auto[iter, success] = mInventory.try_emplace(name, nullptr);
			if (success)
			{
				auto effect = std::make_unique<EffectType>();
				effect->Initialize();
				iter->second = std::move(effect);
			}
			return success;
		}
		
		bool AddEffect(EffectType effects);
		
		Effect* GerEffect(std::string);
		Effect* GerEffect(EffectType effects);
		const std::filesystem::path& GetRootPath() const { return mRootPath; }

		void RegisterRenderObject(Effect* usingEffect, EffectContext* contextPtr, MeshBuffer* meshBufferPtr, const Camera& camera);
		void ClearRenderObject();

		void PreparePostProcessEffect();
		void MainRender();
		void RunPostProcessEffect();
	private:
		
		std::unordered_map<std::string, std::unique_ptr<Effect>> mInventory;

		std::vector<RenderContext> mRenderObjectList;// TODO:: Change to four pointer vector for more optimasion
		std::filesystem::path mRootPath;
	};

}

#endif // !INCLUDE_NFGE_GRAPHICS_EFFECTMANAGER_H

