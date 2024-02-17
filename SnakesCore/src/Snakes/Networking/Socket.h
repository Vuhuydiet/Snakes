#pragma once

#include <string>

#include "Snakes/Core/Core.h"

namespace sk {

	struct SocketProps {
		enum class AF { NONE, INET };
		enum class Type { NONE, _SOCK_STREAM, _SOCK_DGRAM };
		enum class Protocol { NONE, TCP, UDP };

		AF af = AF::NONE;
		Type type = Type::NONE;
		Protocol protocol = Protocol::NONE;

		SocketProps() = default;
		SocketProps(AF af, Type type, Protocol protocol) : af(af), type(type), protocol(protocol) {}
	};

	class Socket {
	public:
		// correct way to create a socket
		static Ref<Socket> Create(const SocketProps& props);
		static Ref<Socket> Create(SocketProps::AF af, SocketProps::Type type, SocketProps::Protocol protocol);
		
		// to be called by the core application
		static void Init();
		static void Shutdown();
	public:
		virtual ~Socket() = default;
		
		virtual void Bind(int port) = 0;
		virtual void Listen(int qlen = 0) = 0;
		virtual Ref<Socket> Accept() = 0;

		virtual void Connect(const std::string& ip, int port) = 0;
		virtual void Disconnect() = 0;
		virtual bool IsConnected() const = 0;

		virtual bool HasIncomingData(long timeout_ms = 1000/60) = 0;

	public:
		// sending and receiving data
		virtual void Send(const std::string& msg) = 0;
		virtual std::string Receive(size_t bytes = 0) = 0;
		virtual std::string Receive(const std::string& back_string) = 0;

		virtual void Send(const void* data, size_t size) = 0;
		// @param data: pointer to the buffer to receive the incoming data, 0 if just get one receive
		// @param size: length, in bytes, of the received data
		// @return: the number of bytes received, or -1 if an error occurred
		virtual int Receive(void* buffer, size_t size = 0) = 0;

		virtual void SendTo(const void* data, size_t size, const std::string& ip, int port) = 0;
		virtual int ReceiveFrom(void* buffer, size_t size, std::string& ip, int& port) = 0;

	protected:
		Socket(const SocketProps& props);
		Socket(SocketProps::AF af);
	
	protected:
		SocketProps::AF m_Af;
	};
}