#include "Game.h"
#include <SDL.h>
#include "SDLException.h"
#include "Connection.h"
#include <iostream>


Game::Game() {
	_window = NULL;
	_renderer = NULL;
	_mainMenu = NULL;
	_isRunning = true;
}

Game::~Game()
{
}

void Game::init()
{
	initSDL();
	createWindowAndRenderer();
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
	_isRunning = true;

	//Main Menu
	_mainMenu = new MainMenu(_renderer);
	_mainMenu->setActive();

	static Mouse* m = new Mouse(_renderer);
	_mouse = m;

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
	SDL_GetMouseState(&(_mouse->getCursor()->x), &(_mouse->getCursor()->y));
	_mouse->setTipXY(_mouse->getCursor()->x, _mouse->getCursor()->y);

	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			_isRunning = false;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (_mainMenu->isActive())
				{
					_mainMenu->handleEvents();
					
					switch (_mainMenu->getButtonPressed())
					{
					case _mainMenu->CREATE_ROOM:
						std::cout << "Pulsando Create Room" << std::endl;
						_mainMenu->setInactive();
						break;
					case _mainMenu->JOIN_ROOM:
						std::cout << "Pulsando Join Room" << std::endl;
						_mainMenu->setInactive();
						break;
					case _mainMenu->DECKS:
						std::cout << "Pulsando Decks" << std::endl;
						_mainMenu->setInactive();
						break;
					case _mainMenu->QUIT_GAME:
						std::cout << "Pulsando Quit Game" << std::endl;
						_isRunning = false;
						break;
					default:
						break;
					}
				}
				
			}
			
		default:
			break;
		}

	}
}

void Game::update()
{
	if (_mainMenu->isActive())
	{
		_mainMenu->update(_mouse);
	}
	else
	{
		// update logic
	}

}

void Game::render()
{	
	if (_mainMenu->isActive())
	{
		_mainMenu->render();
	}
	else
	{
		// render game
		SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
	}

	_mouse->render();

	SDL_RenderPresent(_renderer);
	//SDL_SetRenderDrawColor(_renderer, 20, 20, 20, 255);
	SDL_RenderClear(_renderer);
	
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