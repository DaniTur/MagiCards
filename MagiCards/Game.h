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
#include "NetServer.h"
#include "NetClient.h"

enum gameStates { MAIN_MENU, CREATE_ROOM, GAME_ROOM, JOIN_ROOM, DECKS_MENU, LOADING_SCREEN, PREPARING_GAMETABLE };

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

private:
	bool _isRunning;

	SDL_Window* _window = NULL;
	SDL_Renderer* _renderer = NULL;

	bool _activeMenu = false;

	Mouse* _mouse = NULL;

	int _gameState = -1;
	MainMenu* _mainMenu = NULL;
	CreateRoomMenu* _createRoomMenu = NULL;
	RoomMenu* _gameRoomMenu = NULL;
	JoinRoomMenu* _joinRoomMenu = NULL;
	DecksMenu* _decksMenu = NULL;
	LoadingScreen* _loadingScreen = NULL;

	Player *_playerHost = NULL, *_playerClient = NULL;
	//Connection* _connection = NULL;

	NetClient* netClient_ = nullptr;
	NetServer* netServer_ = nullptr;
};

