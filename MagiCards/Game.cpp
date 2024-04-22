#include "Game.h"
#include "SDLException.h"
#include "Connection.h"
#include "MainMenu.h"
#include "CreateRoomMenu.h"
#include "JoinRoomMenu.h"
#include "DecksMenu.h"
#include "RoomMenu.h"
#include "Player.h"
#include <SDL_image.h>


Game::Game() {
	_isRunning = true;
	_connection = new Connection();
}

Game::~Game()
{
}

void Game::init()
{
	initSDL();
	createWindowAndRenderer();
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);

	if (TTF_Init() == -1) std::cerr << "failed to initialize ttf" << std::endl;

	_isRunning = true;
	_activeMenu = true;

	_mainMenu = new MainMenu(_renderer);
	_gameState = MAIN_MENU;

	static Mouse* m = new Mouse(_renderer);
	_mouse = m;
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
	SDL_CreateWindowAndRenderer(1280, 720, SDL_WINDOW_SHOWN, &_window, &_renderer);
	SDL_SetWindowTitle(_window, "MagiCards");
	if (_window == NULL || _renderer == NULL) throw SDLException(SDL_GetError());
}

void Game::handleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&(_mouse->getCursor()->x), &(_mouse->getCursor()->y));
			_mouse->setTipXY(_mouse->getCursor()->x, _mouse->getCursor()->y);
			break;

		case SDL_QUIT:
			_isRunning = false;
			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				switch (_gameState)
				{
					case MAIN_MENU:
						_mainMenu->handleEvents();
						break;

					case CREATE_ROOM:
						_createRoomMenu->handleEvents();
						break;

					case GAME_ROOM:
						_gameRoomMenu->handleEvents();
						break;

					case JOIN_ROOM:
						_joinRoomMenu->handleEvents();
						break;

					case DECKS_MENU:
						_decksMenu->handleEvents();
						break;
				}
			}
			break;

		case SDL_TEXTINPUT:
			
			switch (_gameState)
			{
				case CREATE_ROOM:
					_createRoomMenu->handleTextInputEvent(event.text);
					break;
				case JOIN_ROOM:
					_joinRoomMenu->handleTextInputEvent(event.text);
					break;
			}
			break;

		case SDL_KEYDOWN:
			switch (_gameState)
			{
			case CREATE_ROOM:
				_createRoomMenu->handleKeyDownEvent(event.key.keysym);
				break;
			case JOIN_ROOM:
				_joinRoomMenu->handleKeyDownEvent(event.key.keysym);
				break;
			}
		}
	}
}

std::queue<int> netEvents;
bool serverReady = false;

void Game::update()
{
	if (_activeMenu)
	{
		updateMenu();
	}
	else
	{
		// go to Preparing game menu
		//prepareGameTable(); //execute funcion on a new thread
		// check connection with the other user, if something wrong show connection error panel to user
		// create the game table
		//obtain 
		//update game
	}
	//std::queue<int> netEvents;
	//if (!netEvents.empty())
	//{
	//	int poped = netEvents.front();
	//	netEvents.pop();
	//	std::thread netThread([&]() { networkUpdate(poped); });
	//	netThread.detach();
	//}
}

void Game::networkUpdate(int e)
{
	if (e == 0 && !_connection->isServerConnected()) //start server connection
	{
		_connection->startServerConnection();
		serverReady = true;
		std::cout << "waiting for next client connection..." << std::endl;
		_connection->acceptNextConnection();
	}

	//for (size_t i = 0; i < 5; i++)
	//{
	//	std::cout << "processing net event: " << e << std::endl;
	//	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	//}
	//std::cout << "net thread finish" << std::endl;
	
}


