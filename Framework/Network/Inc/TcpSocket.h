#ifndef INCLUDE_NETWORK_TCPSOCKET_H
#define INCLUDE_NETWORK_TCPSOCKET_H

#include "SocketAddress.h"

namespace Network {

class TcpSocket
{
public:
	TcpSocket() = default;
	~TcpSocket() {};

	bool Open();
	void Close();
	bool SetNoDelay(bool noDelay);
	bool SetNonBlocking(bool nonBlocking);

	bool Connect(const SocketAddress& address);
	bool Bind(const SocketAddress& address);
	bool Listen(int backlog = 10);
	std::unique_ptr<TcpSocket> Accept(SocketAddress& fromAddress);

	int Send(const uint8_t* buffer, int length);
	int Receive(uint8_t* buffer, int length);

	char* GetIPAddress() const;
private:
	TcpSocket(const TcpSocket&) = delete;
	TcpSocket& operator=(const TcpSocket&) = delete;

	SOCKET mSocket{ INVALID_SOCKET };
};

} // namespace Network

#endif // !INCLUDE_NETWORK_TCPSOCKET_H
