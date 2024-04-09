#pragma once
#include <iostream>
#include <string>
#include "Mouse.h"
#include <SDL_ttf.h>

class TextInput
{
public:
	TextInput();

	TextInput(SDL_Renderer* renderer, int textureX, int textureY, std::string defaultText);

	~TextInput();

	void handleEvents();

	void update(Mouse* mouse);

	void render();

	std::string getText();

	void focus();

	void unfocus();

	bool isFocused();

	bool isSelected();

	void startTextInput();

	void stopTextInput();

	void addText(const char* text);

	void deleteChar();

private:
	std::string _text = "";
	std::string _defaultText = "";

	SDL_Rect _sRect, _dRect, _dTextRect;
	SDL_Renderer* _renderer;
	SDL_Texture* _texture;

	TTF_Font* _textFont;
	SDL_Color _textColor = { 0, 0, 0 }; // black
	SDL_Surface* _surfaceText;

	int _fontSize = 18;
	const int _MAX_CHARS = 15;

	bool _mouseHover = false;
	bool _focused = false;
};

