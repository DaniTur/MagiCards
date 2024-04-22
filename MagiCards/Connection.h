#pragma once
#include <utility>
#include <iostream>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

class Connection
{
public:
	Connection();
	~Connection();

	int startServerConnection();

	void acceptNextConnection();

	int startClientConnection(std::string ip, int port);

	void clear();

	void closeServerConnection();

	void closeClientConnection();

	bool isServerConnected();

	bool isClientConnected();

private:
	bool _serverConnected = false;
	bool _clientConnected = false;
	//std::shared_ptr<asio::ip::tcp::socket>  _socket; //serverside active socket to communicate with client

	void handle_accept(asio::ip::tcp::socket& socket);
};

