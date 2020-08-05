#ifndef INCLUDED_NFGE_GRAPHICS_SAMPLERMANAGER_H
#define INCLUDED_NFGE_GRAPHICS_SAMPLERMANAGER_H

#include "Sampler.h"

namespace NFGE::Graphics
{
	class SamplerManager
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static SamplerManager* Get();

	public:
		SamplerManager() = default;

		void Initialize();
		void Terminate();

		bool AddSampler(std::string name, Sampler::Filter filter, Sampler::AddressMode addressMode);

		Sampler* GetSampler(std::string name);
	private:
		std::unordered_map<std::string,std::unique_ptr<Sampler>> mInventory;
	};
}


#endif // !INCLUDED_NFGE_GRAPHICS_SAMPLERMANAGER_H
