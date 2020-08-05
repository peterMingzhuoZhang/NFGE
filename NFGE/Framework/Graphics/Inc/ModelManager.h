#ifndef INCLUDED_NFGE_GRAPHICS_MODELMANAGER_H
#define INCLUDED_NFGE_GRAPHICS_MODELMANAGER_H

#include "Model.h"

namespace NFGE::Graphics
{
	using ModelId = size_t;

	class ModelManager
	{
	public:
		static void StaticInitialize(std::filesystem::path rootPath);
		static void StaticTerminate();
		static ModelManager* Get();

	public:
		ModelManager() = default;
		~ModelManager();

		void SetRootPath(std::filesystem::path rootPath);

		ModelId LoadModel(std::filesystem::path filename, bool isUsingRootPath = true);
		void Clear();
		Model* GetModel(ModelId textureId);

		//void* GetSprite(TextureId textureId);
	private:
		std::filesystem::path mRootPath;
		std::unordered_map<ModelId, std::unique_ptr<Model>> mInventory;
	};
}

#endif // !INCLUDED_NFGE_GRAPHICS_MODELMANAGER_H
