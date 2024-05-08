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
#include "Message.h"
#include "TSQueue.h"

class NetConnection
{
public:

	enum class Owner {
		server,
		client
	};

	NetConnection(Owner owner, asio::io_context& context, asio::ip::tcp::socket socket, TSQueue<Message>& messagesQueueIn);

	~NetConnection();

	void ConnectToServer(asio::ip::tcp::endpoint endpoint);

	void Disconnect();

	bool IsConnected();

	void Send(const Message message);

	void ReadHeader();

	void ReadBody();

	void WriteHeader();

private:
	asio::io_context& context_; //Needed to implement a socket

	std::thread contextThread_;

	asio::ip::tcp::socket socket_;

	Owner owner_;

	TSQueue<Message>& messagesInQueue_;
	TSQueue<Message> messagesOutQueue_;

	Message messageInTmp;
};

