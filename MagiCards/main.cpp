#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <iostream>
#include "SDLException.h"
#include "main.h"

using namespace std;


SDL_Window* window;
SDL_Renderer* renderer;

bool isRunning;

void initSDL()
{
	bool isInitialized = SDL_Init(SDL_INIT_VIDEO) == 0;
	if (!isInitialized) throw SDLException(SDL_GetError());
}

void init() {
	initSDL();
	createWindowAndRenderer();
	isRunning = true;
}

void createWindowAndRenderer() {
	SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_SHOWN, &window, &renderer);
	SDL_SetWindowTitle(window, "MagiCards");
	if (window == NULL || renderer == NULL) throw SDLException(SDL_GetError());
}

void releaseSDL() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void handleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT){
			isRunning = false;
		}
	}
}

void update() {

}

void render()
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	SDL_RenderClear(renderer);

	//renderizar imagenes

	SDL_RenderPresent(renderer); //muestra o "pinta" el render
}

int main() {
	try {
		init();

		while (isRunning) {
			handleEvents();

			update(); //game logic

			render();
		}

		releaseSDL();
	
	}catch(std::exception &exception) {
		fprintf(stderr, exception.what());
		SDL_Quit();
		return -1;
	}

	return 0;
}