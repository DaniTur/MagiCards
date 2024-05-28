#define SDL_MAIN_HANDLED
#define _CRTDBG_MAP_ALLOC

#include <SDL.h>
#include <iostream>
#include "SDLException.h"
#include "Game.h"
#include <crtdbg.h>

using namespace std;


int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//_CrtSetBreakAlloc(468);

	const int FPS = 60;
	const int frameDelay = 1000 / 60;
	Uint32 frameStart;
	int frameTime;

	Game game;


	try {
		game.init();

		SDL_ShowCursor(SDL_DISABLE);

		while (game.isRunning()) {
			frameStart = SDL_GetTicks();

			game.handleEvents();

			game.update(); //game logic

			game.render();

			frameTime = SDL_GetTicks() - frameStart;
			//std::cout << frameTime << std::endl;
			if (frameDelay > frameTime)
			{
				SDL_Delay(frameDelay - frameTime);
			}
		}

		game.release();
	
	}catch(std::exception &exception) {
		std::cout << "exception in main" << std::endl;
		fprintf(stderr, exception.what());
		SDL_Quit();
		return -1;
	}

	_CrtDumpMemoryLeaks();
	return 0;
}