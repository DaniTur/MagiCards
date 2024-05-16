#include "NetServer.h"
#include <iostream>
#include <vector>

NetServer::NetServer(int port) 
	: acceptor_(context_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{
}

NetServer::~NetServer()
{
	Stop();
}

bool NetServer::Start()
{
	try
	{
		WaitForClientConnection();

		contextThread_ = std::thread([this]() { context_.run(); });
	}
	catch (const std::exception& e)
	{
		std::cerr << "[SERVER] Exception on Start(): " << e.what() << std::endl;
		return false;
	}

	std::cout << "[SERVER] Started." << std::endl;
	running_ = true;
	return true;
}

void NetServer::Stop()
{
	context_.stop();

	//Wait for thread to stop and close
	if (contextThread_.joinable())
	{
		contextThread_.join();
	}
	std::cout << "[SERVER] Stopped." << std::endl;
	running_ = false;
}

void NetServer::WaitForClientConnection()
{
	acceptor_.async_accept([this](std::error_code errorCode, asio::ip::tcp::socket socket) {
		if (!errorCode)
		{
			std::cout << "[SERVER] New Client connection: " << socket.remote_endpoint() << std::endl;

			std::shared_ptr<NetConnection> newConnection = 
				std::make_shared<NetConnection>(NetConnection::Owner::server, context_, std::move(socket), messegesInQueue_);

			// Check the new connection
			if (OnClientConnect(newConnection)) 
			{
				std::cout << "[SERVER] Connection Approved." << std::endl;
				clientConnection_ = std::move(newConnection);

				// Listen for incoming messages from the accepted connection
				clientConnection_->ConnectToClient();
			}
			else
			{
				// newConnection will go out of scope and will be automaticaly deleted(because of shared_ptr)
				std::cout << "[SERVER] Connection Denied, server is full." << std::endl;
			}

		}
		else
		{
			std::cout << "[SERVER] New Connection Error: " << errorCode.message() << std::endl;
		}

		// For handling multiple client connetecions, a connections container is needed,
		// otherwise the last conenction will replace the previous one.
		WaitForClientConnection(); 
	});
}


bool NetServer::OnClientConnect(std::shared_ptr<NetConnection> clientConnection)
{
	// Check if server is full of connections, for now 1 maximum client connection allowed
	if (!clientConnection_)
	{
		Message message;
		message.header.id = MessageType::ServerAccept;
		clientConnection->Send(message);
		return true;	
	}
	else
	{ 
		Message message;
		message.header.id = MessageType::ServerError;
		message << "Connection Denied, server is full";
		clientConnection->Send(message);
		return false;
	}

}

// nMaxMessages: number of maximum messages processed per processing window (per frame)
std::vector<Message> NetServer::GetMessagesToUpdate(size_t nMaxMessages = -1)
{
	std::vector<Message> messagesToUpdate;
	size_t nProcessedMessages = 0;
	while (nProcessedMessages < nMaxMessages && !messegesInQueue_.empty())
	{
		messagesToUpdate.push_back(messegesInQueue_.pop_front());
		nProcessedMessages++;
	}
	return messagesToUpdate;
}

void NetServer::MessageClient(Message message)
{
	if (clientConnection_ && clientConnection_->IsConnected())
	{
		clientConnection_->Send(message);
	}
	else
	{
		clientConnection_.reset();
	}
}

bool NetServer::IsRunning()
{
	return running_;
}

