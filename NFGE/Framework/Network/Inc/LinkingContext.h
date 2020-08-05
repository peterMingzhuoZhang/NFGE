#ifndef INCLUDE_NETWORK_LINKINGCONTEXT_H
#define INCLUDE_NETWORK_LINKINGCONTEXT_H

namespace Network {

class LinkingContext
{
// A helper clas that paires pointer with ids so 
// we can refer to object instance across a network.
public:
	static constexpr int InvalidId = -1;

public:
	int GetNetworkId(void* instance) const;
	void* GetInstance(int networkId) const;

	// Register an instance to the next network id
	int Register(void* instance);

	// Register an instance to a specific network id
	void Register(void* instance, int networkId, bool updateNextId = false);

	// Unregister an instance
	void Unregister(void* instance);
private:
	std::unordered_map<int, void*> mNetworkIdToInstanceMap;
	std::unordered_map<void*, int> mInstanceToNetworkIdMap;
	int mNextNetworkId = 0;
};
} // namespace Network

#endif // !INCLUDE_NETWORK_LINKINGCONTEXT_H