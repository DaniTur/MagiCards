#pragma once
#include "Connection.h"
#include <SDL.h>
#include <iostream>
#include <stack>
#include "Menu.h"
#include "Mouse.h"

//testing
#include "MainMenu.h"
#include "CreateRoomMenu.h"
#include "RoomMenu.h"
#include "JoinRoomMenu.h"
#include "DecksMenu.h"

enum gameStates {MAIN_MENU, CREATE_ROOM, GAME_ROOM, JOIN_ROOM, DECKS_MENU};

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

	void createGameRoom();

	void joinGameRoom();

private:
	void createWindowAndRenderer();

	void initSDL();

	void updateMenu();

private:
	bool _isRunning;

	SDL_Window* _window;
	SDL_Renderer* _renderer;

	Connection _connection;

	std::stack<Menu*> _menuStack;
	//std::stack<MenuINH*> _menuINHStack;
	bool _activeMenu = false;

	Mouse* _mouse;

	int _gameState = -1;
	MainMenu* _mainMenu;
	CreateRoomMenu* _createRoomMenu;
	RoomMenu* _gameRoomMenu;
	JoinRoomMenu* _joinRoomMenu;
	DecksMenu* _decksMenu;	
	
	Player *_playerHost, *_playerClient;
};

