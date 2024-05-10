#include "NetClient.h"
#include <iostream>
#include <vector>


NetClient::NetClient() : socket_(context_)
{

}

NetClient::~NetClient()
{
	// Clean disconnection
	Disconnect();
}

bool NetClient::Connect(const std::string& host, const int port)
{
	try
	{
		connection_ = std::make_unique<NetConnection>(NetConnection::Owner::client, context_, asio::ip::tcp::socket(context_), messagesInQueue_);

		asio::error_code errCode;
		asio::ip::address ipAddress = asio::ip::make_address(host, errCode);

		if (errCode)
		{
			std::cout << "Error making address from a string: " << errCode.message() << std::endl;
		}
		else
		{
			asio::ip::tcp::endpoint endpoint(ipAddress, port);
			connection_->ConnectToServer(endpoint);

			contextThread_ = std::thread([this]() { context_.run(); });
		}

	}
	catch (const std::exception& e)
	{
		std::cout << "Client exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

void NetClient::Disconnect()
{
	if (IsConnected())
	{
		connection_->Disconnect();
	}

	context_.stop();

	//Wait for thread to stop and close
	if (contextThread_.joinable())
	{
		contextThread_.join();
	}

	connection_.release();
}

bool NetClient::IsConnected()
{
	if (connection_)
	{
		return connection_->IsConnected();
	}
	else
	{
		return false;
	}
}

// nMaxMessages: number of maximum messages processed per processing window (per frame)
void NetClient::Update(size_t nMaxMessages)
{
	if (IsConnected())
	{
		size_t nProcessedMessages = 0;
		while (nProcessedMessages < nMaxMessages && !messagesInQueue_.empty())
		{
			Message msg = messagesInQueue_.pop_front();
			HandleMessage(msg);
			nProcessedMessages++;
		}
	}
}

void NetClient::Send(const Message& message)
{
	if (IsConnected())
	{
		connection_->Send(message);
	}
}

void NetClient::SendPlayerData(const Player* player)
{
	Message msg;
	msg.header.id = MessageType::PlayerData;
	msg << "name:" << player->getName().data() << ",deck:" << player->getDeck();
	Send(msg);
}

void NetClient::HandleMessage(Message message)
{
	if (IsConnected())
	{
		switch (message.header.id)
		{
		case MessageType::ServerAccept:
			std::cout << "Connected to server!" << std::endl;
			break;
		case MessageType::ServerError:
			std::cout << "Server error: " << message.body.data() << std::endl;
			break;
		}
	}
}

TSQueue<Message>& NetClient::IncomingMessageQueue()
{
	return messagesInQueue_;
}
