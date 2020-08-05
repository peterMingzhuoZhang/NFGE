#ifndef INCLUDE_NETWORK_COMMON_H
#define INCLUDE_NETWORK_COMMON_H

#define WIN32_LEAN_AND_MEAN		// This macro skip some include in <Windows.h>
#define NOMINMAX				// This macro skip some include in <Windows.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Windows.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>


#include <Core/Inc/Core.h>
#include <cstdlib>
#include <mutex>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

#if defined(_CONSOLE)
#define LOGPRINT printf
#else
#define LOGPRINT OutputDebugStringA
#endif



#endif // !INCLUDE_NETWORK_COMMON_H
