#include "Precompiled.h"
#include "UdpSocket.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

bool Network::UdpSocket::Open()
{
	ASSERT(mSocket == INVALID_SOCKET, "Socket already opened!");
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == INVALID_SOCKET)
	{
		LOG("Failed to open socket.");
		return false;
	}
	return true;
}

void Network::UdpSocket::Close()
{
	if (mSocket != INVALID_SOCKET)
	{
		closesocket(mSocket);
		mSocket = INVALID_SOCKET;
	}
}

bool Network::UdpSocket::Connect(SocketAddress & address)
{
	ASSERT(mSocket != INVALID_SOCKET, "Socket not opened!");
	mSocketAddress = &address;
	address.GetIP4() = INADDR_BROADCAST;

	if (SetUdpOpt(SO_BROADCAST) == false)
	{
		LOG("[Network::UDPSocket] Failed to Set UDP option.");
		Close();
		return false;
	}

	int iResult = connect(mSocket, &address.mSockAddr, static_cast<int>(address.GetSize()));
	if (iResult == SOCKET_ERROR)
	{
		LOG("[Network::UDPSocket] Failed to connect socket to host address. Error: %d", WSAGetLastError());
		Close();
		return false;
	}

	return true;
}

bool Network::UdpSocket::Bind(SocketAddress & address)
{
	ASSERT(mSocket != INVALID_SOCKET, "Socket not opened!");
	mSocketAddress = &address;
	address.GetIP4() = htonl(INADDR_ANY);

	if (SetUdpOpt(SO_REUSEADDR) == false)
	{
		LOG("[Network::UDPSocket] Failed to Set UDP option.");
		Close();
		return false;
	}

	int iResult = bind(mSocket, (LPSOCKADDR)&(address.mSockAddr), sizeof(address.mSockAddr));
	if (iResult == SOCKET_ERROR)
	{
		LOG("Failed to bind socket. Error: %d", WSAGetLastError());
		Close();
		return false;
	}
	return true;
}

bool Network::UdpSocket::SetNonBlocking(bool nonBlocking)
{
	ASSERT(mSocket != INVALID_SOCKET, "Socket not opened!");
	u_long arg = nonBlocking ? 1 : 0;
	int iResult = ioctlsocket(mSocket, FIONBIO, &arg);
	if (iResult == SOCKET_ERROR)
	{
		LOG("[Network::TCPSocket] Fail to set non bolcking mode. Error: %d", WSAGetLastError());
		Close();
		return false;
	}
	return true;
}

int Network::UdpSocket::Send(const uint8_t * buffer, int length)
{
	auto addrIn = mSocketAddress->GetAsSockAddrIn();
	int iResult = sendto(mSocket, reinterpret_cast<const char*>(buffer), length, 0, (SOCKADDR *) (addrIn), sizeof(*(addrIn)));
	if (iResult == SOCKET_ERROR)
	{
		printf("Error detected. When send the message. Send fail.\n");
		printf("sendto failed with error: %d\n", WSAGetLastError());
	}
	else
	{
		printf("[Netword::UdpSocket] Message sent.\n");
	}
	return iResult;
}

int Network::UdpSocket::Receive(uint8_t * buffer, int length)
{
	auto addrIn = mSocketAddress->GetAsSockAddrIn();
	int addrInLen = sizeof(*(addrIn));

	int iResult = recvfrom(mSocket, reinterpret_cast<char*>(buffer), length - 1, 0, (SOCKADDR *) (addrIn), &addrInLen);
	if (iResult >= 0)
	{
		return iResult;
	}
	
	int error = WSAGetLastError();
	if (error == WSAEWOULDBLOCK)
	{
		return 0;
	}

	if (error == WSAECONNRESET)
	{
		// this can happen if a client closed and we havent DC'd yet
		// this is the ICMP message being sent back ssaying the port on that computer is closed
		LOG("[Network::UdpSocket] Connection reset.");
		Close();
		return -WSAECONNRESET;
	}

	LOG("[Network::UdpSocket] Error on receive.");
	Close();
	return -error;
}

bool Network::UdpSocket::SetUdpOpt(int opt)
{
	// Set sockopt for broadcast
	bool bOptVal = true;
	int bOptLen = sizeof(bool);
	int iResult = setsockopt(mSocket, SOL_SOCKET, opt, (char *)&bOptVal, bOptLen);
	if (iResult == SOCKET_ERROR)
	{
		printf("setsockopt for SO_BROADCAST failed with error: %u\n", WSAGetLastError());
		return false;
	}
	return true;
}
