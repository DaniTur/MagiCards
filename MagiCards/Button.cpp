#include "Button.h"
#include <SDL_image.h>
#include "ResourcesList.h"

Button::Button()
{
}

Button::Button(const char* text, SDL_Renderer* renderer, int textureX, int textureY) 
	: _text(text), _renderer(renderer), _x(textureX), _y(textureY)
{
	static SDL_Texture* texture = IMG_LoadTexture(_renderer, IMG_BUTTON_TEXTURE);
	_texture = texture;

	// source rectangle (from texture)
	_sRect.x = textureX;
	_sRect.y = textureY;
	_sRect.h = 100;
	_sRect.w = 500;


	//destination rectangle(to screen)
	_dRect.x = textureX;
	_dRect.y = textureY;
	_dRect.h = 100;
	_dRect.w = 500;

}

Button::~Button()
{
	SDL_DestroyTexture(_texture);
}

// set the destination
void Button::setWindowXY(int x, int y)
{
	_dRect.x = x;
	_dRect.y = y;
}

void Button::update(Mouse* mouse)
{
	if (SDL_HasIntersection(mouse->getTip(), &_dRect))
	{
		_mouseHover = true;
		//play mouse hover sound
	}
	else
	{
		_mouseHover = false;
	}
}

bool Button::isSelected()
{
	return _mouseHover;
}

void Button::render()
{
	if (_mouseHover)
	{
		_sRect.x = 500;
	}
	else
	{
		_sRect.x = 0;
	}
	SDL_RenderCopy(_renderer, _texture, &_sRect, &_dRect);
}