void Game::updateMenu()
{
	switch (_gameState)
	{
		case MAIN_MENU:
			_mainMenu->update(_mouse);
			switch (_mainMenu->getButtonPressed())
			{
				case 0:
					_createRoomMenu = new CreateRoomMenu(_renderer);
					_gameState = CREATE_ROOM;
					break;

				case 1:
					_joinRoomMenu = new JoinRoomMenu(_renderer);
					_gameState = JOIN_ROOM;
					break;

				case 2:
					_decksMenu = new DecksMenu(_renderer);
					_gameState = DECKS_MENU;
					break;

				case 3:
					_isRunning = false;
					break;
			}
			_mainMenu->clearPressedButton();
			break;

		case CREATE_ROOM:
			_createRoomMenu->update(_mouse);
			switch (_createRoomMenu->getButtonPressed())
			{
				case 0: //Create button
				{
					if (!_connection->isServerConnected())
					{
						std::thread t([&]() {_connection->startServerConnection(); });
						t.detach(); // release the thread from parent as a daemon process
					}
					_playerHost = new Player(_createRoomMenu->getPlayerName(), _createRoomMenu->getSelectedDeck());
					_gameRoomMenu = new RoomMenu(_renderer, _playerHost, true);
					_gameState = GAME_ROOM;
					break;
				}
				case 1: //Back button
					delete _gameRoomMenu;
					_gameState = MAIN_MENU;
					break;
			}
			_createRoomMenu->clearPressedButton();
			break;

		case LOADING_SCREEN: // from joinRoom
		{
			_gameRoomMenu = new RoomMenu(_renderer, _playerClient, false);

			if (!_connection->isClientConnected())
			{
				std::thread t([&]() {
					_connection->startClientConnection(_joinRoomMenu->getServerAddress(), _joinRoomMenu->getServerPort());
					});
				t.detach(); // release the thread from parent as a daemon process
			}
			else {
				_gameRoomMenu->playerClientConnected();
				_gameState = GAME_ROOM;
			}

			break;
		}

		case JOIN_ROOM:
			_joinRoomMenu->update(_mouse);
			switch (_joinRoomMenu->getButtonPressed())
			{
				case 0: //Join button
					
					_playerClient = new Player(_joinRoomMenu->getPlayerName(), _joinRoomMenu->getSelectedDeck());
					//_gameRoomMenu = new RoomMenu(_renderer, _playerClient, false);
					//try
					//{
					//	_connection->startClientConnection(_joinRoomMenu->getServerAddress(), _joinRoomMenu->getServerPort());
					//	_gameRoomMenu->playerClientConnected();
					//}
					//catch (const std::exception&)
					//{
					//	std::cout << "Connection Exception error: " << std::endl;
					//	//renderErrorMessage() on top of the current menu
					//}
					_loadingScreen = new LoadingScreen(_renderer, "Creating game room...");
					_gameState = LOADING_SCREEN;
					//_gameState = GAME_ROOM;
					// make connection with the server game room
					break;
				case 1: //Back button
					_gameState = MAIN_MENU;
					break;
			}
			_joinRoomMenu->clearPressedButton();
			break;

		case GAME_ROOM:
			_gameRoomMenu->update(_mouse);
			if (_connection->isServerConnected())
			{
				_gameRoomMenu->playerHostConnected();
			}
			if (_connection->isClientConnected())
			{
				_gameRoomMenu->playerClientConnected();
			}
			switch (_gameRoomMenu->getButtonPressed())
			{
				case 0: // Play button
					//_loadingScreen = new PreparingGameTableScreen(_renderer);
					//_activeMenu = false;
					//_gameState = PREPARING_GAMETABLE;
					break;
				case 1: // Back button
					// close connection, server or client
					if (_gameRoomMenu->serverSide()) {
						_gameState = CREATE_ROOM;
					}
					else {
						_connection->closeClientConnection();
						_gameState = JOIN_ROOM;
					} 
					break;
			}
			_gameRoomMenu->clearPressedButton();
			break;
	}
}

void Game::render()
{	
	if (_activeMenu)
	{
		switch (_gameState)
		{
			case MAIN_MENU:
				_mainMenu->render();
				break;

			case CREATE_ROOM:
				_createRoomMenu->render();
				break;

			case GAME_ROOM:
				_gameRoomMenu->render();
				break;

			case JOIN_ROOM:
				_joinRoomMenu->render();
				break;

			case DECKS_MENU:
				_decksMenu->render();
				break;
			case LOADING_SCREEN:
				_loadingScreen->render();
				break;
		}
	}
	else
	{
		// render game table
		SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
	}

	_mouse->render();

	SDL_RenderPresent(_renderer);
	//SDL_SetRenderDrawColor(_renderer, 20, 20, 20, 255);
	SDL_RenderClear(_renderer);
}

void Game::release()
{
	_connection->clear();
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