#pragma once
#include <asio.hpp>
#include "NetConnection.h"
#include "Message.h"
#include "TSQueue.h"

class NetServer
{
public:
	NetServer(int port);

	~NetServer();

	bool Start();

	void Stop();

	// Asynchronous
	void WaitForClientConnection(); 

	void MessageClient(Message message);

	bool OnClientConnect(std::shared_ptr<NetConnection> clientConnection);

	bool IsRunning();

	void Update(size_t nMaxMessages = -1);

private:
	asio::io_context context_;

	std::thread contextThread_;

	asio::ip::tcp::acceptor acceptor_;

	std::shared_ptr<NetConnection> clientConnection_;

	TSQueue<Message> messegesInQueue_;

	bool running_ = false;

	void HandleMessage(std::shared_ptr<NetConnection> client, Message message);
};

