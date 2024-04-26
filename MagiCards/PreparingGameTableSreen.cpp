#include "PreparingGameTableScreen.h"
#include <SDL_image.h>

PreparingGameTableScreen::PreparingGameTableScreen(SDL_Renderer* renderer) : _renderer(renderer)
{
	_background = IMG_LoadTexture(_renderer, "D:\\MagiCardsProject\\MagiCards\\MagiCards\\preparingGameTable.png");
	const int windowW = 1280;
	const int windowH = 720;
	_sRect.x = 0;
	_sRect.y = 0;
	_sRect.w = 1000;
	_sRect.h = 1000;
	_dRect.x = 0;
	_dRect.y = 0;
	_dRect.w = windowW;
	_dRect.h = windowH;
}

PreparingGameTableScreen::~PreparingGameTableScreen()
{
	SDL_DestroyTexture(_background);
}

void PreparingGameTableScreen::render()
{
	SDL_RenderCopy(_renderer, _background, &_sRect, &_dRect);
}
