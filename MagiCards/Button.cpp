#include "Button.h"
#include <SDL_image.h>
#include "ResourcesList.h"
#include <iostream>


Button::Button(const char* text, SDL_Renderer* renderer, SDL_Rect src, SDL_Rect dstRect)
	: _renderer(renderer), text_(text)
{
	static SDL_Texture* texture = IMG_LoadTexture(_renderer, IMG_BUTTON_TEXTURE);
	_texture = texture;

	// source rectangle (from texture)
	_sRect = {src.x , src.y, 500, 100}; // 500 w, 100 h of texture image 

	//destination rectangle(to screen)
	_dRect = dstRect;

	textFont_ = TTF_OpenFont(TEXT_FONT, 16);
}

Button::~Button()
{
	SDL_DestroyTexture(_texture);
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

bool Button::isSelected() const
{
	return _mouseHover;
}

void Button::render()
{
	if (text_ != "INACTIVE")
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

		if (!text_.empty())
		{
			SDL_Surface* surface = TTF_RenderText_Solid(textFont_, text_.c_str(), { 0, 0, 0, 255 });
			SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
			SDL_Rect dst;
			dst.w = (int)(_dRect.w * 0.85);
			dst.h = (int)(_dRect.h * 0.85);
			dst.x = _dRect.x + (_dRect.w / 2) - (dst.w / 2);
			dst.y = _dRect.y + (_dRect.h / 2) - (dst.h / 2);

			SDL_RenderCopy(_renderer, texture, nullptr, &dst);

			SDL_FreeSurface(surface);
			SDL_DestroyTexture(texture);
		}
	}
}

void Button::changeText(std::string_view newText)
{
	text_ = newText;
	std::cout << "button changed to: " << newText << std::endl;
}
