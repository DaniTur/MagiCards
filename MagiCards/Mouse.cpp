#include "Mouse.h"
#include <SDL_image.h>
#include "ResourcesList.h"

Mouse::Mouse(SDL_Renderer * renderer) : _renderer(renderer)
{
	_texture = IMG_LoadTexture(_renderer, IMG_MOUSE);
	
	_cursor.x = 0;
	_cursor.y = 0;
	_cursor.w = 50;
	_cursor.h = 50;

	_tip.x = 0;
	_tip.y = 0;
	_tip.w = 1;
	_tip.h = 1;
}

Mouse::~Mouse()
{
}

void Mouse::render()
{
	SDL_RenderCopy(_renderer, _texture, NULL, &_cursor);
}

SDL_Rect* Mouse::getCursor()
{
	return &_cursor;
}

SDL_Rect* Mouse::getTip()
{
	return &_tip;
}

void Mouse::setTipXY(int x, int y)
{
	_tip.x = x;
	_tip.y = y;
}
