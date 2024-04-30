#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <thread>
#include <iostream>

class NetConnection
{
public:

	enum class Owner {
		server,
		client
	};

	NetConnection(Owner owner, asio::io_context& context, asio::ip::tcp::socket socket);

	~NetConnection();

	void ConnectToServer(asio::ip::tcp::endpoint endpoint);

	void Disconnect();

	bool IsConnected();

	void Send(std::string message);

	void ReadHeader();

	void WriteHeader(std::string header);

private:
	asio::io_context& context_; //Needed to implement a socket

	std::thread contextThread_;

	asio::ip::tcp::socket socket_;

	Owner owner_;
};

