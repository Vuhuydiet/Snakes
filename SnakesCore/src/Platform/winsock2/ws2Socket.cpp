#include "skpch.h"
#include "WS2Socket.h"

#define _MAX_SIZE_PER_SEND			(1LL << 10) // 1KB
#define _MAX_SIZE_PER_RECEIVE		(1LL << 10) // 1KB

#define _RECEIVE_ERROR				"##_receive_error"

namespace sk {

	WSADATA WS2Socket::s_WsaData;

	static const std::map<SocketProps::AF, int> s_ToWs2Af =
	{ 
		{ SocketProps::AF::NONE, -1 }, 
		{ SocketProps::AF::INET, AF_INET } 
	};

	static const std::map<SocketProps::Type, int> s_ToWs2Type =
	{ 
		{ SocketProps::Type::NONE, -1 }, 
		{ SocketProps::Type::_SOCK_STREAM, SOCK_STREAM }, 
		{ SocketProps::Type::_SOCK_DGRAM, SOCK_DGRAM } 
	};
	
	static const std::map<SocketProps::Protocol, int> s_ToWs2Protocol =
	{ 
		{ SocketProps::Protocol::NONE, -1 }, 
		{ SocketProps::Protocol::TCP, IPPROTO_TCP }, 
		{ SocketProps::Protocol::UDP, IPPROTO_UDP } 
	};

	void WS2Socket::Init() {
		int iResult = WSAStartup(MAKEWORD(2, 2), &s_WsaData);
		SK_ASSERT(iResult != 0, "WSAStartup failed: {0}", iResult);
	}

	void WS2Socket::Shutdown() {
		WSACleanup();
	}

	WS2Socket::WS2Socket(const SocketProps& props)
		: Socket(props)
	{
		CreateNewSocket(props.type, props.protocol);
	}

	WS2Socket::WS2Socket(SocketProps::AF af, SocketProps::Type type, SocketProps::Protocol protocol)
		: Socket(af)
	{
		CreateNewSocket(type, protocol);
	}

	WS2Socket::WS2Socket(SOCKET socketDescriptor)
		: m_SocketDescriptor(socketDescriptor), Socket(SocketProps::AF::INET)
	{
		m_IsConnected = true;
	}

	WS2Socket::~WS2Socket() {
		if (m_SocketDescriptor != INVALID_SOCKET)
			Disconnect();
	}

	void WS2Socket::CreateNewSocket(SocketProps::Type type, SocketProps::Protocol protocol) {
		m_SocketDescriptor = socket(s_ToWs2Af.at(m_Af), s_ToWs2Type.at(type), s_ToWs2Protocol.at(protocol));

		if (m_SocketDescriptor == INVALID_SOCKET) {
			SK_ERROR("Socket failed with error: {0}", WSAGetLastError());
			return;
		}
	}

	void WS2Socket::Delete() {
		closesocket(m_SocketDescriptor);
		m_SocketDescriptor = INVALID_SOCKET;
		m_Af = SocketProps::AF::NONE;
		m_IsConnected = false;
	}

