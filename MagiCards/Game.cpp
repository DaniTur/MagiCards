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
#include <stdexcept>


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
	delete _playerHost;
	delete _playerClient;
	release();
}

void Game::init()
{
	initSDL();

	window_ = std::unique_ptr<SDL_Window, WindowDestructor>
		(SDL_CreateWindow("MagiCards",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			1280, 720,
			SDL_WINDOW_SHOWN),
		SDL_DestroyWindow);

	if (window_.get() == NULL)
	{
		throw std::runtime_error(
			std::string("Runtime Error. Unable to initialize SDL_Window: ") + SDL_GetError());
	}

	renderer_ = SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED);
	if (renderer_ == NULL)
	{
		throw std::runtime_error(
			std::string("Runtime Error. Unable to initialize SDL_Renderer: ") + SDL_GetError());
	}

	SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255); // render clear color

	if (TTF_Init() == -1) std::cerr << "failed to initialize ttf" << std::endl;

	_isRunning = true;
	_activeMenu = true;

	_mainMenu = new MainMenu(renderer_);
	_gameState = GAME_STATE::MAIN_MENU;

	static Mouse* m = new Mouse(renderer_);
	_mouse = m;
}

void Game::initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER |SDL_INIT_EVENTS) != 0)
	{
		throw std::runtime_error(
			std::string("Runtime Error. Unable to initialize SDL: ") + SDL_GetError());
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		throw std::runtime_error(
			std::string("Runtime Error. Unable to initialize SDL_Image: ") + SDL_GetError());
	}
	
}

bool Game::isRunning()
{
	return _isRunning;
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
					case GAME_STATE::MAIN_MENU:
						_mainMenu->handleEvents();
						break;

					case GAME_STATE::CREATE_ROOM:
						_createRoomMenu->handleEvents();
						break;

					case GAME_STATE::GAME_ROOM:
						_gameRoomMenu->handleEvents();
						break;

					case GAME_STATE::JOIN_ROOM:
						_joinRoomMenu->handleEvents();
						break;

					case GAME_STATE::DECKS_MENU:
						_decksMenu->handleEvents();
						break;

					case GAME_STATE::GAME_TABLE:
						gameTable_->handleEvents();
						break;
				}
			}
			break;

		case SDL_TEXTINPUT:
			
			switch (_gameState)
			{
				case GAME_STATE::CREATE_ROOM:
					_createRoomMenu->handleTextInputEvent(event.text);
					break;
				case GAME_STATE::JOIN_ROOM:
					_joinRoomMenu->handleTextInputEvent(event.text);
					break;
			}
			break;

		case SDL_KEYDOWN:
			switch (_gameState)
			{
			case GAME_STATE::CREATE_ROOM:
				_createRoomMenu->handleKeyDownEvent(event.key.keysym);
				break;
			case GAME_STATE::JOIN_ROOM:
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

						Message msg;
						msg.header.id = MessageType::ShuffleDeck;
						for (int id : cardIDs)
						{
							msg << id;
						}
						msg << (int)cardIDs.size();

						if (netClient_) netClient_->Send(msg);
						else netServer_->MessageClient(msg);
					}
				break;

				case ActionButtonType::DRAW:
				{
					bool preparationTurn = gameTable_->preparationTurn();
					int cardsToDraw = preparationTurn ? 5 : 1;
					
					if (gameTable_->playerCanDraw())
					{
						gameTable_->playerDraw(cardsToDraw);

						Message m;
						m.header.id = MessageType::DrawCard;
						m << cardsToDraw;
						
						sendMessageToOpponent(m);


						if (preparationTurn) 
						{
							if (netClient_) gameTable_->clientPlayerPreparationTurnReady();
							else if (netServer_) gameTable_->hostPlayerPreparationTurnReady();

							Message m;
							m.header.id = MessageType::PreparationTurnReady;

							sendMessageToOpponent(m);
						} 
					}
					break;
				}

				case ActionButtonType::PLAY_CARD:
					// obtener la carta seleccionada, quitarla de la mano y ponerla en la posición de playingCard

					break;

				case ActionButtonType::END_TURN:
				{
					gameTable_->clearTurnActions();

					gameTable_->nextTurn();
					
					Message m;
					m.header.id = MessageType::NextTurn;

					sendMessageToOpponent(m);

					break;
				}

				default:
					break;
			}
			gameTable_->clearButtonPressed();
		}
	}

}

