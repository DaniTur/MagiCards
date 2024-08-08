#pragma once
#include <SDL.h>
#include <string>
#include <SDL_ttf.h>
#include "Mouse.h"

class Button
{
public:

	Button(const char* text, SDL_Renderer* renderer, SDL_Rect src, SDL_Rect dstRect);
	
	~Button();

	void update(Mouse* mouse);

	bool isSelected() const;

	void render();

protected:
	void changeText(std::string_view newText);

private:
	SDL_Texture* _texture;
	SDL_Rect _sRect;
	SDL_Rect _dRect;

	SDL_Renderer* _renderer;

	TTF_Font* textFont_ = nullptr;
	std::string text_;
	bool _mouseHover = false;
};

