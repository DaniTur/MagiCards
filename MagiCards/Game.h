#pragma once

#include <SDL.h>
#include "Connection.h"

class Game
{
public:
	Game();

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
};

