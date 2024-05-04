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

void NetConnection::Send(const Message message)
{
	asio::post(context_, 
		[this, message] 
		{
			bool bWritingMessage = !messageOutQueue.empty();
			messageOutQueue.push(message);
			if (!bWritingMessage)
			{
				WriteHeader();
			}
		});
}

std::vector<char> sBuffer(50);

void NetConnection::ReadHeader()
{
	// asio::async_read() needs a buffer with the EXACT same size as the message length sended, otherwise
	// the function wont work, and no exception is called.
	asio::async_read(socket_, asio::buffer(&messageInTmp.header, sizeof(MessageHeader)),
		[this](std::error_code errorCode, std::size_t bytesLength) 
		{
			if (!errorCode)
			{
				std::cout << messageInTmp;
				//TODO: mirar qué hacer con los mensajes recibidos, si ponerlos en una cola para procesarlos en server.update() o procesarlos cuando llegan aquí
			}
			else
			{
				std::cout << "Error reading header, error code: " << errorCode  << std::endl;
			}
		});
}

void NetConnection::WriteHeader()
{
	//std::vector<char> hBuffer(header.begin(), header.end());
	asio::async_write(socket_, asio::buffer(&messageOutQueue.front().header, sizeof(MessageHeader)),
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

