#include "Precompiled.h"
#include "NetworkUtil.h"

using namespace Network;

bool Network::Initialize()
{
	// Initialize Winsock version 2.2
	WSAData wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
	{
		LOG("Network -- Failed to initialize.");
		return false;
	}
	return true;
}

bool Network::Terminate()
{
	// Shutdown Winsock
	int result = WSACleanup();
	return result == 0;
}
