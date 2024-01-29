#include "Connection.h"
#include "Ws2tcpip.h"

Connection::Connection() : _socket(INVALID_SOCKET){
}

Connection::~Connection() {
	clear();
}

int Connection::startServerConnection() {
	WORD wVersionRequested;
    WSADATA wsaData;
    int wsaErr;

	wVersionRequested = MAKEWORD(2, 2);

	wsaErr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaErr != 0) {
		std::cout << "Error starting WSA with code" << wsaErr;
		std::cout << "Windsock DLL not found" << std::endl;
		return 1;
	}

	_socket = socket(AF_INET, SOCK_STREAM ,IPPROTO_TCP);

	if (_socket == INVALID_SOCKET) {
		std::cout << "Error at socket " << WSAGetLastError() << std::endl;
		clear();
		return 1;
	}

	//Binding socketAddress info to the Socket
	sockaddr_in sockInfo;
	sockInfo.sin_family = AF_INET;
	sockInfo.sin_port = htons(30000);
	InetPton(AF_INET, L"127.0.0.1", &sockInfo.sin_addr.s_addr);

	int bindErr = bind(_socket, (SOCKADDR*)&sockInfo, sizeof(SOCKADDR_IN));
	if (bindErr == SOCKET_ERROR) {
		std::cout << "Error at binding socket " << WSAGetLastError() << std::endl;
		clear();
		return 1;
	}

	//Listen to the socket
	const int MAX_NCONNECTIONS = 1;
	int listenErr = listen(_socket, MAX_NCONNECTIONS);
	if (listenErr == SOCKET_ERROR) {
		std::cout << "Error at listening socket " << WSAGetLastError() << std::endl;
		clear();
		return 1;
	}

	std::pair<std::string, int> socketInfo = getConnectionInfo();
	std::cout << "Your IP address is: " << socketInfo.first << std::endl;
	std::cout << "Your port is: " << socketInfo.second << std::endl;
	std::cout << "Share this with your friends" << std::endl;
	std::cout << "Waiting for a client connection..." << std::endl;

	//Accept connection, TODO: accepter class to handle incoming connections
	SOCKET acceptSocket = accept(_socket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET)
	{
		std::cout << "Error with accept socket: " << WSAGetLastError() << std::endl;
		clear();
		return 1;
	}

	std::cout << "Accepted new client!" << std::endl;

	//TODO: Start client server send-receive data
	// Receive data from client

	return 0;
}

int Connection::startClientConnection(char const* ip, int port) {
	WORD wVersionRequested;
	WSADATA wsaData;
	int wsaErr;

	wVersionRequested = MAKEWORD(2, 2);

	wsaErr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaErr != 0) {
		std::cout << "Error starting WSA with code" << wsaErr;
		std::cout << "Windsock DLL not found" << std::endl;
		return 1;
	}

	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_socket == INVALID_SOCKET) {
		std::cout << "Error at socket " << WSAGetLastError() << std::endl;
		clear();
		return 1;
	}

	std::cout << "Socket created" << std::endl;

	// Server socket info
	sockaddr_in serverSocketInfo;
	serverSocketInfo.sin_family = AF_INET;
	serverSocketInfo.sin_port = htons(30000);
	//InetPton(AF_INET, (PCWSTR)ip, &serverSocketInfo.sin_addr.s_addr);
	InetPton(AF_INET, L"127.0.0.1", &serverSocketInfo.sin_addr.s_addr);

	int connectErr = connect(_socket, (sockaddr*)&serverSocketInfo, sizeof(serverSocketInfo));
	if (connectErr == SOCKET_ERROR) {
		std::cout << "Error connecting to a server socket " << WSAGetLastError() << std::endl;
		clear();
		return 1;
	}

	std::cout << "Connected to the server" << std::endl;

	char* buffer;
	std::memcpy(&buffer, "mefumoseris", sizeof(buffer));

	int sendErr = send(_socket, buffer, sizeof(buffer), NULL);
	if (sendErr == SOCKET_ERROR) {
		std::cout << "Error sending data " << WSAGetLastError() << std::endl;
		clear();
		return 1;
	}

	std::cout << "Data sended" << std::endl;

	return 0;
}

std::pair<std::string, int> Connection::getConnectionInfo() {
	char ip[16];
	unsigned int port;
	sockaddr_in socketInfo;
	socklen_t len = sizeof(socketInfo);

	// Get IP address
	getsockname(_socket, (sockaddr*)&socketInfo, &len);

	// Get port
	inet_ntop(AF_INET, &socketInfo.sin_addr, ip, sizeof(ip));
	port = ntohs(socketInfo.sin_port);

	std::pair<std::string, int> IPport;
	IPport.first = ip;
	IPport.second = port;

	return IPport;
}

void Connection::clear() {
	closesocket(_socket);
	WSACleanup();
}
