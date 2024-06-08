#include "MainMenu.h"
#include <SDL_image.h>
#include <iostream>
#include "ResourcesList.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

MainMenu::MainMenu()
{
}

MainMenu::MainMenu(SDL_Renderer* renderer) :  _renderer(renderer)
{
	const int windowW = 1280;
	const int windowH = 720;

	_background = IMG_LoadTexture(_renderer, IMG_MAIN_MENU);

	_sRect.x = 0;
	_sRect.y = 0;
	_sRect.w = 1000;
	_sRect.h = 1000;
	_dRect.x = 0;
	_dRect.y = 0;
	_dRect.w = windowW;
	_dRect.h = windowH;


	_buttons[CREATE_ROOM_BTN] = new Button("Create Room", _renderer, 0, 0);
	_buttons[CREATE_ROOM_BTN]->setWindowXY((windowW/2) - (250), 0 + 50);
	_buttons[JOIN_ROOM_BTN] = new Button("Join Room", _renderer, 0, 100);
	_buttons[JOIN_ROOM_BTN]->setWindowXY((windowW / 2) - (250), 110 + 50);
	_buttons[DECKS_BTN] = new Button("Decks", _renderer, 0, 200);
	_buttons[DECKS_BTN]->setWindowXY((windowW / 2) - (250), 220 + 50);
	_buttons[QUIT_GAME_BTN] = new Button("Quit Game", _renderer, 0, 300);
	_buttons[QUIT_GAME_BTN]->setWindowXY((windowW / 2) - (250), 330 + 50);
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

void MainMenu::handleTextInputEvent(SDL_TextInputEvent event)
{
}
