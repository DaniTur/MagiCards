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
	updateNetworking();

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
					if (!_createRoomMenu) {
						delete _createRoomMenu;
						std::cout << "deleted _createRoomMenu" << std::endl;
					} 
					_createRoomMenu = new CreateRoomMenu(_renderer);
					_gameState = CREATE_ROOM;
					break;

				case 1:
					if (!_joinRoomMenu) delete _joinRoomMenu;
					_joinRoomMenu = new JoinRoomMenu(_renderer);
					_gameState = JOIN_ROOM;
					break;

				case 2:
					if (!_decksMenu) delete _decksMenu;
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
					_gameRoomMenu = new RoomMenu(_renderer, _playerClient, false);
					netClient_ = new NetClient();
					_loadingScreen = new LoadingScreen(_renderer, "Joining game room...");
					_gameState = LOADING_SCREEN;
					break;
				case 1: //Back button
					_gameState = MAIN_MENU;
					break;
			}
			_joinRoomMenu->clearPressedButton();
			break;

		case LOADING_SCREEN:
		{
			if (!_gameRoomMenu->serverSide() && !netClient_->IsConnected()) // Client and not connected to a server
			{
				bool connectionStatus = netClient_->Connect(_joinRoomMenu->getServerAddress(), _joinRoomMenu->getServerPort());
				if (!connectionStatus)
				{
					std::cout << "Connection to the server failed." << std::endl;
				}
			}
			else if (!_gameRoomMenu->serverSide() && !_playerClient->deckLoaded()) // Client connected to a server
			{
				// cargar(crearlo a partir del id del deck) los deck tanto del jugador propio como del oponente
				// loadDecks();
				std::cout << "loading decks..." << std::endl;
				_playerClient->loadDeck();
				// crear/cargar tablero con los jugadores y decks
				// enviar mensaje al server de que esta todo cargado y se puede empezar la partida
			}
			else if (!_playerHost->deckLoaded()) // Server
			{
				std::cout << "loading decks..." << std::endl;
				_playerHost->loadDeck();
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
				case 0: // Start button, only server side can press it
				{
					std::cout << "Play button pressed" << std::endl;
					_loadingScreen = new LoadingScreen(_renderer, "Preparing game table...");

					Message msg;
					msg.header.id = MessageType::StartGame;
					netServer_->MessageClient(msg);

					_gameState = LOADING_SCREEN;
					break;
				}
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

void Game::updateNetworking()
{
	std::vector<Message> messagesToProcess;
	// update the server, processing the incoming messages from the peer
	if (netServer_)
	{
		messagesToProcess =  netServer_->GetMessagesToUpdate(-1);
		for (Message msg : messagesToProcess)
		{
			switch (msg.header.id)
			{
				case MessageType::PlayerData:
				{
					std::cout << "*Player data*" << std::endl;
					std::cout << "Header: \n" << msg << std::endl;
					char bodydata[20] = "";
					msg >> bodydata;
					std::cout << "Body: \n" << bodydata << std::endl;

					//create the client player object
					_playerClient = constructPlayerFromData(std::string(bodydata));
					std::cout << "player created from data" << std::endl;
					_gameRoomMenu->joinPlayerAsClient(_playerClient);
					_gameRoomMenu->playerClientConnected();
					//std::cout << "Body: \n" << msg.body.data() << std::endl;
					Message m;
					m.header.id = MessageType::PlayerData;
					m << "name:player1,deck:1";
					netServer_->MessageClient(m);
					break;
				}
			}
		}
	}
	else if (netClient_)
	{
		messagesToProcess = netClient_->GetMessagesToUpdate(-1);
		for (Message msg : messagesToProcess)
		{
			switch (msg.header.id)
			{
				case MessageType::ServerAccept:
				{
					std::cout << "Connected to server!" << std::endl;
					_gameRoomMenu->playerClientConnected();

					Message m;
					m.header.id = MessageType::PlayerData;
					//std::string name = _playerClient->getName();
					//const char* body = name.data();

					//size_t len = std::strlen(body);
					//for (size_t i = 0; i < len; ++i) {
					//	m << body[i];
					//}
					m << "name:player2,deck:2";
					netClient_->Send(m);
					break;
				}
				case MessageType::PlayerData:
				{
					std::cout << "*Player data*" << std::endl;
					std::cout << "Header: \n" << msg << std::endl;
					char bodydata[20] = "";
					msg >> bodydata;
					std::cout << "Body: \n" << bodydata << std::endl;

					_playerHost = constructPlayerFromData(std::string(bodydata));
					_gameRoomMenu->joinPlayerAsHost(_playerHost);
					_gameRoomMenu->playerHostConnected();

					_gameState = GAME_ROOM;
					break;
				}
				case MessageType::StartGame:
					_gameState = LOADING_SCREEN;
					break;
			}
		}
	}
}

Player* Game::constructPlayerFromData(std::string data)
{
	std::string name;
	int deck;

	// Obtain player name
	size_t namePos = data.find("name:");

	// check if "name:" is part of the string 
	if (namePos != std::string::npos) {

		// Extract the name from the position "name:" untill find a comma ","
		size_t commaPos = data.find(",", namePos);
		if (commaPos != std::string::npos) {
			name = data.substr(namePos + 5, commaPos - (namePos + 5)); // 5 is the length of "name:"
		}
	}

	// Obtain player deck
	size_t deckPos = data.find("deck:");
	if (deckPos != std::string::npos) {
		// Extract the deck from the posición "deck:" hasta el final de la cadena
		std::istringstream(data.substr(deckPos + 5)) >> deck; // 5 es la longitud de "deck:"
	}
	Player* player = new Player(name, deck);
	return player;
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

