#pragma once
#include "Connection.h"
#include <SDL.h>
#include <iostream>
#include <stack>
#include "Menu.h"
#include "Mouse.h"
#include "MainMenu.h"
#include "CreateRoomMenu.h"
#include "RoomMenu.h"
#include "JoinRoomMenu.h"
#include "DecksMenu.h"
#include "PreparingGameTableScreen.h"
#include "Connection.h"
#include "LoadingScreen.h"
#include "GameTable.h"
#include "NetServer.h"
#include "NetClient.h"

enum gameStates { MAIN_MENU, CREATE_ROOM, GAME_ROOM, JOIN_ROOM, DECKS_MENU, LOADING_SCREEN, GAME_TABLE };

class Game
{
public:
	Game();

	~Game();

	void init();

	void handleEvents();

	void update();

	void render();

	void release();

	bool isRunning();

private:
	void createWindowAndRenderer();

	void initSDL();

	void updateMenu();

	void updateNetworking();

	Player* constructPlayerFromData(std::string data);

	void onShuffleDeckMessage(Message& msg);

	void onDrawCardMessage(Message& msg);

private:
	bool _isRunning;

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;

	bool _activeMenu = false;

	Mouse* _mouse = nullptr;

	int _gameState = -1;
	MainMenu* _mainMenu = nullptr;
	CreateRoomMenu* _createRoomMenu = nullptr;
	RoomMenu* _gameRoomMenu = nullptr;
	JoinRoomMenu* _joinRoomMenu = nullptr;
	DecksMenu* _decksMenu = nullptr;
	LoadingScreen* _loadingScreen = nullptr;
	GameTable* gameTable_ = nullptr;

	Player *_playerHost = nullptr, *_playerClient = nullptr;

	NetClient* netClient_ = nullptr;
	NetServer* netServer_ = nullptr;

	bool waiting_ = false;
};

