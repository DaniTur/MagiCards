#include "NetConnection.h"

NetConnection::NetConnection(asio::io_context& context) 
	: context_(context), remoteSocket_(context)
{

}

NetConnection::~NetConnection()
{
}

bool NetConnection::ConnectToServer(asio::ip::tcp::endpoint endpoint)
{
	return false;
}

void NetConnection::Disconnect()
{
}

bool NetConnection::IsConnected()
{
	return false;
}

bool NetConnection::Send()
{
	return false;
}
