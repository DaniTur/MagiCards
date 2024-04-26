#include "LoadingScreen.h"
#include <SDL_image.h>


LoadingScreen::LoadingScreen(SDL_Renderer* renderer, std::string message) : _renderer(renderer), _message(message)
{
	const int windowW = 1280;
	const int windowH = 720;

	_background = IMG_LoadTexture(_renderer, "D:\\MagiCardsProject\\MagiCards\\MagiCards\\LoadingScreen.png");

	_sRect.x = 0;
	_sRect.y = 0;
	_sRect.w = 1280;
	_sRect.h = 720;
	_dRect.x = 0;
	_dRect.y = 0;
	_dRect.w = windowW;
	_dRect.h = windowH;

	_textFont = TTF_OpenFont("D:\\MagiCardsProject\\MagiCards\\MagiCards\\OpenSans-Regular.ttf", _fontSize);
	_surfaceText = TTF_RenderText_Solid(_textFont, message.c_str(), { 0, 0, 0, 255 });

	_textTexture = SDL_CreateTextureFromSurface(_renderer, _surfaceText);

	_dTextRect.w = _surfaceText->w;
	_dTextRect.h = _surfaceText->h;
	_dTextRect.x = (windowW /2) - (_surfaceText->w /2);
	_dTextRect.y = (windowH /2) - (_surfaceText->h /2);

	SDL_FreeSurface(_surfaceText);
}

void LoadingScreen::render()
{
	//render background
	SDL_RenderCopy(_renderer, _background, &_sRect, &_dRect);

	//render message
	SDL_RenderCopy(_renderer, _textTexture, NULL, &_dTextRect);
}
