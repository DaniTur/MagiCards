#pragma once
#include <asio.hpp>
#include "NetConnection.h"
#include "Message.h"
#include "TSQueue.h"

// Esta clase es la responsable de generar un espacio de procesamiento para el contexto de la biblioteca Asio 
// de la parte servidora de la aplicaci�n, que hace de interfaz y gestiona la comunicaci�n entre la capa m�s cercana 
// a la de red(mediante una Connection)  y la parte responsable de la gesti�n general del juego(Game) 
class NetServer
{
public:
	NetServer(int port);

	~NetServer();

	bool Start();

	void Stop();

	// Asynchronous
	void WaitForClientConnection(); 

	void MessageClient(Message message);

	bool OnClientConnect(std::shared_ptr<NetConnection> clientConnection);

	bool IsRunning();

	std::vector<Message> GetMessagesToUpdate(size_t nMaxMessages);

private:
	asio::io_context context_;

	std::thread contextThread_;

	asio::ip::tcp::acceptor acceptor_;

	std::shared_ptr<NetConnection> clientConnection_;

	TSQueue<Message> messegesInQueue_;

	bool running_ = false;
};

