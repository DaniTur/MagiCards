#include "NetConnection.h"
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

NetConnection::NetConnection(Owner owner, asio::io_context& context, asio::ip::tcp::socket socket)
	: context_(context), socket_(std::move(socket))
{
	owner_ = owner;
}

NetConnection::~NetConnection()
{
}

void NetConnection::ConnectToServer(asio::ip::tcp::endpoint endpoint)
{
	if (owner_ == Owner::client)
	{
		socket_.async_connect(endpoint, [this](std::error_code errorCode) {
			if (!errorCode)
			{
				ReadHeader();
			}
		});
		//asio::async_connect(socket_, endpoint, 
		//	[this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
		//	{
		//		if (!ec)
		//		{
		//			ReadHeader();
		//		}
		//	});
	}
}

void NetConnection::Disconnect()
{
	if (IsConnected())
	{
		asio::post(context_, [this]() { socket_.close(); });
	}
}

bool NetConnection::IsConnected()
{
	return socket_.is_open();
}

void NetConnection::Send(std::string message)
{
	asio::post(context_, 
		[this, message] 
		{
			WriteHeader(message);
		});
}

std::vector<char> sBuffer(50);

void NetConnection::ReadHeader()
{
	// asio::async_read() needs a buffer with the EXACT same size as the message length sended, otherwise
	// the function wont work, and no exception is called.
	asio::async_read(socket_, asio::buffer(sBuffer.data(), sBuffer.size()),
		[this](std::error_code errorCode, std::size_t bytesLength) 
		{
			std::cout << "primer caracter del header: " << sBuffer[0] << std::endl;
			std::cout << "bytesLength: " << bytesLength << std::endl;
			if (!errorCode)
			{
				if (bytesLength > 0)
				{
					for (int i = 0; i < bytesLength; i++)
					{
						std::cout << sBuffer[i];
					}
				}
			}
			else
			{
				std::cout << "Error reading header, error code: " << errorCode  << std::endl;
			}
		});
}

void NetConnection::WriteHeader(std::string header)
{
	std::vector<char> hBuffer(header.begin(), header.end());
	asio::async_write(socket_, asio::buffer(hBuffer.data(), hBuffer.size()),
		[this](std::error_code errorCode, std::size_t length) {
			if (!errorCode)
			{
				std::cout << "message header sended" << std::endl;
			}
			else
			{
				std::cout << "Error writing header: errorCode: " << errorCode << std::endl;
			}
		});
}

