#include "Connection.h"
#include <thread>

//testing 
#include <mutex>

std::mutex mtxServer, mtxClient;

Connection::Connection() {
}

Connection::~Connection() {
	//clear();
}

int Connection::startServerConnection() {

	asio::io_context context; //connects with the system net interface
	asio::error_code errCode;

	// Give fake task to asio context to not finish
	//::io_context::work idleWork(context);

	//Run de context in a separate thread
	//std::thread threadContext = std::thread([&]() {context.run(); });

	unsigned short portNum = 30000;

	asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), portNum);

	//passive socket to listen for new connections
	asio::ip::tcp::acceptor acceptor(context, endpoint);
	std::cout << "[SERVER] server ready for incoming connections" << std::endl;
	
	mtxServer.lock();
	_serverConnected = true;
	mtxServer.unlock();

	asio::ip::tcp::socket socket = acceptor.accept();
	std::cout << "[SERVER] client accepted: " << socket.remote_endpoint() << std::endl;

	if (socket.is_open())
	{
		std::cout << "[SERVER] prepared for sending messages to client: " << socket.remote_endpoint() << std::endl;
	}

	//std::cout << "start server connection Acceptor creado" << std::endl;

	//acceptor.async_accept([&](std::error_code ec, asio::ip::tcp::socket s) {
	//	if (!ec) {
	//		// Llamar a la función de manejo cuando se acepta una conexión
	//		handle_accept(s);
	//	}
	//	else {
	//		std::cerr << "Error while accepting the connection: " << ec.message() << std::endl;
	//	}
	//});

	//std::cout << "acceptor async_accept " << std::endl;

	//context.run();

	//std::cout << "contexr. run acabado" << std::endl;

	////if (threadContext.joinable()) threadContext.join();
	//_serverConnected = true;

	return 0;
}

void Connection::acceptNextConnection() {
	//_socket = _acceptor.accept();
}

void Connection:: handle_accept(asio::ip::tcp::socket& socket) {
	std::cout << "Connection accepted" << std::endl;

	std::cout << "from client: " << socket.remote_endpoint() << std::endl;
	//asio::error_code errCode;

	//std::string message = "Hello client, im the server!";
	//socket.write_some(asio::buffer(message.data(), message.size()), errCode);

	//if (errCode)
	//{
	//	std::cerr << "Error writing some data to the client: " << errCode.message() << std::endl;

	//}
}

int Connection::startClientConnection(std::string ip, int port) {
	std::cout << "conenction-> starting client connection (handshake)" << std::endl;
	asio::io_context context; 

	asio::error_code errCode;

	unsigned short portNum = port;

	asio::ip::address ipAddress = asio::ip::make_address(ip, errCode);

	if (errCode)
	{
		std::cout << "Error making address from a std::string:\n" << errCode.message() << std::endl;
	}

	//endpoint(ip, port) we want to connect to
	asio::ip::tcp::endpoint endpoint(ipAddress, portNum);

	// Create a socket, the context deliver the implementation
	asio::ip::tcp::socket socket(context);

	socket.connect(endpoint, errCode);

	if (!errCode) 
	{
		std::cout << "Connected to the server!" << std::endl;

		//beging handleshake

		mtxClient.lock(); //shared variable among threads needs mutex to be accesed to write
		_clientConnected = true;
		mtxClient.unlock();

	}
	else
	{
		std::cout << "Failed to connect to address:\n" << errCode.message() << std::endl;
	}

	////TODO: socket is open explota
	//if (socket.is_open())
	//{
	//	std::string sRequest = 
	//		"TEST\r\n"
	//		"Player: paco\r\n"
	//		"Deck: 1\r\n\r\n";

	//	socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), errCode);
	//	std::this_thread::sleep_for(std::chrono::seconds(1));
	//	size_t bytes = socket.available();
	//	std::cout << "Bytes available!" << bytes << std::endl;

	//	if (bytes > 0)
	//	{
	//		std::vector<char> vBuffer;
	//		socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), errCode);

	//		for (auto c : vBuffer) {
	//			std::cout << c;
	//		}
	//	}
	//}
	//else {
	//	std::cout << "socket not open" << std::endl;
	//}

	return 0;
}


void Connection::clear() {
}

void Connection::closeServerConnection()
{
	//acceptor.close();
}

bool Connection::isServerConnected()
{
	mtxServer.lock(); //shared variable among threads needs mutex to be accesed to read
	bool isconnected = _serverConnected;
	mtxServer.unlock();

	return isconnected;
}

void Connection::closeClientConnection()
{

	_clientConnected = false;
}

bool Connection::isClientConnected()
{
	mtxClient.lock(); //shared variable among threads needs mutex to be accesed to read
	bool isconnected = _clientConnected;
	mtxClient.unlock();

	return isconnected;
}
