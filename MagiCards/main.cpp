#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <iostream>
#include "SDLException.h"
#include "Game.h"

using namespace std;


int main() {

	Game game;

	try {
		game.init();

		while (game.isRunning()) {
			game.handleEvents();

			game.update(); //game logic

			game.render();
		}

		game.release();
	
	}catch(std::exception &exception) {
		fprintf(stderr, exception.what());
		SDL_Quit();
		return -1;
	}

	return 0;
}