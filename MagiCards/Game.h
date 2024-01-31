#pragma once
#include "Connection.h"
#include <SDL.h>
#include <iostream>
#include <stack>
#include "Menu.h"
#include "Mouse.h"

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

private:
	bool _isRunning;

	SDL_Window* _window;
	SDL_Renderer* _renderer;

	Connection _connection;

	std::stack<Menu*> _menuStack;
	bool _activeMenu = false;

	Mouse* _mouse;
};

