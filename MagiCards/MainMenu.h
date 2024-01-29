#pragma once

#include <iostream>
#include <vector>
#include "Button.h"

class MainMenu
{
public:
	MainMenu();

	MainMenu(SDL_Renderer* renderer);
	
	~MainMenu();

	void handleEvents();

	void update(Mouse* mouse);

	void render();

	void setActive();

	void setInactive();

	int getButtonPressed(); //return index of the pressed button of the list

	bool isActive();

	enum ButtonNames { CREATE_ROOM, JOIN_ROOM, DECKS, QUIT_GAME };

private:

	SDL_Texture* _background;
	SDL_Rect _sRect, _dRect;
	SDL_Renderer* _renderer;

	bool _active;

	
	Button* _buttons[4];

	int _buttonPressed = -1;

	//std::vector<Button> _buttons;
};