	void WS2Socket::Bind(int port) {
		sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));

		sin.sin_family = s_ToWs2Af.at(m_Af);
		sin.sin_port = htons(port);
		sin.sin_addr.s_addr = INADDR_ANY;

		int iResult = bind(m_SocketDescriptor, (sockaddr*) & sin, sizeof(sin));
		if (iResult == SOCKET_ERROR) {
			SK_ERROR("Bind failed with error: {0}", WSAGetLastError());
			Delete();
			return;
		}
	}

	void WS2Socket::Listen(int qlen) {
		int iResult = listen(m_SocketDescriptor, qlen);
		if (iResult == SOCKET_ERROR) {
			SK_ERROR("Listen failed with error: {0}", WSAGetLastError());
			Delete();
			return;
		}
	}

	Ref<Socket> WS2Socket::Accept() {
		SOCKET client_socket = accept(m_SocketDescriptor, nullptr, nullptr);
		if (client_socket == INVALID_SOCKET) {
			SK_ERROR("Accept failed with error: {0}", WSAGetLastError());
			Delete();
			return nullptr;
		}
		return NewRef<WS2Socket>(client_socket);
	}

	void WS2Socket::Connect(const std::string& ip, int port) {
		sockaddr_in client_service;
		client_service.sin_family = s_ToWs2Af.at(m_Af);
		client_service.sin_addr.s_addr = inet_addr(ip.c_str());
		client_service.sin_port = htons(port);

		int iResult = connect(m_SocketDescriptor, (SOCKADDR*)&client_service, sizeof(client_service));
		if (iResult == SOCKET_ERROR) {
			SK_ERROR("Connect failed with error: {0}", WSAGetLastError());
			Delete();
			return;
		}
		m_IsConnected = true;
		std::string svResponse = Receive();
	}

	void WS2Socket::Disconnect()
	{
		if (!m_IsConnected)
			return;

		closesocket(m_SocketDescriptor);
		m_IsConnected = false;
		m_Af = SocketProps::AF::NONE;
		//m_Type = SocketProps::Type::NONE, m_Protocol = SocketProps::Protocol::NONE;
	}

	void WS2Socket::Send(const std::string& msg) {
		SK_ASSERT(!m_IsConnected, "Hasn't connected, can not send message!");

		std::string formatted_msg = msg;
		for (int i = 0; i < formatted_msg.size(); i += _MAX_SIZE_PER_SEND) {
			std::string sent_string = formatted_msg.substr(i, _MAX_SIZE_PER_SEND);
			int iResult = send(m_SocketDescriptor, sent_string.c_str(), (int)sent_string.size() * sizeof(char), 0);

			if (iResult == SOCKET_ERROR) {
				int error_num = WSAGetLastError();
				SK_ERROR("Send failed : {0}", error_num);
				Delete();
				__debugbreak();
				break;
			}
		}
	}

	bool WS2Socket::HasIncomingData(long timeout_ms) {
		u_long mode = 1;
        if (ioctlsocket(m_SocketDescriptor, FIONBIO, &mode) == SOCKET_ERROR) {
            SK_ERROR("Set socket to non-blocking mode failed: {0}", WSAGetLastError());
			Delete();
			__debugbreak();
            return false;
        }

        // Use select to check for incoming data with a timeout
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(m_SocketDescriptor, &readSet);

        struct timeval timeout;
        timeout.tv_sec = timeout_ms / 1000;
        timeout.tv_usec = (timeout_ms % 1000) * 1000;

		// ready is the number of sockets that have incoming data
        int ready = select(0, &readSet, NULL, NULL, &timeout);

        if (ready == SOCKET_ERROR) {
			SK_ERROR("Socket Select failed: {0}", WSAGetLastError());
			Delete();
            return false;
        }

		return ready > 0;
	}

	std::string WS2Socket::Receive(size_t size) {
		SK_ASSERT(!m_IsConnected, "Hasn't connected! Can not receive message!");

		std::string res;
		char* buffer = new char[_MAX_SIZE_PER_RECEIVE + 1];
		int received_bytes = 0;
		do {
			int iResult = recv(m_SocketDescriptor, buffer, _MAX_SIZE_PER_RECEIVE, 0);
			if (iResult > 0) {
				received_bytes += iResult;
				buffer[iResult] = '\0';
				res += buffer;
			}
			else if (iResult == 0) {
				SK_INFO("Connection closing...");
			}
			else {
				SK_ERROR("Recv failed: {0}", WSAGetLastError());
				Delete();
				return _RECEIVE_ERROR;
			}
		} while (received_bytes < size);
		delete[] buffer;

		return res;
	}

	std::string WS2Socket::Receive(const std::string& back_string) {
		SK_ASSERT(!m_IsConnected, "Hasn't connected! Can not receive message!");

		std::string res;
		char* buffer = new char[_MAX_SIZE_PER_RECEIVE + 1];
		do {
			int iResult = recv(m_SocketDescriptor, buffer, _MAX_SIZE_PER_RECEIVE, 0);
			if (iResult > 0) {
				buffer[iResult] = '\0';
				res += buffer;
				std::string sub_string = res.substr(res.size() - back_string.size());
				if (sub_string == back_string)
					break;
			}
			else if (iResult < 0) {
				SK_ERROR("Recv failed: {0}", WSAGetLastError());
				Delete();
				return _RECEIVE_ERROR;
			}
		} while (true);
		delete[] buffer;

		return res;
	}

	void WS2Socket::Send(const void* data, size_t size)
	{
		SK_ASSERT(!m_IsConnected, "Hasn't connected, can not send message!");

		for (int i = 0; i < size; i += _MAX_SIZE_PER_SEND) {
			int iResult = send(m_SocketDescriptor, (char*)data + i, _MAX_SIZE_PER_SEND, 0);

			if (iResult == SOCKET_ERROR) {
				int error_num = WSAGetLastError();
				SK_ERROR("Send failed : {0}", error_num);
				Delete();
				__debugbreak();
				break;
			}
		}
	}

	int WS2Socket::Receive(void* data, size_t size) {
		SK_ASSERT(!m_IsConnected, "Hasn't connected! Can not receive message!");

		std::string res;
		char* buffer = new char[_MAX_SIZE_PER_RECEIVE + 1];
		int received_bytes = 0;
		do {
			int iResult = recv(m_SocketDescriptor, buffer, _MAX_SIZE_PER_RECEIVE, 0);
			if (iResult > 0) {
				received_bytes += iResult;
				buffer[iResult] = '\0';
				res += buffer;
			}
			else if (iResult < 0) {
				SK_ERROR("Recv failed: {0}", WSAGetLastError());
				Delete();
				return -1;
			}
		} while (received_bytes < size);
		delete[] buffer;
		memcpy(data, res.c_str(), size);

		return received_bytes;
	}

	void WS2Socket::SendTo(const void* data, size_t size, const std::string& ip, int port) {
		sockaddr_in client_service;
		memset(&client_service, 0, sizeof(client_service));
		client_service.sin_family = s_ToWs2Af.at(m_Af);
		client_service.sin_addr.s_addr = inet_addr(ip.c_str());
		client_service.sin_port = htons(port);

		int iResult = sendto(m_SocketDescriptor, (char*)data, (int)size, 0, (sockaddr*)&client_service, sizeof(client_service));
		if (iResult == SOCKET_ERROR) {
			SK_ERROR("Send failed : {0}", WSAGetLastError());
			Delete();
			__debugbreak();
		}
	}

	int WS2Socket::ReceiveFrom(void* data, size_t size, std::string& ip, int& port) {
		sockaddr_in client_service;
		int client_service_size = sizeof(client_service);
		
		char* buffer = new char[size + 1];
		int received_bytes = recvfrom(m_SocketDescriptor, buffer, size, 0, (sockaddr*)&client_service, &client_service_size);
		
		if (received_bytes < 0) {
			SK_ERROR("Recv failed: {0}", WSAGetLastError());
			Delete();
			return -1;
		}
		
		buffer[received_bytes] = '\0';
		memcpy(data	, buffer, received_bytes);
		delete[] buffer;

		ip = inet_ntoa(client_service.sin_addr);
		port = ntohs(client_service.sin_port);

		return received_bytes;
	}
}






