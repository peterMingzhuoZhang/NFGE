#include "Precompiled.h"
#include "LinkingContext.h"

using namespace Network;

int Network::LinkingContext::GetNetworkId(void * instance) const
{
	//ASSERT(mInstanceToNetworkIdMap.find(instance) != mInstanceToNetworkIdMap.end(), "[LinkingCOntext] instance %d not registered.", instance);
	//if (mInstanceToNetworkIdMap.find(instance) == mInstanceToNetworkIdMap.end())
	//	return LinkingContext::InvalidId;
	//else
	//	return mInstanceToNetworkIdMap.at(instance);
	
	auto iter = mInstanceToNetworkIdMap.find(instance);
	return iter == mInstanceToNetworkIdMap.end() ? InvalidId : iter->second;
}

void * Network::LinkingContext::GetInstance(int networkId) const
{
	//ASSERT(mNetworkIdToInstanceMap.find(networkId) != mNetworkIdToInstanceMap.end() && (networkId != LinkingContext::InvalidId), "[LinkingContext] instance %d not registered.", networkId);
	//return mNetworkIdToInstanceMap.at(networkId);
	
	auto iter = mNetworkIdToInstanceMap.find(networkId);
	return iter == mNetworkIdToInstanceMap.end() ? nullptr : iter->second;
}

int Network::LinkingContext::Register(void * instance)
{
	auto id = GetNetworkId(instance);
	ASSERT(id == InvalidId, "[LinkingContext] Instance already registered!");
	id = mNextNetworkId++;
	mInstanceToNetworkIdMap.emplace(instance, id);
	mNetworkIdToInstanceMap.emplace(id, instance);
	return id;

	//while (mNetworkIdToInstanceMap[++mNextNetworkId] != nullptr);
	//mInstanceToNetworkIdMap[instance] = mNextNetworkId;
	//mNetworkIdToInstanceMap[mNextNetworkId] = instance;
	//return mInstanceToNetworkIdMap[instance];
}

void Network::LinkingContext::Register(void * instance, int networkId, bool updateNextId)
{
	//ASSERT(mNetworkIdToInstanceMap[networkId] == nullptr, "[LinkingContext] networkId: %d, has instance. Unregister it before use it.", networkId);
	//mInstanceToNetworkIdMap[instance] = networkId;
	//mNetworkIdToInstanceMap[networkId] = instance;

	auto id = GetNetworkId(instance);
	ASSERT(id == InvalidId, "[LinkingContext] Instance already registered!");
	
	mInstanceToNetworkIdMap.emplace(instance, networkId);
	mNetworkIdToInstanceMap.emplace(networkId, instance);
	
	if (updateNextId)
	{
		mNextNetworkId = networkId + 1;
	}
}

void Network::LinkingContext::Unregister(void * instance)
{
	//int networkId = GetNetworkId(instance);
	//mNetworkIdToInstanceMap.erase(networkId);
	//mInstanceToNetworkIdMap.erase(instance);
	//mNextNetworkId = --networkId;
	
	auto id = GetNetworkId(instance);
	if (id != InvalidId)
	{
		mInstanceToNetworkIdMap.erase(instance);
		mNetworkIdToInstanceMap.erase(id);
	}
}
