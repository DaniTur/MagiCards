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
		connection_ = std::make_unique<NetConnection>(NetConnection::Owner::client, context_, asio::ip::tcp::socket(context_));

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

			//TODO
			//std::string sRequest = "im this player [player2], i want to join the game room";

			//socket_.write_some(asio::buffer(sRequest.data(), sRequest.size()));

			//GrabSomeData();

			std::cout << "connecting to the server..." << std::endl;

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

std::vector<char> vBuffer(1024);

void NetClient::GrabSomeData()
{

	socket_.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
		[&](std::error_code errorCode, std::size_t bytesRead)
		{
			if (!errorCode)
			{
				std::cout << "Bytes read: " << bytesRead << std::endl;

				for (int i = 0; i < bytesRead; i++)
					std::cout << vBuffer[i];

				GrabSomeData();
			}
		}
	);
}
