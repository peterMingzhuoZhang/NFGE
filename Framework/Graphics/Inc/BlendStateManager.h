#ifndef INCLUDE_NFGE_GRAPHICS_BLENDSTATEMANAGER_H
#define INCLUDE_NFGE_GRAPHICS_BLENDSTATEMANAGER_H

#include "BlendState.h"



namespace NFGE::Graphics
{
	namespace Blend
	{
		extern const std::string Opaque;
		extern const std::string AlphaBlend;
		extern const std::string AlphaPremutiplied;
		extern const std::string Additive;
	}
	class BlendStateManager
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static BlendStateManager* Get();

	public:
		BlendStateManager() = default;

		void Initialize();
		void Terminate();

		bool AddBlendState(std::string name, BlendState::Mode mode);

		BlendState* GetBlendState(std::string name);
	private:
		std::unordered_map<std::string, std::unique_ptr<BlendState>> mInventory;
	};
}


#endif // !INCLUDE_NFGE_GRAPHICS_BLENDSTATEMANAGER_H
