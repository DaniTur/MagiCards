#pragma once
#include <SDL.h>
#include <iostream>
#include "Mouse.h"

class Button
{
public:
	Button();

	Button(const char* text, SDL_Renderer* renderer, int x, int y);
	
	~Button();

	void setWindowXY(int x, int y);

	void update(Mouse* mouse);

	bool isSelected();

	void render();

private:
	SDL_Texture* _texture;
	SDL_Rect _sRect, _dRect;

	SDL_Renderer* _renderer;

	int _x, _y, _width, _height;
	std::string _text;
	bool _mouseHover = false;

};

