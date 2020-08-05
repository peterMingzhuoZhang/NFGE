#ifndef INCLUDE_NFGE_GRAPHICS_RASTERIZERSTATEMANAGER_H
#define INCLUDE_NFGE_GRAPHICS_RASTERIZERSTATEMANAGER_H

#include "RasterizerState.h"

namespace NFGE::Graphics
{
	class RasterizerStateManager
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static RasterizerStateManager* Get();

	public:
		RasterizerStateManager() = default;

		void Initialize();
		void Terminate();

		bool AddRasterizerState(std::string name, RasterizerState::CullMode Cullmode, RasterizerState::FillMode fillMode);

		RasterizerState* GetRasterizerState(std::string name);
	private:
		std::unordered_map<std::string, std::unique_ptr<RasterizerState>> mInventory;
	};
}
#endif // !INCLUDE_NFGE_GRAPHICS_RASTERIZERSTATEMANAGER_H
