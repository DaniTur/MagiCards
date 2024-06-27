#include "TextInput.h"
#include <SDL.h>
#include <SDL_image.h>
#include "ResourcesList.h"

TextInput::TextInput()
{
}

TextInput::TextInput(SDL_Renderer* renderer, int dRectX, int dRectY, std::string defaultText)
	: _renderer(renderer), _defaultText(defaultText)
{
	_texture = IMG_LoadTexture(_renderer, IMG_TEXT_INPUT);

	_sRect.x = 0;
	_sRect.y = 0;
	_sRect.w = 500;
	_sRect.h = 100;

	_dRect.x = dRectX;
	_dRect.y = dRectY;
	_dRect.w = 350;
	_dRect.h = 30;

	_dTextRect.w = 0;
	_dTextRect.h = 0;
	_dTextRect.x = 0;
	_dTextRect.y = 0;

	_textFont = TTF_OpenFont(TEXT_FONT, _fontSize);
}

TextInput::~TextInput()
{
	SDL_DestroyTexture(_texture);
	TTF_CloseFont(_textFont);
}

void TextInput::handleEvents()
{
}

void TextInput::update(Mouse* mouse)
{
	if (SDL_HasIntersection(mouse->getTip(), &_dRect))
	{
		_mouseHover = true;
	}
	else
	{
		_mouseHover = false;
	}
}

void TextInput::render()
{
	SDL_Texture* textTexture = nullptr;
	SDL_Surface* textSurface = nullptr;

	SDL_RenderCopy(_renderer, _texture, &_sRect, &_dRect); // input background

	if (_text.size() <= 0) {
		//placeholder
		textSurface = TTF_RenderText_Solid(_textFont, _defaultText.c_str(), {0, 0, 0, 102});

		textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);

		_dTextRect.w = textSurface->w;
		_dTextRect.h = textSurface->h;
		_dTextRect.x = _dRect.x * 1.2;
		_dTextRect.y = _dRect.y + (_dRect.h / 2) - (textSurface->h / 2);

		SDL_FreeSurface(textSurface);
		SDL_RenderCopy(_renderer, textTexture, NULL, &_dTextRect);
		SDL_DestroyTexture(textTexture);
	}
	else if (_text.size() > 0) // if there is text to draw
	{
		textSurface = TTF_RenderText_Solid(_textFont, _text.c_str(), _textColor);

		textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);

		_dTextRect.w = textSurface->w;
		_dTextRect.h = textSurface->h;
		_dTextRect.x = _dRect.x * 1.2;
		_dTextRect.y = _dRect.y + (_dRect.h / 2) - (textSurface->h / 2);

		SDL_FreeSurface(textSurface);
		SDL_RenderCopy(_renderer, textTexture, NULL, &_dTextRect);
		SDL_DestroyTexture(textTexture);
	}
	//SDL_RenderCopy(_renderer, texture.get(), NULL, &_dTextRect);
	//SDL_RenderCopy(_renderer, textTexture, NULL, &_dTextRect);
}

std::string TextInput::getText()
{
	return _text;
}

void TextInput::focus()
{
	_sRect.x = 500;
	_dRect.w = 355;
	_dRect.h = 35;
	_focused = true;
}

void TextInput::unfocus()
{
	_sRect.x = 0;
	_dRect.w = 350;
	_dRect.h = 30;
	_focused = false;
}

bool TextInput::isFocused()
{
	return _focused;
}

bool TextInput::isSelected()
{
	return _mouseHover;
}

void TextInput::startTextInput()
{
	SDL_StartTextInput();
}

void TextInput::stopTextInput()
{
	SDL_StopTextInput();
}

void TextInput::addText(const char* text)
{
	if (_text.size() < _MAX_CHARS)
	{
		_text += std::string(text);
	}
}

void TextInput::deleteChar()
{
	_text = _text.substr(0, _text.size() - 1);
}
