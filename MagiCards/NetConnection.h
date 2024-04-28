#pragma once
#include <asio.hpp>
#include <thread>

class NetConnection
{
public:
	NetConnection(asio::io_context& context);

	~NetConnection();

	bool ConnectToServer(asio::ip::tcp::endpoint endpoint);

	void Disconnect();

	bool IsConnected();

	bool Send();

private:
	asio::io_context& context_; //Needed to implement a socket

	std::thread contextThread_;

	asio::ip::tcp::socket remoteSocket_;
};

