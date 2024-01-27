#include "Game.h"
#include <SDL.h>
#include "SDLException.h"
#include "Connection.h"
#include <iostream>


Game::Game() {
	_isRunning = true;
}

void Game::init()
{
/*	initSDL();
	createWindowAndRenderer();
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
	*/_isRunning = true;
	std::cout << "Game init" << std::endl;
}

void Game::initSDL()
{
	bool isInitialized = SDL_Init(SDL_INIT_VIDEO) == 0;
	if (!isInitialized) throw SDLException(SDL_GetError());
}

bool Game::isRunning() {
	return _isRunning;
}

void Game::createWindowAndRenderer()
{
	SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_SHOWN, &_window, &_renderer);
	SDL_SetWindowTitle(_window, "MagiCards");
	if (_window == NULL || _renderer == NULL) throw SDLException(SDL_GetError());
}

void Game::handleEvents()
{
	//SDL_Event event;
	//if (SDL_PollEvent(&event)) {
	//	if (event.type == SDL_QUIT) {
	//		_isRunning = false;
	//	}
	//}


	//std::cout << "Cartas en mano:" << std::endl;
	//std::cout << "1. Goblin Fumaporros-Creature" << std::endl;
	//std::cout << "2. Dragon Fumaporros-Creature" << std::endl;
	//std::cout << "3. Rey Fumaporros-Creature" << std::endl;
	//std::cout << "4. Esclavo Fumaporros-Creature" << std::endl;
	//std::cout << "5. Hechizo Fumaporros-Sorcery" << std::endl;
	//std::cout << "6. Forest-Basic Land" << std::endl;
	//std::cout << "7. Forest-Basic Land" << std::endl;

}

void Game::update()
{


	//switch (playerSelection)
	//{
	//case 1:
	//	std::cout << "Has invocado: Goblin Fumaporros-Creature" << std::endl;
	//	break;
	//case 2:
	//	std::cout << "Has invocado: Dragon Fumaporros-Creature" << std::endl;
	//	break;
	//case 3:
	//	std::cout << "Has invocado: Rey Fumaporros-Creature" << std::endl;
	//	break;
	//case 4:
	//	std::cout << "Has invocado: Esclavo Fumaporros-Creature" << std::endl;
	//	break;
	//case 5:
	//	std::cout << "Has lanzado: Fumaporros-Sorcery" << std::endl;
	//	break;
	//case 6:
	//	std::cout << "Has jugado: Forest-Basic Land" << std::endl;
	//	break;
	//case 7:
	//	std::cout << "Has jugado: Forest-Basic Land" << std::endl;
	//	break;
	//default:
	//	std::cout << "No se que has pulsado, debes estar emporrao..." << std::endl;
	//	break;
	//}
}

void Game::render()
{	
	//std::cout << "Renderizando carta " << std::endl;
	//SDL_RenderClear(_renderer);

	//renderizar imagenes

	//SDL_RenderPresent(_renderer); //muestra o "pinta" el render

}

void Game::release()
{
	_connection.clear();
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Game::createGameRoom() {
	std::cout << "creating a server socket" << std::endl;
	Connection connection;
	if (connection.startServerConnection() != 0)
		std::cout << "Throw ConnectionException at server" << std::endl;
	
}

void Game::joinGameRoom() {
	std::string ip;
	int port;

	std::cout << "Introduce the IP adress: " << std::endl;
	std::cin >> ip;

	std::cout << "Introduce the Port: " << std::endl;
	std::cin >> port;
	//TODO: check the input format

	std::cout << "creating a client socket" << std::endl;
	Connection connection;
	if (connection.startClientConnection(ip.c_str(), port) != 0)
		std::cout << "Throw ConnectionException at client" << std::endl;
}