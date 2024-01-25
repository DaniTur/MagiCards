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

		// Client or server selection
		int playerSelection = 0;
		bool server;
		int const CREATE_ROOM = 1;
		int const JOIN_ROOM = 2;
		std::cout << "1. Create Room" << std::endl;
		std::cout << "2. Join Room" << std::endl;
		std::cin >> playerSelection;
		
		switch (playerSelection)
		{
		case CREATE_ROOM:
			server = true;
			game.createGameRoom();
			break;
		case JOIN_ROOM:
			server = false;
			game.joinGameRoom();
			break;
		default:
			break;
		}

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