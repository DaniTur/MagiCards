#include "NetConnection.h"
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

NetConnection::NetConnection(Owner owner, asio::io_context& context, asio::ip::tcp::socket socket, TSQueue<Message>& messagesQueueIn)
	: context_(context), socket_(std::move(socket)), messagesInQueue_(messagesQueueIn)
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
		// Version for DNS resolver endpoints 
		//asio::async_connect(socket_, endpoints, 
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
			bool bWritingMessage = !messagesOutQueue_.empty();
			messagesOutQueue_.push_back(message);
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
				// Check if the message has a body
				if (messageInTmp.header.size > sizeof(MessageHeader))
				{
					// make room in temporary message for body data read from the socket 
					messageInTmp.body.resize(messageInTmp.header.size - sizeof(MessageHeader));
					ReadBody();
				}
				else
				{
					// Add the message to the queue, and get ready to read next message
					messagesInQueue_.push_back(messageInTmp);
					ReadHeader(); // provides to the io_context the task to read next incoming message
				}
			}
			else
			{
				std::cout << "Error reading header, error code: " << errorCode  << std::endl;
				std::cout << messageInTmp << std::endl;
				std::cout << ". Socket will be closed" << std::endl;
				socket_.close();
			}
		});
}

void NetConnection::WriteHeader()
{
	asio::async_write(socket_, asio::buffer(&messagesOutQueue_.front().header, sizeof(MessageHeader)),
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

void NetConnection::ReadBody()
{
	asio::async_read(socket_, asio::buffer(messageInTmp.body.data(), messageInTmp.body.size()),
		[this](std::error_code errorCode, std::size_t bytesLength) 
		{
			if (!errorCode)
			{	
				// Add the message to to queue, and get ready to read next message
				std::cout << " Body data: " << messageInTmp.body.data() << std::endl; //TODO: remove
				messagesInQueue_.push_back(messageInTmp);
				ReadHeader(); // provides to the io_context the task to read next incoming message
			}
			else
			{
				std::cout << "Error reading body from message type: ";
				std::cout << messageInTmp << std::endl;
				std::cout << ". Socket will be closed" << std::endl;
				socket_.close();
			}
		});
}

