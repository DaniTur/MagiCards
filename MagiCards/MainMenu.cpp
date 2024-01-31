#include "MainMenu.h"
#include <SDL_image.h>
#include <iostream>

MainMenu::MainMenu()
{
}

MainMenu::MainMenu(SDL_Renderer* renderer) :  _renderer(renderer)
{
	_background = IMG_LoadTexture(_renderer, "MainMenu.png");
	_sRect.x = 0;
	_sRect.y = 0;
	_sRect.w = 1000;
	_sRect.h = 1000;
	_dRect.x = 0;
	_dRect.y = 0;
	_dRect.w = 800;
	_dRect.h = 600;

	const int windowX = 800;
	const int windowY = 600;

	_buttons[CREATE_ROOM] = new Button("Create Room", renderer, 0, 0);
	_buttons[CREATE_ROOM]->setWindowXY((windowX/2) - (250), 0 + 50);
	_buttons[JOIN_ROOM] = new Button("Join Room", renderer, 0, 100);
	_buttons[JOIN_ROOM]->setWindowXY((windowX / 2) - (250), 110 + 50);
	_buttons[DECKS] = new Button("Decks", renderer, 0, 200);
	_buttons[DECKS]->setWindowXY((windowX / 2) - (250), 220 + 50);
	_buttons[QUIT_GAME] = new Button("Quit Game", renderer, 0, 300);
	_buttons[QUIT_GAME]->setWindowXY((windowX / 2) - (250), 330 + 50);

	//_createRoomMenu = new CreateRoomMenu(_renderer);
}

MainMenu::~MainMenu()
{
	SDL_DestroyTexture(_background);
	for (Button* btn : _buttons)
	{
		delete btn;
	}
}

void MainMenu::handleEvents()
{
	bool anyButtonSelected = false;
	int i = 0;
	//@pre: only one button could be selected at time
	while (i < MAX_BUTTONS && !anyButtonSelected) {
		if (_buttons[i]->isSelected()) anyButtonSelected = true;
		else i++;
	}

	_buttonSelected = anyButtonSelected ? i : -1;
	std::cout << "MainMenu button selected: " << _buttonSelected << std::endl;
}

void MainMenu::update(Mouse* mouse)
{
	//update all menu components
	for (Button* btn : _buttons)
	{
		btn->update(mouse);
	}
}

void MainMenu::render()
{
	//render background
	SDL_RenderCopy(_renderer, _background, &_sRect, &_dRect);

	//render all menu components
	for (Button* btn : _buttons)
	{
		btn->render();
	}
}

int MainMenu::getButtonPressed()
{
	return _buttonSelected;
}

int MainMenu::menuType()
{
	return _MENU_TYPE;
}

void MainMenu::clearPressedButton()
{
	_buttonSelected = -1;
}

