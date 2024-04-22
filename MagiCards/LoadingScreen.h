#pragma once
#include <SDL.h>
#include <string>
#include <SDL_ttf.h>

class LoadingScreen
{
public:

	LoadingScreen(SDL_Renderer* renderer, std::string message);

	~LoadingScreen();

	void render();

private:

	SDL_Texture* _background;
	SDL_Rect _sRect, _dRect, _dTextRect;
	SDL_Renderer* _renderer;

	std::string _message = "";
	SDL_Surface* _surfaceText;
	SDL_Texture* _textTexture;

	TTF_Font* _textFont;
	int _fontSize = 18;
	SDL_Color _textColor = { 0, 0, 0 }; // black
};

