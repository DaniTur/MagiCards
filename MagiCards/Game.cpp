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
}

Game::~Game()
{
	release();
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


void Game::update()
{
	// update the server, processing the incoming messages from the peer
	if (netServer_)
	{
		netServer_->Update(-1);
	}
	else if (netClient_)
	{
		netClient_->Update(-1);
	}

	if (_activeMenu)
	{
		updateMenu();
	}
	else
	{
		//update game
	}

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
					_playerHost = new Player(_createRoomMenu->getPlayerName(), _createRoomMenu->getSelectedDeck());
					_gameRoomMenu = new RoomMenu(_renderer, _playerHost, true);
					netServer_ = new NetServer(30000);
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

		case JOIN_ROOM:
			_joinRoomMenu->update(_mouse);
			switch (_joinRoomMenu->getButtonPressed())
			{
				case 0: //Join button
					_playerClient = new Player(_joinRoomMenu->getPlayerName(), _joinRoomMenu->getSelectedDeck());
					netClient_ = new NetClient();
					_loadingScreen = new LoadingScreen(_renderer, "Creating game room...");
					_gameState = LOADING_SCREEN;
					break;
				case 1: //Back button
					_gameState = MAIN_MENU;
					break;
			}
			_joinRoomMenu->clearPressedButton();
			break;

		case LOADING_SCREEN: // from joinRoom
		{
			_gameRoomMenu = new RoomMenu(_renderer, _playerClient, false);
			if (!_gameRoomMenu->serverSide() && !netClient_->IsConnected())
			{
				bool connectionStatus = netClient_->Connect(_joinRoomMenu->getServerAddress(), _joinRoomMenu->getServerPort());
				if (!connectionStatus)
				{
					std::cout << "Connection to the server failed." << std::endl;
				}
				else
				{
					netClient_->SendPlayerData(_playerClient);

					_gameRoomMenu = new RoomMenu(_renderer, _playerClient, false);
					_gameRoomMenu->playerClientConnected();
					_gameState = GAME_ROOM;
				}
			}

			break;
		}

		case GAME_ROOM:
			_gameRoomMenu->update(_mouse);

			if (_gameRoomMenu->serverSide() && !netServer_->IsRunning()) {
				netServer_->Start();
				_gameRoomMenu->playerHostConnected();
			}

			switch (_gameRoomMenu->getButtonPressed())
			{
				case 0: // Play button
					std::cout << "Play button pressed" << std::endl;
					//_loadingScreen = new PreparingGameTableScreen(_renderer);
					//_activeMenu = false;
					//_gameState = PREPARING_GAMETABLE;
					break;
				case 1: // Back button
					// close connection, server or client
					if (_gameRoomMenu->serverSide()) {
						netServer_->Stop();
						_gameRoomMenu->playerHostDisconnected();
						_gameState = CREATE_ROOM;
					}
					else {
						netClient_->Disconnect();
						_gameRoomMenu->playerClientDisconnected();
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
	if (netClient_ != nullptr) delete netClient_;
	if (netServer_ != nullptr) delete netServer_;

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

