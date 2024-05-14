#pragma once
#include <asio.hpp>
#include <thread>
#include "NetConnection.h"
#include "Message.h"
#include "TSQueue.h"
#include "Player.h"

// Esta clase es la responsable de generar un espacio de procesamiento para el contexto de la biblioteca Asio 
// de la parte cliente de la aplicación, que hace de interfaz y gestiona la comunicación entre la capa más cercana 
// a la de red(mediante una Connection)  y la parte responsable de la gestión general del juego(Game) 
class NetClient
{
public:
	
	NetClient();
	
	~NetClient();

	bool Connect(const std::string& host, const int port);
	
	void Disconnect();
	
	bool IsConnected();

	//void Update(size_t nMaxMessages = -1);
	std::vector<Message> GetMessagesToUpdate(size_t nMaxMessages = -1);

	void Send(const Message& message);

	void SendPlayerData(const Player* player);

private:
	
	asio::io_context context_;

	std::thread contextThread_;
	
	asio::ip::tcp::socket socket_;

	std::unique_ptr<NetConnection> connection_;

	TSQueue<Message> messagesInQueue_;
};

