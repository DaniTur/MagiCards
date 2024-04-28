#include "NetClient.h"
#include <iostream>

NetClient::NetClient() : socket_(context_)
{

}

NetClient::~NetClient()
{
	Disconnect();
}

bool NetClient::Connect(const std::string& host, const int port)
{
	try
	{
		connection_ = std::make_unique<NetConnection>(context_);

		asio::error_code errCode;
		asio::ip::address ipAddress = asio::ip::make_address(host, errCode);

		if (!errCode)
		{
			asio::ip::tcp::endpoint endpoint(ipAddress, port);
			connection_->ConnectToServer(endpoint);

			contextThread_ = std::thread([this]() { context_.run(); });
		}
		else
		{
			std::cout << "Error making address from a string: " << errCode.message() << std::endl;
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << "Client exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

void NetClient::Disconnect()
{
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
