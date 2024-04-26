#pragma once
#include <SDL.h>

class PreparingGameTableScreen
{
public:
	PreparingGameTableScreen(SDL_Renderer* renderer);

	~PreparingGameTableScreen();

	void update();

	void render();

private:

	SDL_Texture* _background;
	SDL_Rect _sRect, _dRect;
	SDL_Renderer* _renderer;

	bool _gameTableReady = false;
};

