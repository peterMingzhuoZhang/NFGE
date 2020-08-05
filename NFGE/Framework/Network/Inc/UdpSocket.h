#ifndef INCLUDE_NETWORK_UDPSOCKET_H
#define INCLUDE_NETWORK_UDPSOCKET_H

#include "SocketAddress.h"

namespace Network {

	class UdpSocket
	{
	public:
		UdpSocket() = default;
		~UdpSocket() {};

		bool Open();
		void Close();

		bool Connect(SocketAddress& address);		// For Server
		bool Bind(SocketAddress& address);			// For Client

		bool SetNonBlocking(bool nonBlocking);

		int Send(const uint8_t* buffer, int length);
		int Receive(uint8_t* buffer, int length);

	private:
		bool SetUdpOpt(int opt);
		std::vector<int> n;
		
		UdpSocket(const UdpSocket&) = delete;
		UdpSocket& operator=(const UdpSocket&) = delete;

	private:
		SOCKET mSocket{ INVALID_SOCKET };
		SocketAddress* mSocketAddress{ nullptr };
	};

} // namespace Network

#endif // !INCLUDE_NETWORK_UDPSOCKET_H