void Game::sendMessageToOpponent(Message& msg)
{
	if (netClient_) netClient_->Send(msg);
	else if (netServer_)  netServer_->MessageClient(msg);
}

void Game::updateMenu()
{
	switch (_gameState)
	{
		case GAME_STATE::MAIN_MENU:
			_mainMenu->update(_mouse);
			switch (_mainMenu->getButtonPressed())
			{
				case 0:
					if (!_createRoomMenu) delete _createRoomMenu;
					_createRoomMenu = new CreateRoomMenu(renderer_);
					_gameState = GAME_STATE::CREATE_ROOM;
					break;

				case 1:
					if (!_joinRoomMenu) delete _joinRoomMenu;
					_joinRoomMenu = new JoinRoomMenu(renderer_);
					_gameState = GAME_STATE::JOIN_ROOM;
					break;

				case 2:
					if (!_decksMenu) delete _decksMenu;
					_decksMenu = new DecksMenu(renderer_);
					_gameState = GAME_STATE::DECKS_MENU;
					break;

				case 3:
					_isRunning = false;
					break;
			}
			_mainMenu->clearPressedButton();
			break;

		case GAME_STATE::CREATE_ROOM:
			_createRoomMenu->update(_mouse);
			switch (_createRoomMenu->getButtonPressed())
			{
				case 0: //Create button
				{
					_playerHost = new Player(_createRoomMenu->getPlayerName(), _createRoomMenu->getSelectedDeck(), renderer_);
					_gameRoomMenu = new RoomMenu(renderer_, _playerHost, true);
					netServer_ = new NetServer(30000);
					_gameState = GAME_STATE::GAME_ROOM;
					break;
				}
				case 1: //Back button
					_gameState = GAME_STATE::MAIN_MENU;
					break;
			}
			_createRoomMenu->clearPressedButton();
			break;

		case GAME_STATE::JOIN_ROOM:
			_joinRoomMenu->update(_mouse);
			switch (_joinRoomMenu->getButtonPressed())
			{
				case 0: //Join button
				{
					_playerClient = new Player(_joinRoomMenu->getPlayerName(), _joinRoomMenu->getSelectedDeck(), renderer_);
					_gameRoomMenu = new RoomMenu(renderer_, _playerClient, false);
					netClient_ = new NetClient();

					bool connectionStatus = netClient_->Connect(_joinRoomMenu->getServerAddress(), _joinRoomMenu->getServerPort());
					if (!connectionStatus)
					{
						std::cout << "Connection to the server failed." << std::endl;
					}
					else
					{
						_gameState = GAME_STATE::GAME_ROOM;
					}

					break;
				}
				case 1: //Back button
					_gameState = GAME_STATE::MAIN_MENU;
					break;
			}
			_joinRoomMenu->clearPressedButton();
			break;

		case GAME_STATE::LOADING_SCREEN:
		{
			if (!_gameRoomMenu->serverSide())	// Client side
			{
				if (!_playerClient->deckLoaded())
					_playerClient->loadDeck();
				
				// Create / load game table
				if (!gameTable_)
				{
					gameTable_ = new GameTable(renderer_, _playerClient, _playerHost, OWNER::CLIENT);

					Message m;
					m.header.id = MessageType::GameTableLoaded;
					netClient_->Send(m);

					waiting_ = true;
				}
				//wait for opponent(server) to load the game table
				if (!waiting_)
				{
					_gameState = GAME_STATE::GAME_TABLE;
					_activeMenu = false;
				}
			}
			else // Server side
			{
				if (!_playerHost->deckLoaded())
					_playerHost->loadDeck();

				// Create / load game table
				if (!gameTable_) {
					gameTable_ = new GameTable(renderer_, _playerHost, _playerClient, OWNER::SERVER);

					Message m;
					m.header.id = MessageType::GameTableLoaded;
					netServer_->MessageClient(m);

					waiting_ = true;
				}
				//wait opponent(client) to load the game table
				if (!waiting_)
				{
					_gameState = GAME_STATE::GAME_TABLE;
					_activeMenu = false;
				}
			}	
			break;
		}

		case GAME_STATE::GAME_ROOM:
			_gameRoomMenu->update(_mouse);

			if (_gameRoomMenu->serverSide() && !netServer_->IsRunning()) {
				netServer_->Start();
				_gameRoomMenu->playerHostConnected();
			}

			switch (_gameRoomMenu->getButtonPressed())
			{
				case 0: // Start button, only server side can press it
				{
					loadingScreen_ = std::make_unique<LoadingScreen>(renderer_, "Preparing game table...");

					Message msg;
					msg.header.id = MessageType::StartGame;
					netServer_->MessageClient(msg);

					_gameState = GAME_STATE::LOADING_SCREEN;
					break;
				}
				case 1: // Back button
					// close connection, server or client
					if (_gameRoomMenu->serverSide()) {
						netServer_->Stop();
						_gameRoomMenu->playerHostDisconnected();
						_gameState = GAME_STATE::CREATE_ROOM;
					}
					else {
						netClient_->Disconnect();
						_gameRoomMenu->playerClientDisconnected();
						_gameState = GAME_STATE::JOIN_ROOM;
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

				case MessageType::PreparationTurnReady:
					gameTable_->clientPlayerPreparationTurnReady();
					break;

				case MessageType::NextTurn:
					gameTable_->nextTurn();
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
					loadingScreen_ = std::make_unique<LoadingScreen>(renderer_, "Preparing game table...");
					_gameState = GAME_STATE::LOADING_SCREEN;
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

				case MessageType::PreparationTurnReady:
					gameTable_->hostPlayerPreparationTurnReady();
					break;

				case MessageType::NextTurn:
					gameTable_->nextTurn();
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
	Player* player = new Player(name, deck, renderer_);
	return player;
}

void Game::render()
{
	SDL_RenderClear(renderer_);

	if (_activeMenu)
	{
		switch (_gameState)
		{
			case GAME_STATE::MAIN_MENU:
				_mainMenu->render();
				break;

			case GAME_STATE::CREATE_ROOM:
				_createRoomMenu->render();
				break;

			case GAME_STATE::GAME_ROOM:
				_gameRoomMenu->render();
				break;

			case GAME_STATE::JOIN_ROOM:
				_joinRoomMenu->render();
				break;

			case GAME_STATE::DECKS_MENU:
				_decksMenu->render();
				break;

			case GAME_STATE::LOADING_SCREEN:
				loadingScreen_.get()->render();
				break;
		}
	}
	else
	{
		gameTable_->render();
	}

	_mouse->render();

	SDL_RenderPresent(renderer_);
}

void Game::release()
{
	if (netClient_ != nullptr) delete netClient_;
	if (netServer_ != nullptr) delete netServer_;

	SDL_DestroyRenderer(renderer_);
	IMG_Quit();
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

	gameTable_->createOpponentDeck(cardIDs); // desde un vector de cardIDs cargar el deck del rival en el mismo orden que el vector
}

void Game::onDrawCardMessage(Message& msg)
{
	int cardsDrawed = 0;
	msg >> cardsDrawed;
	gameTable_->opponentPlayerDraw(cardsDrawed);
}
