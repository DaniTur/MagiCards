#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <iostream>
#include "SDLException.h"
#include "Game.h"

using namespace std;


int main() {

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
		fprintf(stderr, exception.what());
		SDL_Quit();
		return -1;
	}

	return 0;
}