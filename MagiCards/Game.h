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
#include "Common.h"

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

	void initSDL();

	void updateMenu();

	void updateNetworking();

	Player* constructPlayerFromData(std::string data);

	void onShuffleDeckMessage(Message& msg);

	void onDrawCardMessage(Message& msg);

	void sendMessageToOpponent(Message& msg);

private:
	bool _isRunning;

	//SDL_Window* window_ = nullptr;
	using WindowDestructor = std::function<void(SDL_Window*)>;
	std::unique_ptr<SDL_Window, WindowDestructor> window_;

	SDL_Renderer* renderer_ = nullptr;

	bool _activeMenu = false;

	Mouse* _mouse = nullptr;

	GAME_STATE _gameState;

	MainMenu* _mainMenu = nullptr;
	CreateRoomMenu* _createRoomMenu = nullptr;
	RoomMenu* _gameRoomMenu = nullptr;
	JoinRoomMenu* _joinRoomMenu = nullptr;
	DecksMenu* _decksMenu = nullptr;

	std::unique_ptr<LoadingScreen> loadingScreen_;

	GameTable* gameTable_ = nullptr;

	Player *_playerHost = nullptr, *_playerClient = nullptr;

	NetClient* netClient_ = nullptr;
	NetServer* netServer_ = nullptr;

	bool waiting_ = false;
};

