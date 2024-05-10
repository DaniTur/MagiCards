#pragma once
#include <asio.hpp>
#include <thread>
#include "NetConnection.h"
#include "Message.h"
#include "TSQueue.h"
#include "Player.h"

class NetClient
{
public:
	
	NetClient();
	
	~NetClient();

	bool Connect(const std::string& host, const int port);
	
	void Disconnect();
	
	bool IsConnected();

	void Update(size_t nMaxMessages = -1);

	void Send(const Message& message);

	void SendPlayerData(const Player* player);

private:
	
	asio::io_context context_;

	std::thread contextThread_;
	
	asio::ip::tcp::socket socket_;

	std::unique_ptr<NetConnection> connection_;

	TSQueue<Message> messagesInQueue_;

	TSQueue<Message>& IncomingMessageQueue();

	void HandleMessage(Message message);
};

