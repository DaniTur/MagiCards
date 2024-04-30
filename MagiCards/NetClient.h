#pragma once
#include <asio.hpp>
#include <thread>
#include "NetConnection.h"

class NetClient
{
public:
	
	NetClient();
	
	~NetClient();

	bool Connect(const std::string& host, const int port);
	
	void Disconnect();
	
	bool IsConnected();

	void GrabSomeData();

private:
	
	asio::io_context context_;

	std::thread contextThread_;
	
	asio::ip::tcp::socket socket_;

	std::unique_ptr<NetConnection> connection_;
};

