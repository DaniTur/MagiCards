#include "NetServer.h"
#include <iostream>


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
			std::cout << "[SERVER] New Client connected: " << socket.remote_endpoint() << std::endl;

			std::shared_ptr<NetConnection> newConnection = std::make_shared<NetConnection>(context_);

			if (ConnectionIsValid(newConnection)) {
				std::cout << "[SERVER] Connection from: " << socket.remote_endpoint() << " Approved." << std::endl;
				clientConnection_ = std::move(newConnection);
			}
			else
			{
				// newConnection will go out of scope and will be automaticaly deleted(because of shared_ptr)
				std::cout << "[SERVER] Connection from: " << socket.remote_endpoint() << " Denied." << std::endl;
			}
			// Uncomment for handling multiple client connetecions, handle connections in a container,
			// otherwise the last conenction will replace the previous.
			//WaitForClientConnection(); 
		}
		else
		{
			std::cout << "[SERVER] New Connection Error: " << errorCode.message() << std::endl;
		}
	});
}

bool NetServer::ConnectionIsValid(std::shared_ptr<NetConnection> connection)
{
	// Check if the connection is valid(bad source endpoint, banned address, etc)
	return true;
}

bool NetServer::IsRunning()
{
	return running_;
}