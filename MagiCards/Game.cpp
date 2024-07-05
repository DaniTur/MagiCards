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
	delete _mouse;
	delete _mainMenu;
	delete _createRoomMenu;
	delete _gameRoomMenu;
	delete _joinRoomMenu;
	delete _decksMenu;
	delete _loadingScreen;
	delete _playerHost;
	delete _playerClient;
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
					case GAME_TABLE:
						gameTable_->handleEvents();
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
		//update game table
		gameTable_->update(_mouse); // update table objects
		// do whatever the button clicked type is about
		if (gameTable_->actionButtonPressed())
		{
			switch (gameTable_->actionButtonType())
			{
				case ActionButtonType::DECK_SHUFFLE:
					if (gameTable_->preparationTurn())
					{
						std::vector<int> cardIDs = gameTable_->playerDeckShuffle();

						// get the card IDs stream from the shuffled deck

						Message msg;
						msg.header.id = MessageType::ShuffleDeck;
						for (int id : cardIDs)
						{
							msg << id;
						}
						msg << (int)cardIDs.size();

						if (netClient_) netClient_->Send(msg);
						else netServer_->MessageClient(msg);
						std::cout << "sended shuffled deck to opponent" << std::endl;
					}
				break;

				case ActionButtonType::DRAW:
				{
					int cardsToDraw = 0;

					if (!gameTable_->preparationTurn()) cardsToDraw = 1;
					else cardsToDraw = 5;
					
					gameTable_->playerDraw(cardsToDraw);

					Message m;
					m.header.id = MessageType::DrawCard;
					m << cardsToDraw;

					if (netClient_) netClient_->Send(m);
					else netServer_->MessageClient(m);

					// TODO: cambiar la gestión de los turnos porque no siempre que se robe se pasará el turno
					gameTable_->nextTurn();
					break;
				}

				case ActionButtonType::PLAY_CARD:
					// aplicar restricciones para poder ejecutar la funcionalidad del botón aquí?
					// las restricciones para 

					break;

				default:
					break;
			}
			gameTable_->clearButtonPressed();
		}
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
					if (!_createRoomMenu) delete _createRoomMenu;
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
					_playerHost = new Player(_createRoomMenu->getPlayerName(), _createRoomMenu->getSelectedDeck(), _renderer);
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
				{
					_playerClient = new Player(_joinRoomMenu->getPlayerName(), _joinRoomMenu->getSelectedDeck(), _renderer);
					_gameRoomMenu = new RoomMenu(_renderer, _playerClient, false);
					netClient_ = new NetClient();

					bool connectionStatus = netClient_->Connect(_joinRoomMenu->getServerAddress(), _joinRoomMenu->getServerPort());
					if (!connectionStatus)
					{
						std::cout << "Connection to the server failed." << std::endl;
					}
					else
					{
						_gameState = GAME_ROOM;
					}

					break;
				}
				case 1: //Back button
					_gameState = MAIN_MENU;
					break;
			}
			_joinRoomMenu->clearPressedButton();
			break;

		case LOADING_SCREEN:
		{
			if (!_gameRoomMenu->serverSide())	// Client side
			{
				if (!_playerClient->deckLoaded())
				{
					_playerClient->loadDeck();
					std::cout << "My deck loaded." << std::endl;
				}

				if (!_playerHost->deckLoaded()) // Server is the opponent of client
				{
					_playerHost->loadDeck();
					std::cout << "Opponent deck loaded." << std::endl;
				}
				
				// Create / load game table
				if (!gameTable_)
				{
					gameTable_ = new GameTable(_renderer, _playerClient, _playerHost, GameTable::Owner::client);
					std::cout << "game table ready" << std::endl;
					Message m;
					m.header.id = MessageType::GameTableLoaded;
					netClient_->Send(m);
					waiting_ = true;
				}
				//wait for server to load the game table
				if (!waiting_)
				{
					_gameState = GAME_TABLE;
					_activeMenu = false;
				}
			}
			else // Server side
			{
				if (!_playerHost->deckLoaded())
				{
					_playerHost->loadDeck();
					std::cout << "My deck loaded." << std::endl;
				}

				if (!_playerClient->deckLoaded()) // Client is the opponent of server
				{
					_playerClient->loadDeck();
					std::cout << "Opponent deck loaded." << std::endl;
				}

				// Create / load game table
				if (!gameTable_) {
					gameTable_ = new GameTable(_renderer, _playerHost, _playerClient, GameTable::Owner::server);
					std::cout << "game table ready" << std::endl;

					Message m;
					m.header.id = MessageType::GameTableLoaded;
					netServer_->MessageClient(m);

					waiting_ = true;
				}
				//wait client to load the game table
				if (!waiting_)
				{
					_gameState = GAME_TABLE;
					_activeMenu = false;
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
				// the client has the game table ready
				case MessageType::GameTableLoaded: 
				{
					std::cout << "Client table ready" << std::endl;
					waiting_ = false;
					break;
				}
				case MessageType::DrawCard: //opponent has drawed cards
					onDrawCardMessage(msg);
					break;

				case MessageType::ShuffleDeck:
					onShuffleDeckMessage(msg);
					break;
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
					break;
				}
				case MessageType::StartGame:
				{
					if (!_loadingScreen) delete _loadingScreen;
					_loadingScreen = new LoadingScreen(_renderer, "Preparing game table...");
					_gameState = LOADING_SCREEN;
					break;
				}

				case MessageType::GameTableLoaded: // Server has loaded the game table
					waiting_ = false;
					break;

				case MessageType::DrawCard:
					onDrawCardMessage(msg);
					break;

				case MessageType::ShuffleDeck:
					onShuffleDeckMessage(msg);
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
	Player* player = new Player(name, deck, _renderer);
	return player;
}

void Game::render()
{
	SDL_RenderClear(_renderer);

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
		//std::cout << "game table render()" << std::endl;
		gameTable_->render();
		//SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
	}

	_mouse->render();

	SDL_RenderPresent(_renderer);
	//if (gameTable_)
	//{
	//	SDL_Texture* tmp = gameTable_->getBackground();
	//}
	//SDL_SetRenderDrawColor(_renderer, 20, 20, 20, 255);
	//SDL_RenderClear(_renderer);
}

void Game::release()
{
	if (netClient_ != nullptr) delete netClient_;
	if (netServer_ != nullptr) delete netServer_;

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Game::onShuffleDeckMessage(Message &msg) {
	int numberOfCards = 0;
	std::vector<int> cardIDs;

	msg >> numberOfCards;

	int cardId = 0;
	for (int i = 0; i < numberOfCards; i++)
	{
		msg >> cardId;
		cardIDs.push_back(cardId);
	}

	std::reverse(cardIDs.begin(), cardIDs.end());

	std::cout << "opponent cards ids stream" << std::endl;
	for (int n : cardIDs) std::cout << n << ",";

	// TODO: gameTable_->createOpponentDeck(); // desde un vector de cardIDs cargar el deck del rival en el mismo orden que el vector

	std::cout << std::endl;
}

void Game::onDrawCardMessage(Message& msg)
{
	int cardsDrawed = 0;
	msg >> cardsDrawed;
	gameTable_->opponentPlayerDraw(cardsDrawed);
	std::cout << "opponent drawed " << cardsDrawed << "cards" << std::endl;
}
