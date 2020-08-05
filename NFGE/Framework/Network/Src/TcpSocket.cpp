#include "Precompiled.h"
#include  "TcpSocket.h"



using namespace Network;

bool Network::TcpSocket::Open()
{
	ASSERT(mSocket == INVALID_SOCKET, "Socket already opened!");
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
	{
		LOG("Failed to open socket.");
		return false;
	}
	return true;
}

void Network::TcpSocket::Close()
{
	if (mSocket != INVALID_SOCKET)
	{
		closesocket(mSocket);
		mSocket = INVALID_SOCKET;
	}
}

bool Network::TcpSocket::SetNoDelay(bool noDelay) // TCP does combine shiping by default
{
	ASSERT(mSocket != INVALID_SOCKET, "Socket not opened!");
	int delayArg = noDelay ? 1 : 0;
	int iResult = setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&delayArg, sizeof(int));
	if (iResult == SOCKET_ERROR)
	{
		LOG("[Network::TCPSocket] Fail to set no delay. Error: %d", WSAGetLastError());
		Close();
		return false;
	}
	return true;
}

bool Network::TcpSocket::SetNonBlocking(bool nonBlocking)
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

bool Network::TcpSocket::Connect(const SocketAddress & address) 
{
	ASSERT(mSocket != INVALID_SOCKET, "Socket not opened!");
	int iResult = connect(mSocket, &address.mSockAddr, static_cast<int>(address.GetSize()));
	if (iResult == SOCKET_ERROR)
	{
		int lastError = WSAGetLastError();
		if (lastError == WSAEWOULDBLOCK)
		{
			fd_set write, err; //std::vector<FileDescripter>
			FD_ZERO(&write); // Init
			FD_ZERO(&err);
			FD_SET(mSocket, &write);
			FD_SET(mSocket, &err);

			TIMEVAL timeout = { 10, 0 }; // timeout after 10s
			int iResult = select(0, NULL, &write, &err, &timeout);
			if (iResult == 0)
			{
				LOG("[Network::TCPSocket] Connect timed out.");
				return false;
			}
			else
			{
				if (FD_ISSET(mSocket, &write))
					return true;

				if (FD_ISSET(mSocket, &err))
				{
					LOG("[Network::TCPSocket] Select error.");
					return false;
				}
			}
		}
		else
		{
			LOG("[Network::TCPSocket] Failed to connect socket to host address. Error: %d", lastError);
			Close();
			return false;
		}
	}
	return true;
}

bool Network::TcpSocket::Bind(const SocketAddress & address)
{
	int iResult = bind(mSocket, (LPSOCKADDR)&(address.mSockAddr), static_cast<int>(address.GetSize()));
	if (iResult == SOCKET_ERROR)
	{
		LOG("Failed to bind socket. Error: %d", WSAGetLastError());
		Close();
		return false;
	}
	return true;
}

bool Network::TcpSocket::Listen(int backlog)
{
	int iResult = listen(mSocket, backlog);
	if (iResult == SOCKET_ERROR)
	{
		LOG("Failed to listen socket. Error: %d", WSAGetLastError());
		Close();
		return false;
	}
	return true;

}

std::unique_ptr<TcpSocket> Network::TcpSocket::Accept(SocketAddress & fromAddress)
{
	int length = static_cast<int>(fromAddress.GetSize());
	SOCKET newSocket = accept(mSocket, &fromAddress.mSockAddr, &length);
	if (newSocket == INVALID_SOCKET)
	{
		int lastError = WSAGetLastError();
		if (lastError != WSAEWOULDBLOCK)
		{
			LOG("[Network::TCPSocket] Failed to accept connection. Error: %d", lastError);
			Close();
		}
		return nullptr;
	}

	auto newTcpSocket = std::make_unique<TcpSocket>();
	newTcpSocket->mSocket = newSocket;
	return newTcpSocket;
}

int Network::TcpSocket::Send(const uint8_t * buffer, int length)
{
	int iResult = send(mSocket, reinterpret_cast<const char*>(buffer), length, 0);
	if (iResult < 0)
	{
		LOG("Fail to send. Error: %d", WSAGetLastError());
		Close();
	}
	return iResult;
}

int Network::TcpSocket::Receive(uint8_t * buffer, int length)
{
	int byteReceived = recv(mSocket, reinterpret_cast<char*>(buffer), length, 0);
	if (byteReceived == SOCKET_ERROR)
	{
		int lastError = WSAGetLastError();
		if (lastError == WSAEWOULDBLOCK)
		{
			// If we have SOCKET_ERROR because we are non-blocking, just pretend we have 0 zero
			byteReceived = 0;
		}
		else
		{
			LOG("[Network::TCPSocket] Fail to received. Error: %d", lastError);
			Close();
			return SOCKET_ERROR;
		}
	}
	return byteReceived;
}

char * Network::TcpSocket::GetIPAddress() const
{
	sockaddr sa;
	socklen_t sa_len = sizeof(sa);
	getsockname(mSocket, (struct sockaddr *)&sa, (socklen_t *)&sa_len);
	sockaddr_in* s = reinterpret_cast<sockaddr_in*>(&sa);
	return inet_ntoa(s->sin_addr);
}
