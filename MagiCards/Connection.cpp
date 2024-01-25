#include "Connection.h"
#include "Ws2tcpip.h"
#include <iostream>

Connection::Connection() : _socket(INVALID_SOCKET){
}

Connection::~Connection() {
	clear();
}

int Connection::startServerSocket() {
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
	}
	else {
		std::cout << "Waiting for a client connection..." << std::endl;
	}


	return 0;
}

void Connection::clear() {
	closesocket(_socket);
	WSACleanup();
}