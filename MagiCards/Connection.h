#pragma once
#include <winsock2.h>

class Connection
{
public:
	Connection();
	~Connection();

	int startServerSocket();

	void clear();

private:
	SOCKET _socket;
};

