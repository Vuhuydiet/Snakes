#include "skpch.h"
#include "Socket.h"

#include "Platform/WinSock2/WS2Socket.h"

namespace sk {
	Socket::Socket(const SocketProps& props)
		: m_Af(props.af)
	{
	}

	Socket::Socket(SocketProps::AF af)
		: m_Af(af)
	{
	}

	void Socket::Init()
	{
		WS2Socket::Init();
	}

	void Socket::Shutdown()
	{
		WS2Socket::Shutdown();
	}

	Ref<Socket> Socket::Create(const SocketProps& props)
	{
		return NewRef<WS2Socket>(props);
	}

	Ref<Socket> Socket::Create(SocketProps::AF af, SocketProps::Type type, SocketProps::Protocol protocol)
	{
		return NewRef<WS2Socket>(af, type, protocol);
	}
}
