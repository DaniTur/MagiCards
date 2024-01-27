#pragma once
#include <utility>
#include <iostream>
#include <winsock2.h>

class Connection
{
public:
	Connection();
	~Connection();

	int startServerConnection();

	int startClientConnection(char const *ip, int port);

	std::pair<std::string, int> getConnectionInfo();

	void clear();

private:
	SOCKET _socket;
};

