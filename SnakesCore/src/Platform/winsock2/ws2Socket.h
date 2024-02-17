#pragma once
#include "Snakes/Networking/Socket.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

namespace sk {

	class WS2Socket : public Socket
	{
	public:
		WS2Socket(const SocketProps& props);
		WS2Socket(SocketProps::AF af, SocketProps::Type type, SocketProps::Protocol protocol);
		WS2Socket(SOCKET socketDescriptor);
		~WS2Socket();
		static void Init();
		static void Shutdown();

		virtual void Bind(int port) override;
		virtual void Listen(int qlen) override;
		virtual Ref<Socket> Accept() override;

		virtual void Connect(const std::string& ip, int port) override;
		virtual void Disconnect() override;
		virtual bool IsConnected() const override { return m_IsConnected; }

		virtual bool HasIncomingData(long timeout_ms) override;

		virtual void Send(const std::string& msg) override;
		virtual std::string Receive(size_t bytes = 0) override;
		virtual std::string Receive(const std::string& back_string) override;

		virtual void Send(const void* data, size_t size) override;
		virtual int Receive(void* data, size_t size) override;

		virtual void SendTo(const void* data, size_t size, const std::string& ip, int port) override;
		virtual int ReceiveFrom(void* data, size_t size, std::string& ip, int& port) override;

	private:
		void CreateNewSocket(SocketProps::Type type, SocketProps::Protocol protocol);
		void Delete();

	private:
		SOCKET m_SocketDescriptor = INVALID_SOCKET;
		bool m_IsConnected = false;

	private:
		static WSADATA s_WsaData;
	};
}