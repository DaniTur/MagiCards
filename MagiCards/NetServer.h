#pragma once
#include <asio.hpp>
#include "NetConnection.h"
#include "Message.h"

class NetServer
{
public:
	NetServer(int port);

	~NetServer();

	bool Start();

	void Stop();

	// Asynchronous
	void WaitForClientConnection(); 

	void OnMessageReceived();

	void MessageClient(Message message);

	bool ConnectionIsValid(std::shared_ptr<NetConnection> connection);

	bool IsRunning();

private:
	asio::io_context context_;

	std::thread contextThread_;

	asio::ip::tcp::acceptor acceptor_;

	std::shared_ptr<NetConnection> clientConnection_;

	bool running_ = false;
};

