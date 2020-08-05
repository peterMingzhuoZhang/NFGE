#ifndef INCLUDE_NFGE_GRAPHICS_DEPTHSTENCILSTATEMANAGER_H
#define INCLUDE_NFGE_GRAPHICS_DEPTHSTENCILSTATEMANAGER_H

#include "DepthStencilState.h"

namespace NFGE::Graphics
{
	class DepthStencilStateManager
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static DepthStencilStateManager* Get();
	public:
		DepthStencilStateManager() = default;

		void Initialize();
		void Terminate();

		bool AddDepthStencilState(std::string name, DepthStencilState::StateMode stateMode);

		DepthStencilState* GetDepthStencilState(std::string name);
	private:
		std::unordered_map<std::string, std::unique_ptr<DepthStencilState>> mInventory;
	};

} // namespace NFGE::Graphics

#endif // !INCLUDE_NFGE_GRAPHICS_DEPTHSTENCILSTATEMANAGER_H