/*

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define TIMEOUT_MS 1000 // Timeout in milliseconds

int main() {
	WSADATA wsa;
	SOCKET serverSocket;
	struct sockaddr_in serverAddr, clientAddr;
	int clientAddrSize = sizeof(clientAddr);
	char buffer[1024];

	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		perror("WSAStartup failed");
		return 1;
	}

	// Create socket
	serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		perror("Socket creation failed");
		WSACleanup();
		return 1;
	}

	// Set up server address information
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);

	// Bind the socket
	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		perror("Bind failed");
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}

	printf("UDP Server listening on port %d...\n", PORT);

	while (1) {
		// Set socket to non-blocking mode
		u_long mode = 1;
		ioctlsocket(serverSocket, FIONBIO, &mode);

		// Use select to check for incoming data with a timeout
		fd_set readSet;
		FD_ZERO(&readSet);
		FD_SET(serverSocket, &readSet);

		struct timeval timeout;
		timeout.tv_sec = TIMEOUT_MS / 1000;
		timeout.tv_usec = (TIMEOUT_MS % 1000) * 1000;

		int ready = select(0, &readSet, NULL, NULL, &timeout);
		
		if (ready == 0) {
			// No data received within the timeout
			printf("No data received within the timeout.\n");
			continue;  // Continue to the next iteration
		}

		// Receive data from the client
		int bytesRead = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &clientAddrSize);

		// Process and respond to the client
		buffer[bytesRead] = '\0';
		printf("Received from client (%s:%d): %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);

		// Example: Echo back to the client
		sendto(serverSocket, buffer, bytesRead, 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
	}

	closesocket(serverSocket);
	WSACleanup();
	return 0;
}
*/