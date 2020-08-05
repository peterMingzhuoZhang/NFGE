#ifndef INCLUDED_NFGE_GRAPHICS_TEXTUREMANAGER_H
#define INCLUDED_NFGE_GRAPHICS_TEXTUREMANAGER_H

#include "Texture.h"

namespace NFGE::Graphics
{
	using TextureId = size_t;

	class TextureManager
	{
	public:
		static void StaticInitialize(std::filesystem::path rootPath);
		static void StaticTerminate();
		static TextureManager* Get();

	public:
		TextureManager() = default;
		~TextureManager();

		void SetRootPath(std::filesystem::path rootPath);

		TextureId LoadTexture(std::filesystem::path filename, bool isUsingRootPath = true);
		void Clear();
		Texture* GetTexture(TextureId textureId);

		uint32_t GetSpriteWidth(TextureId textureId);
		uint32_t GetSpriteHeight(TextureId textureId);
		void* GetSprite(TextureId textureId);
	private:
		std::filesystem::path mRootPath;
		std::unordered_map<TextureId, std::unique_ptr<Texture>> mInventory;
	};
}

#endif // !INCLUDED_NFGE_GRAPHICS_TEXTUREMANAGER_H
