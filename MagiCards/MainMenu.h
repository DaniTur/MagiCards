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

	void update();

	void render();

	void setActive();

	bool isActive();

private:

	SDL_Texture* _background;
	SDL_Rect _sRect, _dRect;
	SDL_Renderer* _renderer;

	bool _active;

	enum ButtonNames {CREATE_ROOM, JOIN_ROOM, DECKS, QUIT_GAME};
	Button* _buttons[4];


	//std::vector<Button> _buttons;
};

