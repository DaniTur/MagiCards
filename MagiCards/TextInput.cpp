#include "TextInput.h"
#include <SDL.h>
#include <SDL_image.h>


TextInput::TextInput()
{
}

TextInput::TextInput(SDL_Renderer* renderer, int dRectX, int dRectY, std::string defaultText)
	: _renderer(renderer), _defaultText(defaultText)
{
	_texture = IMG_LoadTexture(_renderer, "TextInput.png");

	_sRect.x = 0;
	_sRect.y = 0;
	_sRect.w = 500;
	_sRect.h = 100;

	_dRect.x = dRectX;
	_dRect.y = dRectY;
	_dRect.w = 350;
	_dRect.h = 30;

	_textFont = TTF_OpenFont("OpenSans-Regular.ttf", _fontSize);
}

TextInput::~TextInput()
{
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
	SDL_Texture* textTexture = NULL;

	SDL_RenderCopy(_renderer, _texture, &_sRect, &_dRect); // input background

	if (_text.size() <= 0) {
		//placeholder
		_surfaceText = TTF_RenderText_Solid(_textFont, _defaultText.c_str(), {0, 0, 0, 102});
		textTexture = SDL_CreateTextureFromSurface(_renderer, _surfaceText);
		
		//adjust the text width/heigth by the surface width/heigth
		_dTextRect.w = _surfaceText->w;
		_dTextRect.h = _surfaceText->h;
		_dTextRect.x = _dRect.x * 1.2;
		_dTextRect.y = _dRect.y + (_dRect.h / 2) - (_surfaceText->h / 2);

		SDL_FreeSurface(_surfaceText);
	}
	else if (_text.size() > 0) // if there is text to draw
	{
		_surfaceText = TTF_RenderText_Solid(_textFont, _text.c_str(), _textColor);
		textTexture = SDL_CreateTextureFromSurface(_renderer, _surfaceText);

		_dTextRect.w = _surfaceText->w;
		_dTextRect.h = _surfaceText->h;
		_dTextRect.x = _dRect.x * 1.2;
		_dTextRect.y = _dRect.y + (_dRect.h / 2) - (_surfaceText->h / 2);

		SDL_FreeSurface(_surfaceText);
	}
	SDL_RenderCopy(_renderer, textTexture, NULL, &_dTextRect);
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
